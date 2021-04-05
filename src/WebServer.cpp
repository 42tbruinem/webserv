/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 16:00:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 15:43:20 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "Client.hpp"
#include "WebServer.hpp"
#include "Utilities.hpp"
#include "Context.hpp"
#include <exception>

//-----------------------------------------------Constructors-----------------------------------------------

bool	identicalServersDetected(std::vector<Context*>& servers)
{
	std::map<std::string, const Properties*>	ports;

	for (size_t i = 0 ; i < servers.size(); i++)
	{
		if (ports.count(servers[i]->getProperties().ip_port.second))
		{
			for (std::vector<std::string>::const_iterator it = servers[i]->getProperties().server_names.begin();
				 it != servers[i]->getProperties().server_names.end(); it++)
			{
				std::vector<std::string> tmp = ports[servers[i]->getProperties().ip_port.second]->server_names;
				if (std::find(tmp.begin(), tmp.end(), *it) != tmp.end())
					return (true);
			}
		}
		if (!servers[i]->getProperties().ip_port.second.empty())
			ports[servers[i]->getProperties().ip_port.second] = &servers[i]->getProperties();
	}
	return (false);
}

WebServer::WebServer(char *config_path) :
Context(),
servers(),
clients(),
ioset(),
activity(ioset)
{
	this->keywords.push_back("server");

	Configuration	config(config_path, this);
	config.parse();

	if (identicalServersDetected(this->children))
		throw std::runtime_error("Error: detected multiple servers with the same port");

	for (size_t i = 0 ; i < this->children.size(); i++)
	{
		Server *current_server =  reinterpret_cast<Server*>(this->children[i]);
		this->server_names[current_server] = this->properties.server_names;
		if (current_server->socket.startListening(*current_server))
		{
			this->ioset[SET_READ][current_server->socket] = SET;
			this->servers[current_server->socket] = current_server;
		}
	}
	if (this->servers.empty())
		throw std::runtime_error("Error: All of the specified servers failed to initialize");
}

//-----------------------------------------------Destructor-----------------------------------------------

WebServer::~WebServer()
{
	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
		delete it->second;
	this->clients.clear();
	this->servers.clear();
}

//-----------------------------------------------Util-----------------------------------------------

void	WebServer::deleteClient(int fd)
{
	if (!this->clients.count(fd))
		throw std::runtime_error("Error: Could not delete client, not in 'clients'");
	delete this->clients[fd];
	this->clients.erase(fd);
	ioset[SET_READ][fd] = CLEAR;
	ioset[SET_WRITE][fd] = CLEAR;
}

void	WebServer::addNewClients()
{
	int					client_fd;
	Client				*new_client;

	for (std::map<int, Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		Server*	server = it->second;
		if (!activity[SET_READ][server->socket])
			continue ;
		new_client = new Client(server);
		client_fd = new_client->socket;
		this->clients[client_fd] = new_client;
		ioset[SET_READ][client_fd] = SET;
	}
}

void	WebServer::closeSignal(int status)
{
	std::cout << "Received stop signal" << std::endl;
	exit(status);
}

void	WebServer::readRequests(std::queue<int>& closed_clients)
{
	int fd;
	int	ret;

	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		fd = it->first;
		Client*	client = it->second;
		if (!activity[SET_READ][fd])
			continue ;
		ret = client->receive(this->server_names);
		if (ret == -1)
			closed_clients.push(fd);
		else if (ret)
		{
			ioset[SET_READ][fd] = CLEAR;
			ioset[SET_WRITE][fd] = SET;
		}
	}
}

void	WebServer::writeResponses(std::queue<int>& closed_clients)
{
	int fd;

	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		fd = it->first;
		if (!activity[SET_WRITE][fd])
			continue ;
		Client* client = it->second;
		int		ret = client->send();
		if (ret == -1)
			closed_clients.push(fd);
		else if (ret)
		{
			ioset[SET_WRITE][fd] = CLEAR;
			ioset[SET_READ][fd] = SET;
		}
	}
}

void	WebServer::run()
{
	std::queue<int>		closed_clients;

	signal(SIGINT, WebServer::closeSignal);
	signal(SIGPIPE, Response::setSigpipe);

	while (1)
	{
		activity = ioset;
		if (activity.select() == -1)
			throw std::runtime_error("Error: select() returned an error");
		this->addNewClients();
		this->writeResponses(closed_clients);
		this->readRequests(closed_clients);
		while (closed_clients.size())
		{
			this->deleteClient(closed_clients.front());
			closed_clients.pop();
		}
	}
}

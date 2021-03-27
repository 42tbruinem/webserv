/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 16:00:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 22:30:26 by tbruinem      ########   odam.nl         */
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

CleanExit::CleanExit(const char *what_arg, int exit_code) : std::runtime_error(what_arg), exit_code(exit_code) _GLIBCXX_TXN_SAFE{}
CleanExit::~CleanExit() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {}

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
<<<<<<< HEAD
=======
requests(),
responses(),
>>>>>>> main
ioset(),
activity()
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
		if (current_server->init())
		{
			this->ioset[SET_READ][current_server->fd] = SET;
			this->servers[current_server->fd] = current_server;
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
		if (!activity[SET_READ][server->fd])
			continue ;
		new_client = new Client(server);
		client_fd = new_client->getFd();
		this->clients[client_fd] = new_client;
		ioset[SET_READ][client_fd] = SET;
	}
}

WebServer*	this_copy;
void	WebServer::closeSignal(int status)
{
	std::cout << "Received stop signal" << std::endl;

	for (std::map<int, Client*>::iterator it = this_copy->clients.begin(); it != this_copy->clients.end(); it++)
		delete it->second;
	this_copy->clients.clear();
	this_copy->servers.clear();

	throw CleanExit("Server stopped cleanly", status);
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
		ret = client->receive();
		if (ret == -1)
			closed_clients.push(fd);
		else if (ret)
		{
			// responses[fd].push(Response());
			// Response &current_response = responses[fd].back();
			// current_response.setRequest(requests[fd].front());
			// current_response.locationMatch(this->server_names);
			// current_response.composeResponse();
			// requests[fd].pop();
			// if (requests[fd].empty())
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
		// Response& current_response = responses[fd].front();
		// current_response.sendResponse(fd);
		// if (current_response.getFinished())
		// {
		// 	if (current_response.getStatusCode() != 400)
		// 		std::cout << "[" << current_response.getStatusCode() << "] Response send!" << std::endl;
		// 	if (current_response.getStatusCode() == 400 || current_response.getStatusCode() == 505)
		// 	{
		// 		closed_clients.push(fd);
		// 		continue ;
		// 	}
		// 	responses[fd].pop();
		// 	if (responses[fd].empty())
		// 	{
		// 		ioset[SET_WRITE][fd] = CLEAR;
		// 		ioset[SET_READ][fd] = SET;
		// 	}
		// }
	}
}

void	WebServer::run()
{
	std::queue<int>		closed_clients;

	this_copy = this;
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
		for (size_t i = 0; i < closed_clients.size(); i++)
		{
			this->deleteClient(closed_clients.front());
			closed_clients.pop();
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 16:00:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 18:38:44 by tbruinem      ########   odam.nl         */
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

//Constructors

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

WebServer::WebServer(char *config_path) : Context(), servers(), clients()
{
	this->keywords.push_back("server");

	FD_ZERO(&this->read_sockets);
	FD_ZERO(&this->write_sockets);
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
			FD_SET(current_server->server_fd, &this->read_sockets);
			this->servers[current_server->server_fd] = current_server;
		}
	}
	if (this->servers.empty())
		throw std::runtime_error("Error: All of the specified servers failed to initialize");
}

//Destructor

WebServer::~WebServer()
{
	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
		delete it->second;
	this->clients.clear();
	this->servers.clear();
}

//Util

void	WebServer::deleteClient(int fd)
{
	if (!this->clients.count(fd))
		throw std::runtime_error("Error: Could not delete client, not in 'clients'");
	delete this->clients[fd];
	this->clients.erase(fd);
	FD_CLR(fd, &this->read_sockets);
	FD_CLR(fd, &this->write_sockets);
}

void	WebServer::addNewClients(fd_set& read_set)
{
	int					client_fd;
	Client				*new_client;

	for (std::map<int, Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		Server*	server = it->second;
		if (!FD_ISSET(server->server_fd, &read_set))
			continue ;
		new_client = new Client(server);
		client_fd = new_client->getFd();
		this->clients[client_fd] = new_client;
		FD_SET(client_fd, &this->read_sockets);
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

	FD_ZERO(&this_copy->read_sockets);
	FD_ZERO(&this_copy->write_sockets);

	std::cout << "Server stopped cleanly" << std::endl;

	exit(status);
}

void	WebServer::readRequests(fd_set& read_set, std::vector<int>& closed_clients)
{
	int fd;

	//probably need to actually use this, the response might fail to send, causing us to have to close the client.
	(void)closed_clients;
	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		fd = it->first;
		if (FD_ISSET(fd, &read_set))
		{
			if (!requests[fd].size())
				requests[fd].push(Request());
			Request &current_request = requests[fd].front();
			current_request.process(fd);
			if (current_request.getDone())
			{
				responses[fd].push(Response());
				Response &current_response = responses[fd].back();
				current_response.setRequest(requests[fd].front());
				current_response.locationMatch(this->server_names);
				current_response.composeResponse();
				requests[fd].pop();
				if (requests[fd].empty())
					FD_CLR(fd, &this->read_sockets);
				FD_SET(fd, &this->write_sockets);
			}
		}
	}
}

void	WebServer::writeResponses(fd_set& write_set, std::vector<int>& closed_clients)
{
	int fd;

	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		fd = it->first;
		if (FD_ISSET(fd, &write_set))
		{
			Response& current_response = responses[fd].front();
			current_response.sendResponse(fd);
			if (current_response.getFinished())
			{
				if (current_response.getStatusCode() != 400)
					std::cout << "[" << current_response.getStatusCode() << "] Response send!" << std::endl;
				if (current_response.getStatusCode() == 400 || current_response.getStatusCode() == 505)
					closed_clients.push_back(fd);
				responses[fd].pop();
				if (responses[fd].empty())
				{
					FD_CLR(fd, &this->write_sockets);
					FD_SET(fd, &this->read_sockets);
				}
			}
			else
				break;
		}
	}
}

void	WebServer::run()
{
	std::vector<int>	closed_clients;
	fd_set				read_set;
	fd_set				write_set;

	this_copy = this;
	signal(SIGINT, WebServer::closeSignal);
	signal(SIGPIPE, Response::setSigpipe);

	while (1)
	{
		closed_clients.clear();
		size_t		max_fd = ft::max(ft::maxElement(this->servers), ft::maxElement(this->clients)) + 1;
		read_set = this->read_sockets;
		write_set = this->write_sockets;
		if (select(max_fd, &read_set, &write_set, NULL, NULL) == -1)
			throw std::runtime_error("Error: select() returned an error");
		this->addNewClients(read_set);
		this->writeResponses(write_set, closed_clients);
		this->readRequests(read_set, closed_clients);
		for (size_t i = 0; i < closed_clients.size(); i++)
			this->deleteClient(closed_clients[i]);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/05 15:21:16 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 16:00:16 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"
#include "Utilities.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string>
#include <exception>

Socket::Socket()
{
	int opt = 1;

	this->fd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->fd == -1)
		throw std::runtime_error("Error: Creation of socket failed");

	// Set additional options for the socket and the socket type
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Error: Failed to set socket options");

	//Set the resulting socketfd to be non blocking
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set server-socket to O_NONBLOCK");
}

Socket::Socket(Server *server)
{
	memset(&this->address, '\0', sizeof(this->address));
	this->size = sizeof(this->address);
	this->fd = accept(server->socket, (struct sockaddr*)&this->address, &this->size);
	if (this->fd == -1)
		throw std::runtime_error("Error: failed to open a new client connection");
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set client-socket to O_NONBLOCK");
}

Socket::operator ssize_t() const
{
	return (this->fd);
}

Socket::~Socket()
{
	if (this->fd != -1 && close(this->fd) == -1)
		throw std::runtime_error("Error: failed to close socket fd");
}

bool	Socket::startListening(Server& server)
{
	const std::pair<std::string, std::string>& ip_port = server.getProperties().ip_port;
	
	// Assign transport address
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = (ip_port.first == "0.0.0.0") ? INADDR_ANY : inet_addr(ip_port.first.c_str());
	this->address.sin_port = htons(ft::stoi(ip_port.second));
	memset(this->address.sin_zero, '\0', sizeof(this->address.sin_zero));
	if (bind(this->fd, reinterpret_cast<struct sockaddr*>(&this->address), sizeof(this->address)) == -1)
		return (false);
	if (listen(this->fd, MAX_CONNECTIONS) == -1)
		throw std::runtime_error("Error: could not set server-socket to listening mode");
	std::cout << "Server created!" << std::endl << std::endl;
	return (true);
}

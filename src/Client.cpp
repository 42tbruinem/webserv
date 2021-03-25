/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 17:36:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 18:50:48 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <exception>
#include "Response.hpp"

Client::Client(Server* server) : server(server)
{
	ft::memset(&this->address, '\0', sizeof(this->address));
	this->addr_len = sizeof(this->address);
	this->fd = accept(server->server_fd, (struct sockaddr*)&this->address, &this->addr_len);
	if (this->fd == -1)
		throw std::runtime_error("Error: failed to open a new client connection");
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set client-socket to O_NONBLOCK");
}

int		Client::getFd()
{
	return (this->fd);
}

// Client::Client(const Client& other) : server(other.server), address(other.address), addr_len(other.addr_len), fd(other.fd) {}

Client::~Client()
{
	//std::cout << "CLIENT DESTRUCTOR CALLED" << std::endl;
	close(this->fd);
}

// Client&	Client::operator=(const Client& other)
// {
// 	if (this != &other)
// 	{
// 		this->server = other.server;
// 		this->address = other.address;
// 		this->addr_len = other.addr_len;
// 		this->req = other.req;
// 		this->fd = other.fd;
// 	}

// 	return *this;
// }

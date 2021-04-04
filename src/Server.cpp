/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 16:21:50 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/04/04 14:42:29 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <exception>
#include <vector>
#include <arpa/inet.h>

#include "Server.hpp"
#include "Utilities.hpp"
#include "Location.hpp"
#include "Context.hpp"

//Constructors

#define IP4_LOCALHOST_ADDRESS "127.0.0.1"

Server::Server(Context& parent) : Context(parent)
{
	this->keywords.push_back("location");
	this->keywords.push_back("listen");
	this->keywords.push_back("server_name");
	this->keywords.push_back("error_page");
	this->keywords.push_back("client_max_body_size");
	this->keywords.push_back("cgi_param");
	this->keywords.push_back("auth_basic");
	this->keywords.push_back("auth_basic_user_file");
}

//Destructor

Server::~Server()
{
	close(this->fd);
}

//Utils

bool	Server::init()
{
	int 	opt = 1;

	// Create socket file descriptor
	this->fd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->fd == -1)
		throw std::runtime_error("Error: Creation of socket failed");

	// Set additional options for the socket and the socket type
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Error: Failed to set socket options");

	// Assign transport address
	this->address.sin_family = AF_INET;
	if (this->properties.ip_port.first == "localhost")
		this->properties.ip_port.first = IP4_LOCALHOST_ADDRESS;
	this->address.sin_addr.s_addr = (this->properties.ip_port.first == "0.0.0.0") ? INADDR_ANY : inet_addr(this->properties.ip_port.first.c_str());
	this->address.sin_port = htons(ft::stoi(this->properties.ip_port.second));
	ft::memset(this->address.sin_zero, '\0', sizeof(this->address.sin_zero));

	// Attach socket to transport address
	if (bind(this->fd, reinterpret_cast<struct sockaddr*>(&this->address), sizeof( this->address )) == -1)
		return false;

	if (listen(this->fd, MAX_CONNECTIONS) == -1)
		throw std::runtime_error("Error: could not set server-socket to listening mode");
	std::cout << "Server created!" << std::endl << std::endl;

	//Set the resulting socketfd to be non blocking
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set server-socket to O_NONBLOCK");

	return true;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 17:36:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/03 15:53:31 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <queue>
#include <exception>
#include "Response.hpp"

Client::Client(Server* server) : server(server), current_request()
{
	ft::memset(&this->address, '\0', sizeof(this->address));
	this->addr_len = sizeof(this->address);
	this->fd = accept(server->fd, (struct sockaddr*)&this->address, &this->addr_len);
	if (this->fd == -1)
		throw std::runtime_error("Error: failed to open a new client connection");
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set client-socket to O_NONBLOCK");
}

//read once, split into requests, save remainder into remainder
//return false if error occurred
bool	Client::createRequests(void)
{
	char		buffer[PIPE_CAPACITY_MAX + 1];
	ssize_t		ret;
	std::string	bytes;

	//read new portion of request
	ret = read(this->fd, buffer, PIPE_CAPACITY_MAX);
	if (ret == -1)
		return (false);
	buffer[ret] = '\0';
	bytes = std::string((char*)buffer, ret);
//	std::cerr << "REQUEST HAS A SIZE OF " << bytes.size() << " SO FAR" << std::endl;
	// if (bytes.size() > 10 * MB)
	// 	(void)ret;
//	std::cerr << ft::rawString(bytes) << std::endl;

	for (; bytes.size();)
	{
		int found;

		found = current_request.findEndOfRequest(bytes);
		if (found == -1)
			return (false);
		if (found)
		{
			requests.push(current_request);
			current_request = Request();
		}
		else
			break ;
	}
	return (true);
}

//return:
// -1	=	error
//  0	=	no request(s) finished
//  1> 	=	amount of requests finished
int		Client::receive(const std::map<Server*, std::vector<std::string> >& server_names)
{
	int	requests_finished = 0;

	if (!createRequests())
		return (-1);
	for (; this->requests.size();)
	{
		if (!this->requests.front().process())
			return (-1);
		this->requests.front().printRequest();
		this->responses.push(Response(this->requests.front()));
		this->responses.back().setRequest(this->requests.front());
		this->responses.back().locationMatch(server_names);
		this->responses.back().composeResponse();
		this->requests.pop();
		requests_finished++;
	}
	return (requests_finished);
}

int		Client::send()
{
	for (; this->responses.size(); )
	{
		Response& current_response = responses.front();
		current_response.sendResponse(fd);
		if (current_response.getFinished())
		{
			if (current_response.getStatusCode() != 400)
				std::cout << "[" << current_response.getStatusCode() << "] Response send!" << std::endl;
			responses.pop();
		}
		else
			break ;
	}
	return (1);
}

int		Client::getFd()
{
	return (this->fd);
}

Client::~Client()
{
	close(this->fd);
}

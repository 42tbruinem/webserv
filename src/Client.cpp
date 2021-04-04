/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 17:36:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 12:08:22 by tbruinem      ########   odam.nl         */
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

bool	Client::createRequests(void)
{
	char		buffer[PIPE_CAPACITY_MAX + 1];
	ssize_t		ret;
	std::string	bytes;

	ret = read(this->fd, buffer, PIPE_CAPACITY_MAX);
	if (ret == -1 && g_sigpipe)
		return (false);
	if (ret == -1)
		throw std::runtime_error("Error: failed to read from client");
	buffer[ret] = '\0';
	bytes = std::string((char*)buffer, ret);

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
	int responses_send = 0;

	for (; this->responses.size(); )
	{
		Response& current_response = responses.front();
		if (!current_response.sendResponse(fd))
			return (-1);
		if (!current_response.getFinished())
			break ;
		if (current_response.getStatusCode() != 400)
			std::cout << "[" << current_response.getStatusCode() << "] Response send!" << std::endl;
		responses.pop();
		responses_send++;
	}
	return (responses_send);
}

int		Client::getFd()
{
	return (this->fd);
}

Client::~Client()
{
	close(this->fd);
}

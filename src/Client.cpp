/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 17:36:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/28 13:12:39 by tbruinem      ########   odam.nl         */
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

Client::Client(Server* server) : server(server)
{
	ft::memset(&this->address, '\0', sizeof(this->address));
	this->addr_len = sizeof(this->address);
	this->fd = accept(server->fd, (struct sockaddr*)&this->address, &this->addr_len);
	if (this->fd == -1)
		throw std::runtime_error("Error: failed to open a new client connection");
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Could not set client-socket to O_NONBLOCK");
}

bool	containsHeader(std::string& bytes, std::string header_key, std::string& header_field)
{
	size_t	eol;
	size_t	header;

	header = bytes.find(header_key + std::string(": "));
	if (header == std::string::npos)
		return (false);
	eol = bytes.find("\r\n", header);
	if (eol == std::string::npos)
		return (false);
	header_field = bytes.substr(header, eol - header);
	return (true);
}

bool	bodyEnd(std::string bytes, ssize_t& end_of_request, bool& encoding)
{
	std::string	header_field;
	std::string	headers;

	headers = bytes.substr(0, end_of_request + 2);
	end_of_request += 4;
	if (containsHeader(headers, "Transfer-Encoding", header_field))
	{
		std::pair<std::string, std::string>	keyval;
		std::vector<std::string>			value;

		keyval = ft::getKeyval(header_field);
		value = ft::split(keyval.second, " ");
		if (value.empty() || value[value.size() - 1] != "chunked")
		{
			end_of_request = -1;
			return (true);
		}
		encoding = true;
		end_of_request = bytes.find("0\r\n\r\n", end_of_request);
		if ((size_t)end_of_request == std::string::npos)
			return (false);
		end_of_request += 5;
		return (true);
	}
	//need to check for duplicate Content-Length headerfields
	else if (containsHeader(headers, "Content-Length", header_field))
	{
		std::pair<std::string, std::string>	keyval;
		std::vector<std::string>			value;

		keyval = ft::getKeyval(header_field);
		value = ft::split(keyval.second, " ");
		ft::printIteration(value.begin(), value.end());
		if (value.size() != 1 || !ft::onlyConsistsOf(value[0], "0123456789"))
		{
			end_of_request = -1;
			return (true);
		}
		end_of_request += ft::stoul(value[0]);
		if ((size_t)end_of_request > bytes.size())
			return (false);
		return (true);
	}
	else
		return (true);
}

//read once, split into requests, save remainder into remainder
//return false if error occurred
bool	readRequests(int fd, std::string& remainder, std::queue<Request>& requests)
{
	char		buffer[PIPE_CAPACITY_MAX + 1];
	ssize_t		ret;
	std::string	bytes;

	ret = read(fd, buffer, PIPE_CAPACITY_MAX);
	if (ret == -1)
		return (false);
	bytes.reserve((size_t)ret + remainder.size());
	buffer[ret] = '\0';
	bytes = remainder + std::string((char*)buffer, ret);
	std::cerr << ft::rawString(bytes) << std::endl;

	size_t	i = 0;
	for (;i < bytes.size();)
	{
		bool		encoding = false;
		ssize_t		end_of_request;
		std::string	request;

		end_of_request = bytes.find("\r\n\r\n", i);
		if ((size_t)end_of_request == std::string::npos ||
			!bodyEnd(bytes.substr(i, bytes.size()), end_of_request, encoding))
			break ;
		if (end_of_request == -1)
		{
//			std::cerr << "Can't determine body end yet!\n";
			return (false);
		}
		request = bytes.substr(i, (end_of_request - i));
		requests.push(Request(request, encoding));
		i = end_of_request;
	}
	remainder = bytes.substr(i, ret + remainder.size());
	return (true);
}

//read once, as much as we can
//split it into requests
//process all of the finished requests (if more than one)
int		Client::receive(const std::map<Server*, std::vector<std::string> >& server_names)
{
	if (!readRequests(this->fd, this->remainder, this->requests))
		return (-1);
	if (requests.empty())
		return (0);
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
	}
	return (1);
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

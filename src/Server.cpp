/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 16:21:50 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/04/05 16:01:43 by tbruinem      ########   odam.nl         */
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

Server::Server(Context& parent) : Context(parent), socket()
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

Server::~Server() {}

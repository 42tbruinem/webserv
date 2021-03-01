/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Context.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/06 09:33:44 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/02/06 16:23:16 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "Context.hpp"
#include "Properties.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "WebServer.hpp"
#include "Utilities.hpp"

//Parent Context
Context::Context() : parent(*this), properties() {}

//Child Context
Context::Context(Context& parent) : parent(parent), properties(parent.properties) {}

Context *Context::key_server(const std::list<std::string>& args)
{
	std::cout << "CREATE_SERVER CALLED" << std::endl;
	if (args.size())
		throw ft::runtime_error("Error: Arguments provided to 'server'");
	Server *elem = new Server(*this);
	this->children.push_back(elem);
	return (elem);
}

const Properties&	Context::get_properties()
{
	return (this->properties);
}

Context	*Context::key_location(const std::list<std::string>& args)
{
	std::cout << "CREATE_LOCATION CALLED" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: Not the right amount of arguments given to 'location'");
	Location *elem = new Location(*this, args.back());
	Server& parent = dynamic_cast<Server&>(*this);
	std::string path = args.back();
	parent.locations[path] = elem;
	this->children.push_back(elem);
	return (elem);
}

Context *Context::key_listen(const std::list<std::string>& args)
{
	std::cout << "LISTEN" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: Invalid amount of arguments given to 'listen'");
	std::string ip_port = args.front();
	if (ip_port.find(':') != std::string::npos)
	{
		std::pair<std::string, std::string>	keyval = ft::get_keyval(ip_port, ":");
		for (size_t i = 0; i < keyval.second.size(); i++)
			if (keyval.second[i] < '0' || keyval.second[i] > '9')
				throw ft::runtime_error("Error: Invalid port given to 'listen'");
		this->properties.ip_port = keyval;
	}
	else
	{
		bool port = true;
		for (size_t i = 0; i < ip_port.size() && port; i++)
			if (ip_port[i] < '0' || ip_port[i] > '9')
				port = false;
		if (port)
			this->properties.ip_port.second = ip_port;
		else
			this->properties.ip_port.first = ip_port;
	}
	return (NULL);
}

Context *Context::key_server_name(const std::list<std::string>& args)
{
	std::cout << "SERVER_NAME" << std::endl;
	if (!args.size())
		throw ft::runtime_error("Error: No arguments given to 'server_name'");
	for (std::list<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
		this->properties.server_names.push_back(*it);
	return (NULL);
}

Context *Context::key_client_max_body_size(const std::list<std::string>& args)
{
	std::cout << "MAX_CLIENT_BODY_SIZE" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: Invalid amount of arguments given to 'client_max_body_size'");
	std::string value = args.front();
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!isalnum(value[i]))
			throw std::runtime_error("Error: Invalid value provided to 'client_max_body_size'");
		value[i] = tolower(value[i]);
	}
	std::vector<std::string>	split_value = ft::split(value, "abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz");
	if (split_value.size() == 2)
	{
		if ((split_value[0][0] < '0' || split_value[0][0] > '9') || split_value[1].size() != 1)
			throw ft::runtime_error("Error: invalid value provided to 'client_max_body_size'");
	}
	else if (split_value.size() != 1)
		throw ft::runtime_error("Error: invalid value provided to 'client_max_body_size'");
	size_t body_size = ft::stoul(split_value[0]);
	
	if (split_value.size() == 2)
	{
		switch(split_value[1][0])
		{
			case 'k':
				body_size *= 1000; break ;
			case 'm':
				body_size *= 1000000; break ;
			default:
				throw ft::runtime_error("Error: invalid value provided to 'client_max_body_size'");
		}
	}
	this->properties.client_max_body_size = body_size;
	return (NULL);
}

Context *Context::key_autoindex(const std::list<std::string>& args)
{
	std::cout << "AUTOINDEX" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: No arguments given to 'autoindex'");
	std::string value = args.front();
	if (value != "off" && value != "on")
		throw ft::runtime_error("Error: Invalid value provided to 'autoindex");
	this->properties.auto_index = (value == "on");
	return (NULL);
}

//might want to add a check to see if it's a valid path..
Context *Context::key_index(const std::list<std::string>& args)
{
	std::cout << "INDEX" << std::endl;
	if (!args.size())
		throw ft::runtime_error("Error: No arguments given to 'index'");
	for (std::list<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
		this->properties.index.push_back(*it);
	return (NULL);
}

Context *Context::key_error_page(const std::list<std::string>& args)
{
	std::cout << "ERROR_PAGE" << std::endl;
	if (args.size() < 2)
		throw ft::runtime_error("Error: No arguments given to 'error_page'");
	std::list<std::string>::const_iterator end_of_statuscodes = args.end();
	--end_of_statuscodes;
	std::vector<int>	status_codes;
	std::list<std::string>::const_iterator it = args.begin();
	for (; it != end_of_statuscodes; it++)
		status_codes.push_back(ft::stoi(*it));
	for (size_t i = 0; i < status_codes.size(); i++)
		this->properties.error_pages[status_codes[i]] = *it;
	return (NULL);
}

Context *Context::key_limit_except(const std::list<std::string>& args)
{
	std::cout << "LIMIT_EXCEPT" << std::endl;
	if (!args.size())
		throw ft::runtime_error("Error: No arguments given to 'limit_except'");
	for (std::map<std::string, bool>::iterator it = this->properties.accepted_methods.begin(); it != this->properties.accepted_methods.end(); it++)
		it->second = false;
	for (std::list<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
		this->properties.accepted_methods[Method(*it).get_str()] = true;
	return (NULL);
}

Context* Context::key_cgi_param(const std::list<std::string>& args)
{
	std::cout << "CGI_PARAM" << std::endl;

	if (args.size() != 2)
		throw ft::runtime_error("Error: Invalid amount of arguments given to 'cgi_param'");
	if (!ft::onlyConsistsOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ_"))
		throw ft::runtime_error("Error: PARAM name does not consist of only upper_case");
	std::string key = args.front();
	std::string value = args.back();
	this->properties.cgi_param[key] = value;
	return (NULL);
}

//Might want to make sure that it's actually a path
Context *Context::key_root(const std::list<std::string>& args)
{
	std::cout << "ROOT" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: No arguments given to 'root'");
	this->properties.root = args.front();
	return (NULL);
}

Context *Context::key_php_cgi(const std::list<std::string>& args)
{
	std::cout << "PHP_CGI" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: Wrong amount of arguments given to 'php-cgi'");
	this->properties.php_cgi = args.front();
	return (NULL);
}

Context *Context::key_auth_basic(const std::list<std::string>& args)
{
	std::cout << "AUTH_BASIC" << std::endl;
	if (!args.size())
		throw ft::runtime_error("Error: No arguments provided to 'auth_basic'");
	this->properties.auth.enabled = true;
	std::string argument;
	for (std::list<std::string>::const_iterator it = args.begin(); it != args.end();)
		argument += *it + ((++it == args.end()) ? "" : " ");
	argument = ft::removeSet(argument, "\"\'");
	this->properties.auth.realm = argument;
	std::cout << "REALM: " << argument << std::endl;
	if (argument == "off")
		this->properties.auth.enabled = false;
	return (NULL);
}

bool	parse_htpasswd(std::map<std::string, std::string>& user_pass, std::string user_file)
{
	user_pass.clear();
	int fd = open(user_file.c_str(), O_RDONLY);
	if (fd == -1)
		return (false);
	bool valid = true;
	std::vector<std::string>	lines = ft::get_lines(fd);
	for (size_t i = 0; i < lines.size() && valid; i++)
	{
		if (!lines[i].size() || lines[i][0] == '#')
			continue ;
		if (lines[i].find(':') == std::string::npos)
			valid = false;
		user_pass.insert(ft::get_keyval(lines[i], ":"));
		// std::pair<std::string, std::string>	keyval = ft::get_keyval(lines[i], ":");
		// std::cout << "USER: " << keyval.first << " | PASS: " << keyval.second << std::endl;
	}
	close(fd);
	return (valid);
}

Context *Context::key_auth_basic_user_file(const std::list<std::string>& args)
{
	std::cout << "AUTH_BASIC_USER_FILE" << std::endl;
	if (args.size() != 1)
		throw ft::runtime_error("Error: Invalid amount of arguments given to 'auth_basic_user_file'");
	if (!this->properties.auth.enabled)
		throw ft::runtime_error("Error: 'auth_basic_user_file' provided for disabled 'auth_basic' authentication");
	if (!parse_htpasswd(this->properties.auth.user_pass, args.front()))
		throw ft::runtime_error("Error: provided file either doesn't exist or does not conform to the htpasswd format");
	return (NULL);
}

Context	*Context::parse_keyword(std::string key, std::list<std::string> args)
{
	const static std::pair<std::string, Context::keyword_func>	pairs[] = {
		std::pair<std::string, Context::keyword_func>("server", &Context::key_server),
		std::pair<std::string, Context::keyword_func>("listen", &Context::key_listen),
		std::pair<std::string, Context::keyword_func>("location", &Context::key_location),
		std::pair<std::string, Context::keyword_func>("server_name", &Context::key_server_name),
		std::pair<std::string, Context::keyword_func>("client_max_body_size", &Context::key_client_max_body_size),
		std::pair<std::string, Context::keyword_func>("autoindex", &Context::key_autoindex),
		std::pair<std::string, Context::keyword_func>("index", &Context::key_index),
		std::pair<std::string, Context::keyword_func>("limit_except", &Context::key_limit_except),
		std::pair<std::string, Context::keyword_func>("root", &Context::key_root),
		std::pair<std::string, Context::keyword_func>("error_page", &Context::key_error_page),
		std::pair<std::string, Context::keyword_func>("php-cgi", &Context::key_php_cgi),
		std::pair<std::string, Context::keyword_func>("cgi_param", &Context::key_cgi_param),
		std::pair<std::string, Context::keyword_func>("auth_basic", &Context::key_auth_basic),
		std::pair<std::string, Context::keyword_func>("auth_basic_user_file", &Context::key_auth_basic_user_file),
	};
	static std::map<std::string, Context::keyword_func>	functions(pairs, pairs + (sizeof(pairs) / sizeof(std::pair<std::string, Context::keyword_func>)));

	if (!functions.count(key))
		throw ft::runtime_error("Error: unrecognized keyword in config-parser");
	return ((this->*(functions[key]))(args));
}

Context::~Context()
{
	std::cout << "CONTEXT DECONSTRUCTOR!!!" << std::endl;
	for (size_t i = 0; i < children.size(); i++)
	{
		if (dynamic_cast<Server *>(children[i]))
			delete dynamic_cast<Server*>(children[i]);
		else if (dynamic_cast<Location *>(children[i]))
			delete dynamic_cast<Location*>(children[i]);
		else
			throw ft::runtime_error("Error: Unrecognized parser-Context type");
	}
	children.clear();
}

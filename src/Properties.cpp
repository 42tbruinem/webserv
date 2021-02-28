/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Properties.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/06 09:37:47 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/02/28 16:50:19 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "Properties.hpp"
# include "Method.hpp"
# include "Utilities.hpp"

bool	Authorization::operator () (std::string userpass) const
{
	if (this->enabled == false)
		return (true);
	userpass = ft::base64decode(userpass);
	std::pair<std::string, std::string>	user_and_pass = ft::get_keyval(userpass);
	std::map<std::string, std::string>	user_list = this->user_pass;
	return (!user_list.size() || (user_list.count(user_and_pass.first) && user_list[user_and_pass.first] == user_and_pass.second));
}

Authorization&	Authorization::operator = (const Authorization& other)
{
	if (this != &other)
	{
		this->enabled = other.enabled;
		this->user_pass = other.user_pass;
		this->realm = other.realm;
	}
	return (*this);
}

Authorization::Authorization() : enabled(false), realm(""), user_pass() {}

Authorization::Authorization(const Authorization& other)
{
	*this = other;
}

Authorization::~Authorization() {}

//----------------------------------------------------------------------------------------------

Properties::Properties() :
	root(""),
	ip_port(std::pair<std::string, std::string>("0.0.0.0", "80")),
	server_names(), index(), auto_index(false),
	accepted_methods(), error_pages(), client_max_body_size(1000000),
	php_cgi(""), cgi_param()
{
	auth.enabled = false;
	for (size_t i = 0; i < E_METHOD_END; i++)
		accepted_methods[Method((e_method)i).get_str()] = true;
	this->index.push_back("index.html");
}

Properties::Properties(const Properties& other) :
	root(other.root), ip_port(other.ip_port),
	server_names(other.server_names),
	index(other.index), auto_index(other.auto_index),
	accepted_methods(other.accepted_methods),
	error_pages(other.error_pages), client_max_body_size(other.client_max_body_size), php_cgi(other.php_cgi),
	cgi_param(other.cgi_param) {}

Properties& Properties::operator = (const Properties& other)
{
	if (this != &other)
	{
		this->root = other.root;
		this->ip_port = other.ip_port;
		this->server_names = other.server_names;
		this->index = other.index;
		this->auto_index = other.auto_index;
		this->accepted_methods = other.accepted_methods;
		this->error_pages = other.error_pages;
		this->client_max_body_size = other.client_max_body_size;
		this->php_cgi = other.php_cgi;
		this->cgi_param = other.cgi_param;
		this->auth = other.auth;
	}
	return (*this);
}

Properties::~Properties() {}

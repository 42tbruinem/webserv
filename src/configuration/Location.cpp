/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/06 01:22:09 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 18:14:36 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Context.hpp"

Location::Location(Context& parent, const std::string& location) : Context(parent), location(location)
{
	this->keywords.push_back("limit_except");
	this->keywords.push_back("root");
	this->keywords.push_back("autoindex");
	this->keywords.push_back("index");
	this->keywords.push_back("error_page");
	this->keywords.push_back("php-cgi");
	this->keywords.push_back("cgi_param");
	this->keywords.push_back("auth_basic");
	this->keywords.push_back("auth_basic_user_file");
	this->keywords.push_back("client_max_body_size");
	this->keywords.push_back("ext");
}

Location::~Location() {}

std::string	Location::get_location() const
{
	return (this->location);
}

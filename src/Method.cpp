/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Method.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 20:24:09 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 16:00:38 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"
#include <exception>
#include <string>
#include <vector>

const char * const Method::names[] = 
{
	"OPTIONS",
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"TRACE",
	"CONNECT"
};

Method::Method(const Method& other) : EnumString(other) {}

Method::Method(std::string str) : EnumString(str, names, sizeof(names)) {}

Method::Method(e_method id) : EnumString(id, names, sizeof(names)) {}

Method&	Method::operator = (const Method& other)
{
	if (this != &other)
	{
		this->id = other.id;
		this->str = other.str;
	}
	return (*this);
}

Method::~Method() {}

Method::Method() : EnumString() {}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Message.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:37:38 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 16:30:43 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

Message::Message() {}

Message::Message(const Message& other) 
{

	*this = other;
}

Message& Message::operator = (const Message& other)
{
	if (this != &other)
	{
		this->body = other.body;
		this->headers = other.headers;
	}
	return (*this);
}

std::string	Message::getHeader(const std::string& key)
{
	if (this->headers.count(key))
		return (this->headers[key]);
	else
		return "";
}

std::string Message::getBodyLen()
{
	std::string			length = "";
	int 				total = 0;

	for (std::vector<std::string>::const_iterator it = this->body.begin(); it != this->body.end(); it++)
		total += (*it).size() + 2;

	if (total == 0)
		length = "0";
	else
		total -= 2;

	while (total != 0) {
		length.insert(length.begin(), static_cast<char>(total % 10 + '0'));
		total /= 10;
	}

	return length;
}

Message::~Message() {}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 18:58:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 16:18:17 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "Configuration.hpp"
#include "Utilities.hpp"

#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <exception>
#include <iostream>

Parse::Parse() {}

Parse::~Parse() {}

Parse::Parse(Context* context, std::list<std::string> tokens) : context(context), tokens(tokens) {}

Parse::Parse(const Parse& other)
{
	*this = other;
}

Parse&	Parse::operator=(const Parse& other)
{
	if (this != &other)
	{
		this->context = other.context;
		this->tokens = other.tokens;
	}

	return *this;
}

bool	Parse::collect_args(std::list<std::string>::iterator& it, std::list<std::string>::iterator end, std::list<std::string>& args)
{
	bool body = false;

	args.clear();
	for (; it != end; it++)
	{
		if (*it == "}")
			throw std::runtime_error("Error: unclosed arguments");
		if (*it == "{" || *it == ";")
		{
			body = (*it == "{");
			break ;
		}
		//std::cout << "argument: " << *it << std::endl;
		args.push_back(*it);
	}
	if (it == end)
		throw std::runtime_error("Error: unclosed arguments");
	return (body);
}

void	Parse::handle_body(std::queue<Parse>& children, Context* child, std::list<std::string>::iterator& it, std::list<std::string>& tokens)
{
	std::list<std::string>	children_tokens;

	//std::cout << "BODY ENCOUNTERED" << std::endl;
	if (!child)
		throw std::runtime_error("Error: unexpected body encountered in config-parse");
	it++;
	std::list<std::string>::iterator	end = this->endOfBlock(it, tokens.end());
	if (end == tokens.end())
		throw std::runtime_error("Error: Unclosed block in configuration");
	children_tokens.splice(children_tokens.begin(), this->tokens, it--, end);
	it++;
	children.push(Parse(child, children_tokens));
}

void	Parse::parse()
{
	std::list<std::string>	args;
	std::queue<Parse>		children;

	//std::cout << "TOKENS: ";
	//ft::print_iteration(tokens.begin(), tokens.end());
	for (std::list<std::string>::iterator it = tokens.begin(); it != tokens.end();)
	{
		//std::cout << "Key: " << *it << std::endl;
		if (find(context->keywords.begin(), context->keywords.end(), *it) != context->keywords.end())
		{
			std::string		key = *it++;
			Context			*child;
			bool body = this->collect_args(it, tokens.end(), args);
			child = context->parse_keyword(key, args);
			if (body)
				this->handle_body(children, child, it, tokens);
			else if (!body && child)
				throw std::runtime_error("Error: no body encountered for property that expects a body");
			it++;
		}
		else
			throw std::runtime_error("Error: unrecognized keyword in config-parser");
	}
	while (children.size())
	{
		children.front().parse();
		children.pop();
	}
}

std::list<std::string>::iterator	Parse::endOfBlock(std::list<std::string>::iterator start, std::list<std::string>::iterator end)
{
	size_t depth = 0;

	for (;start != end; start++)
	{
		if (*start == "}")
		{
			if (!depth)
				break ;
			depth--;
		}
		else if (*start == "{")
			depth++;
	}
	return (start);
}

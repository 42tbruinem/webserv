/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Configuration.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 18:51:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 18:56:36 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"
#include "Utilities.hpp"
#include "WebServer.hpp"
#include "Server.hpp"
#include "Parse.hpp"
#include "WebServer.hpp"

#include <fcntl.h>
#include <string>
#include <list>

Configuration::Configuration(char *config, WebServer* webserv) : webserv(webserv)
{
	if (!config)
		config = (char *)DEFAULT_CONFIG;
	this->fd = open(config, O_RDONLY);
	if (this->fd == -1 || read(this->fd, NULL, 0) == -1)
		throw std::runtime_error("Error: Failed to open config file for reading");
}

void	stripComments(std::string& config)
{
	size_t	start = std::string::npos;
	bool	entire_line = false;

	for (size_t i = 0; i < config.size();)
	{
		if (start == std::string::npos && config[i] == '#')
		{
			entire_line = (!i || config[i - 1] == '\n');
			start = i++;
		}
		else if (start != std::string::npos && config[i] == '\n')
		{
			config.erase(start, i + entire_line - start);
			i = start;
			start = std::string::npos;
		}
		else
			i++;
	}
	if (start != std::string::npos)
		config.erase(start, config.size());
}

void	populateTokens(std::list<std::string>& tokens, int fd)
{
	std::string	raw_content;

	char	*buffer[BUFFER_SIZE + 1];
	int 	bytes_read = 0;

	while (1)
	{
		raw_content += std::string((char *)buffer, bytes_read);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			throw std::runtime_error("Error: failed to read config file");
		if (!bytes_read)
			break ;
	}
	stripComments(raw_content);
	std::vector<std::string>	vec_tokens = ft::split(raw_content, "\n\t\r {};", "{};");
	tokens = std::list<std::string>(vec_tokens.begin(), vec_tokens.end());
}

void	Configuration::parse()
{
	std::list<std::string>	tokens;
	populateTokens(tokens, this->fd);
	std::list<std::string>	arguments;
	Parse(this->webserv, tokens).parse();
	std::cout << "Configuration loaded" << std::endl;
}

Configuration::~Configuration()
{
	close(this->fd);
}

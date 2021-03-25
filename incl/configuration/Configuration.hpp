/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Configuration.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:00:37 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:21:33 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

#include <vector>
#include <list>
#include <string>

# include "Parse.hpp"

class WebServer;

//Creates Server and Location contexts containing their properties
//based on the configuration file provided.
class Configuration
{
	public:
		Configuration(char *config, WebServer* webserv);
		Configuration(const Configuration & src);
		Configuration&	operator=(const Configuration & rhs);
		~Configuration();
		void	parse();
	private:
		int		fd;
		void	populateTokens(std::list<std::string>& tokens);
		WebServer*	webserv;
		Configuration();
};

#endif

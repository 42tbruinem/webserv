/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:30:08 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/25 17:43:36 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <string>
# include <list>
# include <vector>

# include "Context.hpp"
# include "Utilities.hpp"
# include "Request.hpp"
# include "Location.hpp"
# include "Utilities.hpp"

//WebServer will try to initialize the server, if that fails because the port is already in use
//the server might still boot up serving only as a resource to parse incoming requests.

class Server : public Context
{
	public:
		Server(Context& parent);
		Server(const Server &src);
		Server&	operator = (const Server &rhs);
		~Server();
		bool	init();

		std::map<std::string, Location*, ft::SizeCompare>	locations;
		int							server_fd;
	private:
		struct sockaddr_in			address;
};

#endif

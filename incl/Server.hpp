/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:30:08 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/25 18:47:44 by tbruinem      ########   odam.nl         */
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

#ifndef MAX_CONNECTIONS
# define MAX_CONNECTIONS 200
#endif
//WebServer will try to initialize the server, if that fails because the port is already in use
//the server might still boot up serving only as a resource to parse incoming requests.

class Server : public Context
{
	public:
		Server(Context& parent);
		~Server();
		bool	init();

		std::map<std::string, Location*, ft::SizeCompare>	locations;
		int							server_fd;
	private:
		Server(const Server &src);
		Server&	operator = (const Server &rhs);
		struct sockaddr_in			address;
};

#endif

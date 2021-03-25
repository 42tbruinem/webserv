/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:30:08 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/25 16:18:19 by tbruinem      ########   odam.nl         */
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

class Server : public Context
{
	public:
		Server(Context& parent);
		Server( const Server &src );
		Server&	operator=( const Server &rhs );
		virtual ~Server();
		bool	init();

		void	startListening( void );
		int		acceptNewClient();

		void	handle_args(std::list<std::string> args);

		std::map<std::string, Location*, ft::size_compare>	locations;
		int									server_fd;

	private:
		Server();
		struct sockaddr_in			address;
};

#endif

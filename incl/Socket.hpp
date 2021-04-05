/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/04 20:32:39 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 15:49:39 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <arpa/inet.h>
#include <stddef.h>
#include <string>
#include <utility>

class Server;

class Socket
{
	public:
		Socket();
		Socket(Server *server);
		~Socket();
		operator ssize_t() const;
		bool	startListening(Server& server);

	private:
		Socket& operator = (const Socket& other);
		Socket(const Socket& other);

		ssize_t				fd;
		socklen_t			size;
		struct sockaddr_in	address;
};

#endif

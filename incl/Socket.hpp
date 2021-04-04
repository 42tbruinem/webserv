/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/04 20:32:39 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 20:39:50 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <arpa/inet.h>
#include <stddef.h>

class Socket
{
	public:
		Socket();
		Socket();
		~Socket();
		Socket& operator = (const Socket& other);
		Socket(const Socket& other);

		operator ssize_t() const;
	private:
		ssize_t				fd;
		socklen_t			size;
		struct sockaddr_in	address;
};

#endif

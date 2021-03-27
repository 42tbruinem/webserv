/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 15:24:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 11:19:11 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"

class Client
{
	private:
		Server*					server;
		struct sockaddr_in		address;
		socklen_t				addr_len;
		Request					*req;
		Client();
		Client& operator = (const Client& other);
		Client(const Client& other);
		ssize_t					fd;
	public:
		int		getFd();
		Client(Server* server);
		~Client();
};

#endif

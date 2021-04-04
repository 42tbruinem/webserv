/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 15:24:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 20:30:22 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <queue>
#include <map>

//Keep track of requests and responses

class Client
{
	private:
		struct sockaddr_in			address;
		ssize_t						fd;

		Request						current_request;
		std::queue<Request>			requests;
		std::queue<Response>		responses;

		Client();
		Client& operator = (const Client& other);
		Client(const Client& other);
	public:
		bool	createRequests(void);
		int		receive(const std::map<Server*, std::vector<std::string> >& server_names);
		int		send();
		int		getFd();
		Client(Server* server);
		~Client();
};

#endif

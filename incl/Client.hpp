/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 15:24:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 16:03:39 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

#include <queue>
#include <map>

//Keep track of requests and responses, receiving and sending

class Client
{
	private:
		Request						current_request;
		std::queue<Request>			requests;
		std::queue<Response>		responses;

		Client& operator = (const Client& other);
		Client(const Client& other);
	public:
		Socket						socket;
		bool	createRequests(void);
		int		receive(const std::map<Server*, std::vector<std::string> >& server_names);
		int		send();
		Client(Server* server);
		~Client();
};

#endif

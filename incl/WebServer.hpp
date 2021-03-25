/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 14:16:49 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 19:16:11 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERVER_HPP
# define WEB_SERVER_HPP

# include "Configuration.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Context.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Properties.hpp"

# include <queue>
# include <vector>
# include <map>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <exception>

# define DEFAULT_CONFIG "./config/default.conf"

// enum ResponseCode
// {
// 	200 = OK,
// 	400 = ERROR,

// };

//Heart of the program, contains the main loop that handles all the requests/responses

class CleanExit : public std::runtime_error
{
	public:
		explicit CleanExit(const char *what_arg, int exit_code) _GLIBCXX_TXN_SAFE;
		virtual ~CleanExit() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
		int	exit_code;
	private:
		CleanExit();
		CleanExit& operator = (const CleanExit& rhs);
};

class WebServer : public Context
{
	private:
		friend class Configuration;
		friend class Context;

		std::map<int, Server*>							servers;
		std::map<int, Client*>							clients;

		std::map<int, std::queue<Request> >				requests;
		std::map<int, std::queue<Response> >			responses;

		fd_set											read_sockets;
		fd_set											write_sockets;
		std::map<Server*, std::vector<std::string> >	server_names;

		void	deleteClient(int fd);
		void	addNewClients(fd_set& read_set);
		void	readRequests(fd_set& read_set, std::vector<int>& closed_clients);
		void	writeResponses(fd_set& write_set, std::vector<int>& closed_clients);
		static void	closeSignal(int status);

		WebServer(const WebServer& other);
		WebServer& operator = (const WebServer& other);
	public:
		WebServer(char *config_path);
		void	run();
		~WebServer();
};

#endif

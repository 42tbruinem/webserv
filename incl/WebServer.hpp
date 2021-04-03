/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 14:16:49 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/29 14:24:34 by tbruinem      ########   odam.nl         */
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
# include "IOSet.hpp"
# include "Utilities.hpp"

# include <queue>
# include <vector>
# include <map>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <exception>

# define DEFAULT_CONFIG "./config/default.conf"

//Heart of the program, contains the main loop that handles all the requests/responses

class WebServer : public Context
{
	private:
		friend class Configuration;
		friend class Context;

		std::map<int, Server*>							servers;
		std::map<int, Client*>							clients;

		IOSet											ioset;
		IOSet											activity;
		std::map<Server*, std::vector<std::string> >	server_names;

		void	deleteClient(int fd);
		void	addNewClients();
		void	readRequests(std::queue<int>& closed_clients);
		void	writeResponses(std::queue<int>& closed_clients);
		static void	closeSignal(int status);

		WebServer(const WebServer& other);
		WebServer& operator = (const WebServer& other);
	public:
		WebServer(char *config_path);
		void	run();
		~WebServer();
};

#endif

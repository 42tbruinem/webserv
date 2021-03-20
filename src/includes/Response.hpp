/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:12:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/02/28 18:45:56 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include "Request.hpp"
//# include "WebServer.hpp"
# include "Location.hpp"

class Server;

class Response : public Message
{
	public:
		Response();
		Response(const Response& other);
		Response& operator = (const Response& other);
		virtual ~Response();

		static void	setSigpipe(int);
		void	sendResponse(int fd);
		void	printResponse(void) const;
		void	composeResponse(void);
		void	setRequest(Request& req);
		int		get_status_code() const;
		void	location_match(const std::map<Server*, std::vector<std::string> >& server_names);
		Server*	server_match(const std::map<Server*, std::vector<std::string> >& server_names);

	private:
		Request						req;
		std::map<int, std::string>	status_codes;
		std::string					status_line;
		std::string					path;
		int							response_code;
		std::string 				server_name;
		Location*					location_block;
		bool						isDir;
		std::string					root;
		std::string					location_key;

//		void	checkRequestBody(void);
		void	checkMethod(void);
		void	checkPath(void);
		bool	checkAuthorization(void);

		void	handlePut(void);

		void	setStatusLine(void);
		void	setServer(void);
		void	setDate(void);
		void	setContentType(void);
		void	setBody(void);
		void	setBodyError(void);
		void	listDirectory(void);
		void	parseCgiHeaders(void);
		void	setContentLen(void);
		void	setContentLang(void);
		void	setLocation(void);
		void	setModified(void);
};

#endif

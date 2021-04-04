/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:12:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 19:19:12 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include "Request.hpp"
# include "Location.hpp"
# include "StatusLine.hpp"

class Server;

class Response : public Message
{
	public:
		Response(Request& req);
		Response(const Response& other);
		Response& operator = (const Response& other);
		~Response();

		static void	setSigpipe(int);
		bool	sendResponse(int fd);
		void	composeResponse(void);
//		void	setRequest(Request& req);
		bool	getFinished(void);
		void	locationMatch(const std::map<Server*, std::vector<std::string> >& server_names);
		Server*	serverMatch(const std::map<Server*, std::vector<std::string> >& server_names);
		StatusLine					status_line;

	private:
		Request						req;
		std::string					path;
		std::string 				server_name;
		Location*					location_block;
		bool						is_dir;
		std::string					root;
		std::string					location_key;
		std::string					response;
		size_t						size;
		size_t						send;
		bool						finished;

		void	checkMethod(void);
		void	checkPath(void);
		bool	checkAuthorization(void);

		void	handlePut(void);

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

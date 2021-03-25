/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:12:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:32:14 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include "Request.hpp"
# include "Location.hpp"

class Server;

class Response : public Message
{
	public:
		Response();
		Response(const Response& other);
		Response& operator = (const Response& other);
		~Response();

		static void	setSigpipe(int);
		void	sendResponse(int fd);
		void	printResponse(void) const;
		void	composeResponse(void);
		void	setRequest(Request& req);
		int		getStatusCode() const;
		bool	getFinished(void);
		void	locationMatch(const std::map<Server*, std::vector<std::string> >& server_names);
		Server*	serverMatch(const std::map<Server*, std::vector<std::string> >& server_names);

	private:
		Request						req;
		std::map<int, std::string>	status_codes;
		std::string					status_line;
		std::string					path;
		int							response_code;
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

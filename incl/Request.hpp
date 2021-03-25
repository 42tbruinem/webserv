/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:12:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 18:41:38 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Request_HPP
# define Request_HPP

# include "Message.hpp"
# include "Method.hpp"
# include "URI.hpp"

class Request : public Message
{
	public:
		Request();
		Request(const Request& other);
		Request& operator = (const Request& other);
		~Request();

		bool 			isStatusLine(const std::string& line);
		bool			parseLine(std::string line);
		void			splitRequest(void);
		void			process(int fd);

		bool			getDone(void) const;
		std::string		getMethod(void) const;
		std::string		getPath(void) const;
		int				getStatusCode(void) const;
		std::map<std::string, std::string>& getHeaders(void);
		std::vector<std::string>&	getBody(void);
		URI							uri;

	private:
		//move this to Method
		bool						isMethod(std::string str);
		bool						done;
		std::string 				status_line;
		std::vector<std::string>	lines;
		std::string					path;
		int							status_code;
		Method						method;
		int							body_read;
		int							body_total;
		bool						body_started;
		bool						encoding;
};

#endif

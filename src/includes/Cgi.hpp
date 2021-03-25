/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Cgi.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/17 14:12:50 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/25 16:18:20 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <map>

# include "Request.hpp"

class Cgi 
{

public:
	Cgi();
	Cgi(const Cgi & src);
	Cgi & operator=(const Cgi & rhs);
	virtual ~Cgi();

	void	execute(Request *req, std::string path, std::string host, std::string port, std::string phpcgi);

private:
	void	set_env(Request *req, std::string path, std::string host, std::string port, std::string phpcgi);

	std::map<std::string, std::string>	vars;
	char								**env;
	char								*args[3];
};

#endif

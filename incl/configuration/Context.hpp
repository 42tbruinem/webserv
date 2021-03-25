/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Context.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 18:35:46 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:50:19 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include "Properties.hpp"

# include <vector>
# include <list>
# include <string>
# include <map>

//Base class for WebServer/Server/Location, contains the properties and children

class Context
{
	private:
		typedef Context *(Context::*keyword_func)(const std::list<std::string>& args);

		Context *key_server(const std::list<std::string>& args);
		Context	*key_location(const std::list<std::string>& args);
		Context	*key_listen(const std::list<std::string>& args);
		Context *key_server_name(const std::list<std::string>& args);
		Context *key_client_max_body_size(const std::list<std::string>& args);
		Context *key_root(const std::list<std::string>& args);
		Context *key_error_page(const std::list<std::string>& args);
		Context *key_limit_except(const std::list<std::string>& args);
		Context *key_index(const std::list<std::string>& args);
		Context *key_autoindex(const std::list<std::string>& args);
		Context	*key_php_cgi(const std::list<std::string>& args);
		Context	*key_cgi_param(const std::list<std::string>& args);
		Context *key_auth_basic(const std::list<std::string>& args);
		Context *key_auth_basic_user_file(const std::list<std::string>& args);
		Context *key_ext(const std::list<std::string>& args);

	protected:
		Context&					parent;
		Properties					properties;
		std::vector<Context*>		children;
	public:
		const Properties& getProperties();
		std::vector<std::string>	keywords;
		Context	*parseKeyword(std::string key, std::list<std::string> args);
		Context();
		Context(Context & src);
		Context& operator=(Context & rhs);
		virtual ~Context();
};

#endif

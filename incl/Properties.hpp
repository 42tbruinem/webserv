/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Properties.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/06 09:28:09 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:33:26 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROPERTIES_HPP
# define PROPERTIES_HPP

# include <map>
# include <vector>
# include <list>
# include <string>
# include <iostream>

struct Authorization
{
	bool								enabled;
	std::string							realm;
	std::map<std::string, std::string>	user_pass;
	bool	operator () (std::string userpass) const;
	Authorization();
	~Authorization();
	Authorization& operator = (const Authorization& other);
	Authorization(const Authorization& other);
};

struct Properties
{
	Properties();
	Properties(const Properties& other);
	Properties& operator = (const Properties& other);
	~Properties();

	std::string							root;
	std::pair<std::string, std::string>	ip_port;
	std::vector<std::string>			server_names;
	std::vector<std::string>			index;
	bool								auto_index;
	std::map<std::string, bool>			accepted_methods;
	std::map<int, std::string>			error_pages;
	size_t								client_max_body_size;
	std::string							php_cgi;
	std::map<std::string, std::string>	cgi_param;
	Authorization						auth;
	std::vector<std::string>			ext;
};

#endif

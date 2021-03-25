/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   URI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/04 17:22:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:58:18 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

# include <vector>
# include <string>

enum	e_uri
{
	URI_SCHEME,
	URI_HOST,
	URI_PORT,
	URI_PATH,
	URI_QUERY,
	URI_FRAGMENT_ID,
	URI_FULL
};

class URI
{
	public:
		friend class PartModifier;
		class PartModifier
		{
			private:
				URI& uri;
				e_uri id;
				std::string &str;
			public:
				PartModifier(URI& uri, std::string& part, e_uri id);
				void	operator () (std::string updated);	
		};

		const std::string& operator [] (e_uri id);
		PartModifier operator [] (std::string part);

		const std::string&			getUri();
		const std::string&			getScheme();
		const std::string&			getHost();
		const std::string&			getPort();
		const std::string&			getPath();
		const std::string&			getQuery();
		const std::string&			getFragment();
		std::vector<std::string>	getParts();

		void						setUri(std::string updated);
		void						setScheme(std::string updated);
		void						setHost(std::string updated);
		void						setPort(std::string updated);
		void						setPath(std::string updated);
		void						setQuery(std::string updated);
		void						setFragment(std::string updated);
		void						setParts(const std::vector<std::string>& updated);

		URI(const std::string& uri);
		URI(const URI& other);
		URI& operator = (const URI& other);
		~URI();
	private:
		void	printState();
		URI();
		std::string					str;
		std::vector<std::string>	parts;
};

#endif

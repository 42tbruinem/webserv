/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Attribute.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 18:35:46 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/02/06 13:10:21 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATTRIBUTE_HPP
# define ATTRIBUTE_HPP

# include <vector>
# include <string>
# include <map>
# include <list>
# include <stdexcept>

//# include "Scope.hpp"

//KEYWORD [ARGS] { [KEYWORD] }

class Scope;

class Attribute
{
	public:
		virtual void		handle_args(std::list<std::string> args);
		virtual Attribute& 	handle_keyword(std::string key);

		Attribute();
		Attribute(Scope& scope);
		virtual ~Attribute();
		//keywords that the attribute responds to for parsing
		std::vector<std::string>	keywords;
		Scope&						scope;
};

#endif

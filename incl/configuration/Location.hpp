/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/06 01:10:59 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:50:30 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <list>
# include <string>

# include "Utilities.hpp"
# include "Context.hpp"

class Location : public Context
{
	public:
		~Location();
		Location(Context& parent, const std::string& location);
		Location(const Location& other);
		std::string	get_location() const;
	private:
		std::string location;
		Location& operator = (const Location& other);
};

#endif

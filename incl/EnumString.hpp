/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EnumString.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 12:04:00 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:35:15 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_STRING_HPP
# define ENUM_STRING_HPP

#include <string>
#include <exception>
#include <vector>
#include <iostream>

template <class Enum>
class EnumString
{
	private:
		std::vector<std::string>	names;
	public:
		const	Enum&			getId() const
		{
			return (this->id);
		}
		const	std::string&	getStr() const
		{
			return (this->str);
		}
	protected:
		Enum			id;
		std::string		str;
		EnumString() : id(), str() {}

		EnumString(Enum id, const char *const names[], size_t len) : names(names, names + (len / sizeof(char *))), id(id), str(toString(id)) {}

		EnumString(std::string str, const char *const names[], size_t len) : names(names, names + (len / sizeof(char *))), id(toId(str)), str(str) {}

		EnumString(const EnumString& other) : names(other.names), id(other.id), str(other.str) {}

		EnumString& operator = (const EnumString& other)
		{
			if (this != &other)
			{
				this->names = other.names;
				this->id = other.id;
				this->str = other.str;
			}
			return (*this);
		}

		virtual ~EnumString() {}

		Enum			toId(std::string str)
		{
			for (size_t i = 0; i < names.size(); i++)
			{
				if (str == names[i])
					return Enum(i);
			}
			throw std::runtime_error("Error: Conversion from str to id failed in EnumString::toID()");
		}

		std::string		toString(Enum id)
		{
			if (id >= 0 && id < names.size())
				return (names[id]);
			throw std::runtime_error("Error: Conversion from id to str failed in EnumString::toString()");
		}
};

#endif

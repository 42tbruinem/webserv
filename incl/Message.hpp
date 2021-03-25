/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Message.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:08:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:34:09 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <vector>
#include <string>
#include <utility>
#include <map>

//virtual base class for Response/Request
class Message
{
	public:
		Message();
		Message(const Message& other);
		Message& operator = (const Message& other);
		virtual ~Message();

		std::string getHeader(const std::string& key);
		std::string getBodyLen();
	protected:
		std::map<std::string, std::string>					headers;
		std::vector<std::string>							body;
};

#endif

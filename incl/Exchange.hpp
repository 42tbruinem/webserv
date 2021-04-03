/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Exchange.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 19:35:42 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/28 19:55:43 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCHANGE_HPP
# define EXCHANGE_HPP

#include <queue>
#include <string>
#include <vector>

#include "Response.hpp"
#include "Request.hpp"

class Exchange
{
	private:
		struct	RequestStorage
		{
			struct	ReadBuffer
			{
				size_t		read;
				std::string	buffer;
				size_t		capacity;
				//overload += for string to add to the buffer
				//overload = for string to replace the contents of the buffer
			};
			bool			encoding;
			ReadBuffer		buffer;
			ssize_t			content_length;
			ssize_t			end_of_request;
		};

		std::queue<Response>	responses;
		std::queue<Request>		request;
	public:
		void	read();
		void	write();
};

#endif

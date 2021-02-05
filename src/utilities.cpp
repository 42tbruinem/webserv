/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utilities.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:29:21 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/02/05 18:20:31 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>

namespace ft
{
	// size_t	first_of_group(std::string raw, const std::vector<std::string>& delim_groups, size_t search_start, int& group)
	// {
	// 	size_t smallest = std::string::npos;

	// 	for (size_t i = 0; i < delim_groups.size(); i++)
	// 	{
	// 		size_t tmp = raw.find_first_of(delim_groups[i], search_start);
	// 		if (tmp < smallest)
	// 		{
	// 			group = i;
	// 			smallest = tmp;
	// 		}
	// 	}
	// 	return (smallest);
	// }

	std::vector<std::string>	split(std::string raw, std::string delim, std::string preserve_delim = "")
	{
		std::vector<std::string>	tokens;
		size_t	end;
		for (size_t begin = 0 ; begin < raw.size();)
		{
			end = raw.find_first_of(delim, begin);
//			std::cout << end << " char: " << ((end != std::string::npos) ? std::string(1, raw[end]) : std::string(" END")) << std::endl;
			if (end == std::string::npos)
				end = raw.size();
			if (begin != end)
				tokens.push_back(raw.substr(begin, end - begin));
			if (preserve_delim.size() && end < raw.size() && preserve_delim.find(raw[end]) != std::string::npos)
				tokens.push_back(std::string(1, raw[end]));
			for (end = end + 1; end < raw.size() && delim.find(raw[end]) != std::string::npos && (preserve_delim.size() == 0 || preserve_delim.find(raw[end]) == std::string::npos) ; end++) {}
			begin = end;
		}
		for (size_t i = 0; i < tokens.size(); i++)
		{
			std::cout << "'" << ((tokens[i] == "\n") ? "\\n" : tokens[i]) << "'" << std::endl;
		}
		return (tokens);
	}

	void	put_error(const std::string &str) {

		std::cout << "Webserv: " << str << std::endl;
		exit(EXIT_FAILURE);
	}

	void	*memset(void *b, int c, size_t len)
	{
		unsigned char	*str;

		str = static_cast<unsigned char*>(b);
		while (len > 0)
		{
			*str = c;
			str++;
			len--;
		}
		return (b);
	}

	unsigned short	htons(unsigned short x)
	{
		return ((((x) >> 8) & 0xff ) | (((x) & 0xff) << 8));
	}
}

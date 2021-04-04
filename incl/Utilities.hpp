/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utilities.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 21:38:40 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 17:54:39 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITIES_HPP
# define UTILITIES_HPP

#include <map>
#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 256
#endif

#ifndef MB
# define MB 1000000
#endif

#ifndef PAGE_SIZE
# define PAGE_SIZE 4096
#endif

#ifndef PIPE_CAPACITY_MAX
# define PIPE_CAPACITY_MAX (PAGE_SIZE * 16)
#endif

extern bool g_sigpipe;

namespace ft
{
	std::string	itos(int num, const std::string base = "0123456789");

	int stoi(std::string number, const std::string base = "0123456789");
	size_t stoul(std::string number, const std::string base = "0123456789");

	template <class Iter>
	void	printIteration(Iter first, Iter last, std::string delim = ",")
	{
		size_t distance = std::distance(first, last);
		while (first != last)
			std::cout << *first << ((++first == last) ? "\n" : delim);
		if (!distance)
			std::cout << std::endl;
	}

	char			*strdup(const char *s1);

	std::string toUpperStr(const std::string& str);

	std::string	rawString(std::string const &str);

	std::pair<std::string, std::string>	getKeyval(std::string raw, std::string delimiter = ": ");

	size_t	firstOfGroup(std::string raw, const std::vector<std::string>& delim_groups, size_t search_start, int& match);
	std::vector<std::string>	split(std::string raw, std::vector<std::string>& delim);

	std::vector<std::string>	split(std::string raw, std::string delim, std::string preserve_delim = "");

	struct SizeCompare
	{
		bool	operator() (const std::string& a, const std::string& b) const;
	};

	bool	isUpperStr(const std::string& str);

	bool	isLowerStr(const std::string& str);

	bool	onlyConsistsOf(const std::string& str, std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	std::string	base64Decode(std::string input, std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

	std::string removeSet(std::string str, std::string remove);
}

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utilities.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/03 21:38:40 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/25 17:43:15 by tbruinem      ########   odam.nl         */
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

namespace ft
{
	std::string	itos(int num, const std::string base = "0123456789");

	int stoi(std::string number, const std::string base = "0123456789");
	size_t stoul(std::string number, const std::string base = "0123456789");

	template <class T1, class T2>
	T1	maxElement(const std::map<T1,T2>& container)
	{
		typename std::map<T1,T2>::const_reverse_iterator it;
		if (!container.size() || (it = container.rbegin()) == container.rend())
			return (T1());
		return (it->first);
	}

	template <class T1>
	T1	maxElement(const std::vector<T1>& container)
	{
		typename std::vector<T1>::const_reverse_iterator it;
		if (!container.size() || (it = container.rbegin()) == container.rend())
			return (T1());
		return (std::max_element(container.begin(), container.end()));
	}

	template <class T>
	T	max(T a, T b)
	{
		return ((a > b) ? a : b);
	}

	template <class Iter>
	void	printIteration(Iter first, Iter last, std::string delim = ",")
	{
		size_t distance = std::distance(first, last);
		for (; first != last;)
			std::cout << *first << ((++first == last) ? "\n" : delim);
		if (!distance)
			std::cout << std::endl;
	}

	void	putError(const std::string &str);

	void	*memset(void *b, int c, size_t len);

	unsigned short	hostToNetworkShort(unsigned short x);

	char			*strdup(const char *s1);

	std::string toUpperStr(const std::string& str);

	std::string	rawString(std::string const &str);

	struct tm					getTime(time_t sec = 0);

	std::vector<std::string>	getLines(int fd, std::string eol_sequence = "\n", int *ret = NULL, bool encoding = false, bool file = false, size_t max_lines = std::numeric_limits<size_t>::max());
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

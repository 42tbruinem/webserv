/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utilities.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:29:21 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/04/04 15:08:10 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include "Utilities.hpp"
#include <arpa/inet.h>

bool	g_sigpipe;

namespace ft
{
	bool	SizeCompare::operator () (const std::string& a, const std::string& b) const
	{
		if (a.size() != b.size())
			return (a.size() > b.size());
		return (a > b);
	}

	std::string	itos(int num, const std::string base)
	{
		size_t size = (num <= 0);
		num *= ((num >= 0) + (num >= 0) - 1);
		for (int tmp = num; tmp ; tmp /= base.size(), size++) {}
		if (!size || !base.size())
			return ("");
		std::string	number(size, '-');
		if (!num)
			number[num] = base[num];
		for (--size; num ; num /= base.size(), size--)
			number[size] = base[num % base.size()];
		return (number);
	}

	std::string	base64Decode(std::string input, std::string charset)
	{
		size_t bits = input.size() * 6;
		bits -= (((input.size() >= 1 && input[input.size() - 1] == '=') + (input.size() >= 2 && input[input.size() - 2] == '=')) * 8);
		std::string output(bits / 8, '\0');
		char encoded;

		for (size_t i = 0; i < bits; i++)
		{
			if (i % 6 == 0)
				encoded = (char)charset.find(input[i / 6]);
			output[i / 8] = output[i / 8] * 2 + (encoded >> (5 - (i % 6)) & 1);
		}
		return (output);
	}

	int stoi(std::string number, const std::string base)
	{
		int res = 0;

		if (!number.size())
			return (res);
		int sign = (number[0] == '+') - (number[0] == '-');
		for (size_t i = !!sign; i < number.size(); i++)
		{
			size_t num = base.find(number[i]);
			if (num == std::string::npos)
				continue;
			res = res * base.size() + num;
		}
		sign += (!sign);
		return (res * sign);
	}

	size_t stoul(std::string number, const std::string base)
	{
		size_t res = 0;

		if (!number.size())
			return (res);
		for (size_t i = 0; i < number.size(); i++)
		{
			size_t num = base.find(number[i]);
			if (num == std::string::npos)
				continue;
			res = res * base.size() + num;
		}
		return (res);
	}

	size_t	firstOfGroup(std::string raw, const std::vector<std::string>& delim_groups, size_t search_start, int& match)
	{
		size_t smallest = std::string::npos;

		for (size_t i = 0; i < delim_groups.size(); i++)
		{
			size_t tmp = raw.find(delim_groups[i], search_start);
			if (tmp < smallest)
			{
				match = (int)i;
				smallest = tmp;
			}
		}
		return (smallest);
	}

	std::vector<std::string>	split(std::string raw, std::vector<std::string>& delim)
	{
		std::vector<std::string>	tokens;
		size_t	end;
		int		match;
		for (size_t begin = 0 ; begin < raw.size();)
		{
			match = -1;
			end = firstOfGroup(raw, delim, begin, match);
			if (end == std::string::npos)
				end = raw.size();
			if (begin != end)
				tokens.push_back(raw.substr(begin, end - begin));
			if (match == -1)
				break ;
			begin = end + delim[match].size();
		}
		return (tokens);
	}

	std::vector<std::string>	split(std::string raw, std::string delim, std::string preserve_delim)
	{
		std::vector<std::string>	tokens;
		size_t	end;
		for (size_t begin = 0 ; begin < raw.size();)
		{
			end = raw.find_first_of(delim, begin);
			if (end == std::string::npos)
				end = raw.size();
			if (begin != end)
				tokens.push_back(raw.substr(begin, end - begin));
			if (preserve_delim.size() && end < raw.size() && preserve_delim.find(raw[end]) != std::string::npos)
				tokens.push_back(std::string(1, raw[end]));
			for (end = end + 1; end < raw.size() && delim.find(raw[end]) != std::string::npos && (preserve_delim.size() == 0 || preserve_delim.find(raw[end]) == std::string::npos) ; end++) {}
			begin = end;
		}
		return (tokens);
	}

	void	memset(void *b, int c, size_t len)
	{
		std::fill((unsigned char*)b, (unsigned char*)b + len, c);
	}

	char	*strdup(const char *str)
	{
		char	*dup;
		size_t	i = 0;

		while (str[i])
			i++;
		dup = (char*)malloc(sizeof(char) * (i + 1));
		if (dup == NULL)
			return (NULL);
		for (size_t j = 0; j < i; j++)
			dup[j] = str[j];
		dup[i] = '\0';
		return (dup);
	}

	std::string	toUpperStr(const std::string& str)
	{
		std::string 	upper_string;

		upper_string.reserve(str.size());
		for (size_t i = 0 ; i < str.size(); i++)
		{
			if (str[i] >= 'a' && str[i] <= 'z')
				upper_string += (str[i] - 'a') + 'A';
			else
				upper_string += str[i];
		}
		return upper_string;
	}

	bool	isUpperStr(const std::string& str)
	{
		return (onlyConsistsOf(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	}

	bool	isLowerStr(const std::string& str)
	{
		return (onlyConsistsOf(str, "abcdefghijklmnopqrstuvwxyz"));
	}

	bool	onlyConsistsOf(const std::string& str, std::string charset)
	{
		return (str.find_first_not_of(charset) == std::string::npos);
	}

	std::string removeSet(std::string str, std::string remove)
	{
		size_t size = str.size();
		for (size_t i = 0; i < remove.size(); i++)
			size -= std::count(str.begin(), str.end(), remove[i]);
		std::string result(size, ' ');
		size = 0;
		for (size_t i = 0; i < str.size(); i++)
		{
			if (remove.find(str[i]) == std::string::npos)
				result[size++] = str[i];
		}
		return (result);
	}

	std::string rawChar(char c)
	{
		switch (c)
		{
			case '\a': return "\\a";
			case '\b': return "\\b";
			case '\t': return "\\t";
			case '\n': return "\\n";
			case '\v': return "\\v";
			case '\f': return "\\f";
			case '\r': return "\\r";

			case '\"': return "\\\"";
			case '\'': return "\\\'";
			case '\?': return "\\\?";
			case '\\': return "\\\\";
			default: return std::string(1, c);
		}
	}

	std::string	rawString(std::string const &str)
	{
		std::string	tmp;

		for (unsigned long i = 0; i < str.length(); i++)
			tmp.append(rawChar(str[i]));

		return tmp;
	}

	std::pair<std::string, std::string>	getKeyval(std::string raw, std::string delimiter)
	{
		std::pair<std::string, std::string>	keyval;
		size_t	delim_pos = raw.find(delimiter);
		if (delim_pos == std::string::npos)
			throw std::runtime_error("Error: delimiter string not found in 'getKeyval'");
		keyval.first = raw.substr(0, delim_pos);
		keyval.second = raw.substr(delim_pos + delimiter.size(), raw.size());
		return (keyval);
	}
}

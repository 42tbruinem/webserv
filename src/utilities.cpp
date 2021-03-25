/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utilities.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 20:29:21 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/25 17:43:37 by tbruinem      ########   odam.nl         */
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

	void	putError(const std::string &str) {

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

	unsigned short	hostToNetworkShort(unsigned short x)
	{
		return ((((x) >> 8) & 0xff ) | (((x) & 0xff) << 8));
	}

	char	*strdup(const char *s1)
	{
		char	*p;
		int		len = 0;
		int		i = 0;

		while (s1[i])
		{
			len++;
			i++;
		}
		p = (char*)malloc((sizeof(char) * len) + 1);
		if (p == 0)
			return (NULL);
		for (int j = 0; j < len; j++)
			p[j] = s1[j];
		p[len] = '\0';
		return (p);
	}

	std::string	toUpperStr(const std::string& str)
	{
		std::string 	upperstr = "";

		for (unsigned long i = 0; i < str.size(); i++)
			upperstr.push_back(toupper(str[i]));

		return upperstr;
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

	std::string rawChar(char c) {
		switch (c) {
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
		}
		std::string tmp;
		tmp.push_back(c);
		return tmp;
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

	struct tm getTime(time_t sec)
	{
		struct timeval	tv;
		struct tm		tm;

		gettimeofday(&tv, NULL);

		if (sec != 0)
			tv.tv_sec = sec;

		tm.tm_sec = tv.tv_sec % 60;

		int minutes = (tv.tv_sec - tm.tm_sec) / 60;

		tm.tm_min =  minutes % 60;
		int days = (((minutes - tm.tm_min) / 60) + 1) / 24;
		int year = ((((minutes - tm.tm_min) / 60) + 1) / 24) / 365 + 1970;

		tm.tm_year = year - 1900;
		tm.tm_hour = (((minutes - tm.tm_min) / 60) + 1) % 24;
		tm.tm_yday = (days % 365 - ((year - 1972) / 4)) - 1;

		int	monthdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		// Change february if leap year
		if ((year - 1972) % 4 == 0)
			monthdays[1] = 29;

		// Check if daylight saving time is active
		int		first_day;
		int 	last_day = 0;

		first_day = monthdays[0] + monthdays[1] + monthdays[2] - 1;
		while ((tm.tm_year * 365 + first_day + ((year - 1900) / 4)) % 7 != 6)
			first_day--;

		for (int i = 0; i < 10; i++)
			last_day += monthdays[i];
		while ((tm.tm_year * 365 + last_day + ((year - 1900) / 4)) % 7 != 6)
			last_day--;

		// If time is between last sunday of march and last sunday of october, add hour for daylight saving time
		if ((tm.tm_yday > first_day || (tm.tm_yday == first_day && tm.tm_hour >= 2)) &&
			(tm.tm_yday < last_day || (tm.tm_yday == last_day && tm.tm_hour <= 2))) {
			tv.tv_sec += 3600;
		}

		tm.tm_sec = tv.tv_sec % 60;
		minutes = (tv.tv_sec - tm.tm_sec) / 60;
		tm.tm_min =  minutes % 60;

		int hours = ((minutes - tm.tm_min) / 60) + 1;

		tm.tm_hour = hours % 24;
		days = hours / 24;
		year = days / 365 + 1970;
		tm.tm_year = year - 1900;
		tm.tm_mday = days % 365 - ((year - 1972) / 4);
		tm.tm_yday = tm.tm_mday;

		// Set correct month
		for (int i = 0; i < 12; i++) {
			// Found month
			if (tm.tm_mday < monthdays[i] + 1 ) {
				tm.tm_mon = i;
				break;
			}
			tm.tm_mday -= monthdays[i];
		}

		// Set correct weekday
		tm.tm_wday = (tm.tm_year * 365 + tm.tm_yday + ((year - 1900) / 4)) % 7;

		return tm;
	}
}

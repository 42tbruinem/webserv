/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_line_test.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 14:15:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/02/02 18:59:33 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <limits>
#include <utility>

std::pair<std::string, std::string>	get_keyval(std::string raw, char delimiter = ':');
std::vector<std::string>	get_lines(int fd, size_t max_lines = std::numeric_limits<size_t>::max());

int main(int argc, char **argv)
{
	std::vector<std::string>	lines;
	if (argc == 1)
		return (!!printf("No arguments provided\n"));
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (!!printf("Not a valid file\n"));
	lines = get_lines(fd);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		size_t iskeyval = ((*it).find(':') == std::string::npos ? false : true);
		if (iskeyval)
		{
			std::pair<std::string, std::string>	keyval = get_keyval(*it);
			std::cout << "KEY: " << keyval.first << " | VAL: " << keyval.second << std::endl;
		}
		std::cout << *it << std::endl;
	}
	// while ((lines = get_lines(fd, 1)) != std::vector<std::string>())
	// {
	// 	std::cout << "PRINTED LINES[0] = " << lines[0] << std::endl;
	// }
//	std::cout << "DOUBLE NEWLINE CHECK\n";
	return (0);
}

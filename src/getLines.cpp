/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   getLines.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/25 17:45:58 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/26 14:53:14 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <map>
#include <string>
#include <limits>
#include <algorithm>
#include <unistd.h>
#include <iostream>

#include "Utilities.hpp"

namespace ft
{
	std::vector<std::string>	getLines(int fd, std::string eol_sequence, int* ret, bool encoding, size_t max_lines)
	{
		static std::map<int,std::string>	buffers;
		std::vector<std::string>			lines;
		char								buf[BUFFER_SIZE + 1];
		ssize_t								bytes_read = 1;
		size_t								end_pos;

		for (size_t i = 0; i < max_lines && bytes_read; i++)
		{
			while ((end_pos = buffers[fd].find(eol_sequence)) == std::string::npos)
			{
				bytes_read = read(fd, buf, BUFFER_SIZE);
				if (bytes_read == -1)
				{
					if (eol_sequence.size() > 1 && buffers[fd].size() && buffers[fd][0] != eol_sequence[0] && !encoding &&
						std::find(lines.begin(), lines.end(), "") != lines.end() && lines.size() &&
						std::find(lines.begin(), lines.end(), "Transfer-Encoding: chunked") == lines.end() &&
						buffers[fd].find("Transfer-Encoding: chunked") == std::string::npos)
					{
						end_pos = buffers[fd].find(eol_sequence);
						lines.push_back(buffers[fd].substr(0, end_pos));
						buffers.erase(fd);
					}
					return (lines);
				}
				buf[bytes_read] = '\0';
				buffers[fd].append(std::string(buf));
				if (!bytes_read)
				{
					if (ret && !lines.size())
						*ret = -1;
					break ;
				}
			}
			end_pos = buffers[fd].find(eol_sequence);
			if (end_pos != std::string::npos)
			{
				std::string new_line;
				if (end_pos == buffers[fd].size())
					new_line = "";
				else
					new_line = buffers[fd].substr(0, end_pos);
				if (new_line.empty())
					lines.push_back("");
				else if (i >= lines.size())
					lines.push_back(new_line);
				else
					lines[i].append(new_line);
				buffers[fd] = buffers[fd].substr(end_pos + eol_sequence.size(), buffers[fd].size());
			}
			else
			{
				end_pos = buffers[fd].size() - 1;
				buffers[fd].clear();
			}
			if (bytes_read == 0)
				return (lines);
			if (bytes_read == -1)
			{
				buffers[fd].clear();
				return (lines);
			}
		}
		return (lines);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:37:38 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 22:27:04 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Request.hpp"
#include <exception>
#include "Utilities.hpp"
#include "Method.hpp"

Request::Request(std::string content, bool encoding) :
uri(""),
content(content),
done(false),
status_line(""),
status_code(200),
method(GET),
body_read(0),
body_total(-1),
body_started(false),
encoding(encoding) {}

Request::Request(const Request& other) : uri(other.uri), method(other.method)
{
	*this = other;
}

Request& Request::operator = (const Request& other)
{
	if (this != &other)
	{
		this->done = other.done;
		this->status_line = other.status_line;
		this->lines = other.lines;
		this->status_code = other.status_code;
		this->method = other.method;
		this->path = other.path;
		this->headers = other.headers;
		this->body = other.body;
		this->uri = other.uri;
		this->body_read = other.body_read;
		this->body_total = other.body_total;
		this->body_started = other.body_started;
		this->encoding = other.encoding;
	}
	return (*this);
}

bool	Request::isMethod(std::string str)
{
	static const char *methods[] = {
	"OPTIONS",
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"TRACE",
	"CONNECT"
	};

	for (size_t i = 0; i < sizeof(methods) / sizeof(char *); i++)
		if (std::string(methods[i]) == str)
			return (true);
	return (false);
}

bool	Request::parseHeader(std::string& line)
{
	(void)line;
	return (true);
}

bool	Request::process()
{
	//might need to skip over empty lines at the start

	//parse status_line
	size_t		end_of_statusline = this->content.find("\r\n");
	if (!parseStatusLine(this->content.substr(0, end_of_statusline)))
		return (false);

	//parse headers
	size_t		end_of_headers = this->content.find("\r\n\r\n");
	std::vector<std::string>	headers = ft::split(this->content.substr(end_of_statusline + 2, end_of_headers), "\r\n");
	for (size_t i = 0; i < headers.size(); i++)
	{
		if (!parseHeader(headers[i]))
			return (false);
	}
	if (encoding)
	{



	}
	else
	{


	}
	return (true);
}

Request::~Request() {}

bool Request::isStatusLine(const std::string &line)
{
	size_t i = 0;
	if (!line.size() || line.size() >= 8000 || line.find(' ') == std::string::npos)
		return (false);
	std::vector<std::string>	parts = ft::split(line, " \r", " \r");
	if (parts[i++].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos)
		return (false);
	if (parts[i++] != " ")
		return (false);
	for (; parts[i] == " "; i++) {}
	size_t end = parts.size() - 1;
	for (;end > 0 && parts[end] == " "; end--) {}
	if (parts[end].substr(0, 5) != "HTTP/")
		return (false);
	std::vector<std::string>	version = ft::split(parts[end].substr(5, parts[end].size()), ".");
	if (version.size() != 2)
		return (false);
	if (version[0].find_first_not_of("0123456789") != std::string::npos)
		return (false);
	if (version[1].find_first_not_of("0123456789") != std::string::npos)
		return (false);

	if (parts[--end] != " ")
		return (false);
	if (i >= end)
		return (false);
	for (;end > i && parts[end] == " "; end--) {}

	std::string request_target;
	for (--end; i < end; i++)
		request_target += parts[i];
	
	return (true);
}

bool Request::parseStatusLine(const std::string &line)
{
	if (line.empty() || !isStatusLine(line))
		return (false);
	int start = line.length() - 1;

	while (line[start] == ' ' || (line[start] > 9 && line[start] < 13))
		start--;

	int end = start;

	while (line[start] != ' ')
		start--;

	if (line.substr(start + 1, end - start) != "HTTP/1.1")
	{
		this->status_code = 505;
		return (true);
	}
	this->status_line = line;
	return (false);
}

bool	Request::parseLine(std::string line)
{
	if (line.empty())
	{
		if (this->status_line.empty())
			return false;
		else if (this->status_line.size() && this->lines.size() == 0)
		{
			this->status_code = 400;
			return (true);
		}
		else if (this->status_line.size() && this->lines.size() > 0 && !this->encoding)
		{
			if (this->body_total != 0 && this->body_total > this->body_read)
			{
				if (this->body_started)
				{
					this->body_read += 2;
					this->lines.push_back("\r\n");
				}
				else
					this->lines.push_back("\r");
				this->body_started = true;
				return false;
			}

			int	end_pos_method = this->status_line.find(' ');
			int start_pos_path = end_pos_method;

			while (this->status_line[start_pos_path] == ' ')
				start_pos_path++;

			int end_pos_path = this->status_line.length() - 1;
			while (this->status_line[end_pos_path] == ' ')
				end_pos_path--;
			end_pos_path -= 8;

			while (this->status_line[end_pos_path] == ' ')
				end_pos_path--;
			end_pos_path++;

			std::string methodpart = this->status_line.substr(0, end_pos_method);
			if (isMethod(methodpart))
				this->method = Method(methodpart);
			else
				this->status_code = 405;
			this->path = this->status_line.substr(start_pos_path, end_pos_path - start_pos_path);
			std::cout << "Request received to: " << this->path << std::endl;
			this->uri = URI(path);
			if (this->uri.getPort() == "" && this->uri.getScheme() == "HTTP")
				this->uri.setPort("80");
			this->splitRequest();

			return (true);
		}
	}
	else if (isStatusLine(line))
	{
		if (this->status_line == "")
		{
			int start = line.length() - 1;

			while (line[start] == ' ' || (line[start] >= 10 && line[start] <= 13))
				start--;

			int end = start;

			while (line[start] != ' ')
				start--;

			if (line.substr(start + 1, end - start) != "HTTP/1.1")
			{
				this->status_code = 505;
				return (true);
			}
			this->status_line = line;
		}
		return (false);
	}
	else if (line.find(':') != std::string::npos && !this->body_started)
	{
		if (this->status_line == "")
		{
			this->status_code = 400;
			return (true);
		}
		size_t carriage_return = line.find_last_of('\r');

		if (line.size() >= 16 && line.substr(0, 16) == "Content-Length: ")
		{
			if (carriage_return != std::string::npos)
				this->body_total = ft::stoi(line.substr(16, line.length() - 17));
			else
				this->body_total = ft::stoi(line.substr(16, line.length() - 16));
		}
		else if (line.size() >= 19 && line.substr(0, 19) == "Transfer-Encoding: ")
			this->encoding = true;

		if (carriage_return != std::string::npos && carriage_return + 1 == line.size())
			this->lines.push_back(line.substr(0, line.size() - 1));
		else
			this->lines.push_back(line);
		return (false);
	}
	if (this->status_line != "")
	{
		if (body_started)
		{
			//pleasefix
			//useless check??
			size_t carriage = line.find_last_of('\r');
			if (carriage != std::string::npos && carriage + 1 == line.size())
				line += "\n";
			else
				line += "\r\n";

			std::string newLine = "";

			for (int i = 0; line[i] != '\0'; i++)
			{
				newLine.push_back(line[i]);
				this->body_read++;
				if (this->body_read == this->body_total)
					break;
			}
			if (this->encoding && this->lines.back() != "")
				this->lines.back().append(newLine);
			else
				this->lines.push_back(newLine);

			if (this->body_read >= this->body_total)
			{
				if (!this->encoding)
					return (this->parseLine(""));
				this->body_read = 0;
				this->body_total = -1;
				this->body_started = false;
			}
		}
		else if (this->encoding)
		{
			if (this->body_total == -1 && line != "")
			{
				this->body_total = ft::stoi(ft::toUpperStr(line), "0123456789ABCDEF");
				if (this->body_total == 0)
					this->encoding = false;
				else
					this->body_started = true;
			}
			else if (line == "")
				this->lines.push_back(line);
		}
		return (false);
	}
	this->status_code = 400;
	return (true);
}

void Request::splitRequest(void) 
{

	std::vector<std::string>::iterator	header_end;

	for (header_end = this->lines.begin(); header_end != this->lines.end(); header_end++) {
		if (*header_end == "\r" || *header_end == "")
			break;
	}

	for (std::vector<std::string>::iterator it = this->lines.begin(); it != header_end; it++) {
		if ((*it).find(": ") != std::string::npos)
		{
			std::pair<std::string, std::string>	keyval = ft::getKeyval(*it, ": ");
			this->headers.insert(keyval);
		}
	}

	if (header_end != lines.end())
	{
		header_end++;
		if (header_end != lines.end())
		{
			for (std::vector<std::string>::iterator it = header_end; it != this->lines.end(); it++)
				this->body.push_back(*it);
		}
	}
}

bool			Request::getDone() const { return this->done; }
std::string		Request::getMethod() const { return this->method.getStr(); }
std::string		Request::getPath() const { return this->path; }
int				Request::getStatusCode() const { return this->status_code; }
std::map<std::string, std::string>&	Request::getHeaders() { return this->headers; }
std::vector<std::string>&	Request::getBody() { return this->body; }

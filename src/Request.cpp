/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 19:37:38 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/03 21:41:25 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Request.hpp"
#include <exception>
#include "Utilities.hpp"
#include "Method.hpp"

//		Constructors

Request::Request() :
uri(""),
content(),
done(false),
status_line(""),
status_code(200),
method(GET),
body_read(0),
body_total(-1),
rawbody(""),
end_of_headers(0),
end_of_body(0),
encoding(false)
{}

Request::Request(const Request& other) : uri(""), method(GET)
{
	*this = other;
}

//		Operators

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
		this->rawbody = other.rawbody;
		this->end_of_body = other.end_of_body;
		this->end_of_headers = other.end_of_headers;
		this->encoding = other.encoding;
		this->content = other.content;
	}
	return (*this);
}

//		Parsing

bool	Request::parseHeader(std::string& line)
{
	if (line.find(": ") == std::string::npos)
		return (false);
	std::pair<std::string, std::string>	keyval;

	keyval = ft::getKeyval(line);
	this->headers[keyval.first] = keyval.second;
	return (true);
}

bool	parseChunksIntoBody(std::vector<std::string> chunks, std::vector<std::string>& body)
{
	std::string	raw;
	
	for (std::vector<std::string>::iterator it = chunks.begin(); it != chunks.end(); it++)
	{
		if (it->empty() || !ft::onlyConsistsOf(*it, "0123456789abcdefABCDEF"))
			return (false);
		*it = ft::toUpperStr(*it);
		size_t chunk_size = ft::stoul(*it, "0123456789ABCDEF");
		it++;
		std::string chunk_content;
		for (; it != chunks.end() && chunk_content.size() < chunk_size; it++)
		{
			if (chunk_content.size())
				chunk_content += "\r\n";
			if (chunk_content.size() + it->size() > chunk_size)
				return (false);
			chunk_content += *it;
			if (chunk_content.size() == chunk_size)
				break ;
		}
		raw += chunk_content;
		if (!chunk_size)
			break ;
	}
	if (raw.size())
		body = ft::split(raw, "\r\n");
	else
		body.clear();
	return (true);
}

//return:
//-1 = error
//0 = not found
//1 = found
int		Request::bodyEnd(std::string& bytes)
{
	ssize_t	end;

	//if it contains Transfer-Encoding
	if (encoding || this->headers.count("Transfer-Encoding"))
	{
		std::vector<std::string>			value;
		std::string							to_search;
		size_t								oldsize;

//		std::cout << "ENCODING FOUND" << std::endl;
		if (!encoding)
		{
			value = ft::split(this->headers["Transfer-Encoding"], " ");
//			ft::printIteration(value.begin(), value.end());
			if (value.empty() || value[value.size() - 1] != "chunked")
				return (-1);
			encoding = true;
		}
		if (rawbody.size() < 5)
			to_search = rawbody;
		else
			to_search = rawbody.substr(rawbody.size() - 5, 5);
		oldsize = to_search.size();
		to_search += bytes;
		end = to_search.find("0\r\n\r\n");
		if ((size_t)end == std::string::npos)
		{
			rawbody += bytes;
//			std::cout << "RAWBODY: " << ft::rawString(rawbody) << std::endl;
			bytes.clear();
			return (0);
		}
		rawbody += bytes.substr(0, (end + 5) - oldsize);
		bytes = bytes.substr((end + 5) - oldsize, bytes.size());
//		std::cout << "RAWBODY: " << ft::rawString(rawbody) << std::endl;
//		std::cout << "BYTES: " << ft::rawString(bytes) << std::endl;
		return (true);
	}
	//need to check for duplicate Content-Length headerfields
	//if it contains Content-Length
	else if (!encoding && this->headers.count("Content-Length"))
	{
		std::vector<std::string>			value;
		size_t								overflow;

		//if we haven't calculated bodysize yet
		if (!end_of_body)
		{
			value = ft::split(this->headers["Content-Length"], " ");
//			ft::printIteration(value.begin(), value.end());
			if (value.size() != 1 || !ft::onlyConsistsOf(value[0], "0123456789"))
				return (-1);
			end_of_body = ft::stoul(value[0]);
		}
		//if we haven't reached bodysize yet
		if (end_of_body >= rawbody.size() + bytes.size())
		{
			rawbody += bytes;
			bytes.clear();
		}
		if (end_of_body > rawbody.size() + bytes.size())
			return (0);
		if (end_of_body == rawbody.size() + bytes.size())
			return (1);
//		std::cout << "BYTES: " << ft::rawString(bytes) << std::endl;
		overflow = end_of_body - (rawbody.size() + bytes.size());
		rawbody += bytes.substr(0, overflow);
		std::cout << overflow << std::endl;
		bytes = bytes.substr(overflow, bytes.size());
		return (1);
	}
	else
		return (1);
}

//return:
// -1 = error
// 0 = not found
// 1 = found
int		Request::findEndOfRequest(std::string& buffer)
{
	std::string		to_search;
	ssize_t			tmp = 0;
	size_t			old_content_size;

	//set out the area we're going to search
	if (content.size() < 25)
		to_search = content;
	else
		to_search = content.substr(content.size() - 25, content.size());
	old_content_size = to_search.size();
	to_search += buffer;

	//if we haven't found the end of the headers yet
//	std::cout << "END_OF_HEADERS: " << end_of_headers << std::endl;
//	std::cout << ft::rawString(to_search) << std::endl;
	if (!end_of_headers && ((size_t)(tmp = to_search.find("\r\n\r\n")) != std::string::npos))
	{
//		std::cout << "FOUND CRLF CRLF" << std::endl;
		size_t		end_of_statusline;

		end_of_headers = (this->content.size() + tmp) - old_content_size;
		//add last part of headers to content
		this->content += buffer.substr(0, tmp - old_content_size);
		//remove everything that's part of headers
		buffer = to_search.substr(tmp + 4, to_search.size());
		to_search = buffer;
		end_of_statusline = this->content.find("\r\n");
		std::string	status_line = this->content.substr(0, end_of_statusline);
		if (!parseStatusLine(status_line))
			return (-1);

		//parse headers
		std::vector<std::string>	headers = ft::split(this->content.substr(end_of_statusline + 2, (end_of_headers - (end_of_statusline + 2))), "\r\n");
		for (size_t i = 0; i < headers.size(); i++)
		{
			if (!parseHeader(headers[i]))
				return (-1);
		}
//		std::cout << "FOUND END OF HEADERS" << std::endl;
		//the request might not expect any body
		content.clear(); //just processed
	}
	else if (!end_of_headers)
	{
		content += buffer;
		buffer.clear();
		return (0);
	}
	//find the end of the entire request
	if (end_of_headers)
	{
//		std::cout << "CHECKING IF BODY END IS FOUND:" << std::endl;
		//if we've not found the end of the body yet
		int ret;
		ret = bodyEnd(to_search);
		if (ret <= 0)
		{
			buffer.clear();
			return (ret);
		}
		buffer = to_search;
//		std::cout << "BODY END WAS FOUND:" << std::endl;
//		std::cout << buffer << std::endl;
		return (1);
	}
	return (1);
}

bool	Request::process()
{
	//might need to skip over empty lines at the start

	// if (this->content.size() >= 10 * MB)
	// {
	// 	int i = 0;
	// 	(void)i;
	// }
	//parse status_line
	//std::cout << "RAWBODY: " << ft::rawString(this->rawbody) << std::endl;

	this->body = ft::split(this->rawbody, "\r\n");
	//parse chunks
	if (encoding && !parseChunksIntoBody(this->body, this->body))
		return (false);
	return (true);
}

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

	while (line[start] == ' ' || (line[start] >= 9 && line[start] <= 13))
		start--;

	int end = start;

	while (line[start] != ' ')
		start--;

	if (line.substr(start + 1, end - start) != "HTTP/1.1")
	{
		this->status_code = 505;
		return (false);
	}
	this->status_line = line;
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
	{
		this->status_code = 405;
		return (false);
	}
	this->path = this->status_line.substr(start_pos_path, end_pos_path - start_pos_path);
	std::cout << "Request received to: " << this->path << std::endl;
	this->uri = URI(path);
	if (this->uri.getPort() == "" && this->uri.getScheme() == "HTTP")
		this->uri.setPort("80");
	return (true);
}

void	Request::printRequest(void) const
{
	// Print values for debugging
	std::cout << std::endl << "Request:" << std::endl;
	std::cout << "  Headers:" << std::endl;
	std::cout << "\t" << this->method.getStr() << " " << this->path << " HTTP/1.1" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	if (this->body.size())
	{
		int	amount_printed = 0;
		std::cout << "  Body:" << std::endl;
		for (std::vector<std::string>::const_iterator it = this->body.begin(); it != this->body.end() && amount_printed < 5; it++)
		{
			if ((*it).length() > 1000)
			{
				std::cout << "\t" << ft::rawString((*it).substr(0, 1000)) << "..." << std::endl;
				amount_printed++;
			}
			else
				std::cout << "\t" << *it << std::endl;
		}
	}
	else
		std::cout << "  No body" << std::endl;
}

bool									Request::getDone() const { return this->done; }
std::string								Request::getMethod() const { return this->method.getStr(); }
std::string								Request::getPath() const { return this->path; }
int										Request::getStatusCode() const { return this->status_code; }
std::map<std::string, std::string>&		Request::getHeaders() { return this->headers; }
std::vector<std::string>&				Request::getBody() { return this->body; }

Request::~Request() {}

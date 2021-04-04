/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/17 13:59:06 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/04/04 17:55:01 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "Cgi.hpp"
#include "Utilities.hpp"
#include <cstring>

Cgi::Cgi() 
{
	memset(this->args, 0, sizeof(args));
}

Cgi::Cgi(const Cgi & src)
{
	*this = src;
}

Cgi &	Cgi::operator=(const Cgi & rhs)
{
	if (this != &rhs)
	{
		this->vars = rhs.vars;
		this->env = rhs.env;
	}

	return *this;
}

Cgi::~Cgi() 
{
	int		i = 0;

	for (size_t i = 0; i < 3; i++)
		free(args[i]);

	while (this->env[i])
	{
		free(this->env[i]);
		this->env[i] = 0;
		i++;
	}
	if (this->env)
	{
		free(this->env);
		this->env = 0;
	}
	this->vars.clear();
}

void	Cgi::setEnv(Request *req, std::string path, std::string host, std::string port, std::string phpcgi)
{
	this->vars["AUTH_TYPE"] = req->getHeader("Authorization");
	if (req->getHeader("Content-Length") == "")
	{
		if (req->getHeader("Transfer-Encoding") == "")
			this->vars["CONTENT_LENGTH"] = "0";
		else
		{
			int	len = 0;
			for (std::vector<std::string>::iterator it = req->getBody().begin(); it != req->getBody().end(); it++)
				len += (*it).size();
			this->vars["CONTENT_LENGTH"] = ft::itos(len);
		}
	}
	else
		this->vars["CONTENT_LENGTH"] = req->getHeader("Content-Length");
	this->vars["CONTENT_TYPE"] = req->getHeader("Content-Type");
	this->vars["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->vars["PATH_INFO"] = req->uri.getUri();

	char buf[512];
	std::string	root = getcwd(buf, 512);
	this->vars["PATH_TRANSLATED"] = root + req->uri.getUri();
	if ( req->getPath().length() > 3 && req->getPath().substr(req->getPath().length() - 4, 4) == ".php" && !phpcgi.empty())
	{
		this->vars["QUERY_STRING"] = req->uri.getQuery();
		for (std::vector<std::string>::iterator it = req->getBody().begin(); it != req->getBody().end(); it++)
			this->vars["QUERY_STRING"] += *it;
	}
	else
		this->vars["QUERY_STRING"] = "";
	this->vars["REMOTE_ADDR"] = "localhost";
	this->vars["REMOTE_IDENT"] = "";
	this->vars["REMOTE_USER"] = "";
	this->vars["REQUEST_METHOD"] = req->getMethod();
	this->vars["REQUEST_URI"] = req->uri.getUri();
	this->vars["SCRIPT_NAME"] = path;
	this->vars["SCRIPT_FILENAME"] = path;
	this->vars["SERVER_NAME"] = host;
	this->vars["SERVER_PORT"] = port;
	this->vars["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->vars["SERVER_SOFTWARE"] = "HTTP/1.1";
	this->vars["REDIRECT_STATUS"] = "true";

	for (std::map<std::string, std::string>::iterator it = req->getHeaders().begin(); it != req->getHeaders().end(); it++)
		this->vars["HTTP_" + ft::toUpperStr(it->first)] = it->second;

	int 	i = 0;
	this->env = (char**)malloc(sizeof(char *) * (this->vars.size() + 1));

	for (std::map<std::string, std::string>::iterator it = this->vars.begin(); it != this->vars.end(); it++)
	{
		this->env[i] = ft::strdup((it->first + "=" + it->second).c_str());
		if (!this->env[i])
		{
			for (int j = i - 1; j >= 0; j--)
				free(this->env[j]);
			free(this->env);
			throw std::runtime_error("Error: malloc failed in Cgi::set_this->_env");
		}
		i++;
	}
	this->env[i] = 0;
}

void	Cgi::execute(Request *req, std::string path, std::string host, std::string port, std::string phpcgi)
{
	pid_t	pid;
	int 	in_fd, out_fd, status;

	this->args[0] = ft::strdup(&path[0]);
	if ((req->getMethod() == "POST" || req->getPath().substr(req->getPath().length() - 4, 4) == ".php") &&
		!phpcgi.empty())
	{
		free(this->args[0]);
		this->args[0] = ft::strdup((phpcgi).c_str());
		if (!this->args[0])
			throw std::runtime_error("Error: malloc failed in Cgi::execute");
		this->args[1] = ft::strdup(path.c_str());
		if (!this->args[1])
			throw std::runtime_error("Error: malloc failed in Cgi::execute");
	}

	this->args[2] = NULL;
	this->setEnv(req, path, host, port, phpcgi);

	if ((in_fd = open("/tmp/webservin", O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
		throw std::runtime_error("Error: open failed in Cgi::execute");

	for (std::vector<std::string>::iterator it = req->getBody().begin(); it != req->getBody().end(); it++)
	{
		if ((write(in_fd, (*it).c_str(), (*it).length())) == -1)
		{
			close(in_fd);
			throw std::runtime_error("Error: failed writing to previously opened /tmp/webservin");
		}
	}
	if (close(in_fd) == -1)
		throw std::runtime_error("Error: close failed in Cgi::execute");

	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Error: fork failed in Cgi::execute");

	if (pid == 0)
	{
		if ((out_fd = open("/tmp/webservout", O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
			throw std::runtime_error("Error: open failed in Cgi::execute");
		if (dup2(out_fd, STDOUT_FILENO) == -1 || close(out_fd) == -1)
			throw std::runtime_error("Error: dup2/close failed in Cgi::execute");
		if ((in_fd = open("/tmp/webservin", O_RDONLY)) == -1)
			throw std::runtime_error("Error: open failed in Cgi::execute");
		if (dup2(in_fd, STDIN_FILENO) == -1 || close(in_fd) == -1)
			throw std::runtime_error("Error: dup2/close failed in Cgi::execute");
		execve(this->args[0], this->args, this->env);
		exit(0);
	}
	waitpid(0, &status, 0);
}

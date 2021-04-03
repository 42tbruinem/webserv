/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 21:00:20 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/03/29 14:24:27 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Utilities.hpp"
#include "WebServer.hpp"
#include "URI.hpp"

#include <iostream>
#include <exception>

int		main(int argc, char **argv)
{
	if (argc != 1 && argc != 2)
	{
		std::cerr << "Wrong number of argument to webserver!" << std::endl;
		return (1);
	}
	try
	{
		WebServer	webserv(argv[1]);
		webserv.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return 0;
}

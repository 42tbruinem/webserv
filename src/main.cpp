/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/01 21:00:20 by novan-ve      #+#    #+#                 */
/*   Updated: 2021/02/02 11:26:59 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int 	main() {

	Server	serv;

	serv.startListening();
	return 0;
}
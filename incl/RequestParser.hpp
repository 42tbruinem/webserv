/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/05 17:13:39 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/05 17:36:04 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*

https://refactoring.guru/design-patterns/factory-method/cpp/example#example-0

Creator = RequestParser

Product = Response
Response is an interface for a response
Based on the request it gets (and more specifically the Method), a concrete class inheriting from Response is returned
GET, POST, HEAD, etc..


*/

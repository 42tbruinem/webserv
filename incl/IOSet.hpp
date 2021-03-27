/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOSet.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:56:53 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 12:28:29 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOSET_HPP
# define IOSET_HPP

#include <Utilities.hpp>
#include <sys/select.h>

enum	e_IOSET
{
	SET_READ,
	SET_WRITE,
	SET_EXCEPT
};

class FDWrapper
{
	private:
		fd_set&		set;
		ssize_t&	nfd;
		int			fd;
		FDWrapper& operator = (const FDWrapper& other);
	public:
		FDWrapper(const FDWrapper& other);
		~FDWrapper();
		FDWrapper(fd_set& set, ssize_t& nfd, int fd);
		void	operator = (bool set);
		operator bool();
};

class FDSet
{
	private:
		fd_set		set;
		ssize_t&	nfd;
	public:
		FDSet(ssize_t& nfd);
		FDWrapper operator [] (ssize_t fd);
		~FDSet();
		FDSet& operator = (const FDSet& other);
		FDSet(const FDSet& other);
		operator fd_set*();
};

class IOSet
{
	private:
		ssize_t	nfd;
		FDSet	read;
		FDSet	write;
		FDSet	except;
	public:
		int		select();
		FDSet& operator [] (enum e_IOSET set);
		IOSet& operator = (const IOSet& other);
		IOSet(const IOSet& other);
		~IOSet();
		IOSet();
};

#endif

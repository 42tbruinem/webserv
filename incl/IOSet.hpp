/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOSet.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:56:53 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 11:50:26 by tbruinem      ########   odam.nl         */
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
		void	operator = (bool set);		//FDWrapper = bool
		operator bool();				//bool = FDWrapper
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

//Incorporate select() in IOSet wrapper

//Might want to make a pointer to indicate which FDSet is responsible for the highest numbered fd
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

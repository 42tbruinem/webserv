/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOSet.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:56:53 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 10:55:56 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <Utilities.hpp>
#include <sys/select.h>

enum	e_IO
{
	FD_READ,
	FD_WRITE,
	FD_EXCEPT
};

class FDWrapper
{
	private:
		fd_set&		set;
		int			fd;
		ssize_t&	nfd;
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
		FDWrapper operator [] (int fd);
		~FDSet();
		FDSet& operator = (const FDSet& other);
		FDSet(const FDSet& other);
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
		FDSet& operator [] (enum e_IO set);
		IOSet& operator = (const IOSet& other);
		IOSet(const IOSet& other);
		~IOSet();
		IOSet();
};

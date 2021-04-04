/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOSet.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 10:19:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 13:31:53 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "IOSet.hpp"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

//-----------------------------------------------FDWrapper-----------------------------------------------

FDWrapper::FDWrapper(fd_set& set, int fd) : set(set),fd(fd) {}

FDWrapper::FDWrapper(const FDWrapper& other) : set(other.set), fd(other.fd) {}

FDWrapper::~FDWrapper() {}

void	FDWrapper::operator = (bool set)
{
	if (set && !FD_ISSET(this->fd, &this->set))
		FD_SET(this->fd, &this->set);
	else if (!set && FD_ISSET(this->fd, &this->set))
		FD_CLR(this->fd, &this->set);
}

FDWrapper::operator bool()
{
	return ((bool)FD_ISSET(this->fd, &this->set));
}

//-----------------------------------------------FDSet-----------------------------------------------

FDSet::FDSet()
{
	FD_ZERO(&this->set);
}

FDSet::FDSet(const FDSet& other) : set(other.set) {}

// only increase nfd, we cant be sure that this FDSet
// is the reason that the nfd is high
// so we cant decrease nfd
FDSet& FDSet::operator = (const FDSet& other)
{
	if (this != &other)
	{
		this->set = other.set;
	}
	return (*this);
}

FDSet::~FDSet() {}

FDWrapper FDSet::operator [] (ssize_t fd)
{
	return (FDWrapper(this->set, fd));
}

FDSet::operator fd_set* ()
{
	return (&this->set);
}

//-----------------------------------------------IOSet-----------------------------------------------

IOSet::IOSet() : read(), write(), except() {}

IOSet::~IOSet() {}

IOSet::IOSet(const IOSet& other) : read(other.read), write(other.write), except(other.except) {}

FDSet&	IOSet::operator [] (enum e_IOSET set)
{
	if (set == SET_READ)
		return (this->read);
	if (set == SET_WRITE)
		return (this->write);
	return (this->except);
}

int	IOSet::select()
{
	return (::select(FD_SETSIZE, read, write, except, NULL));
}

IOSet& IOSet::operator = (const IOSet& other)
{
	if (this != &other)
	{
		this->read = other.read;
		this->write = other.write;
		this->except = other.except;
	}
	return (*this);
}

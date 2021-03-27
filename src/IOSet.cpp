/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOSet.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 10:19:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/03/27 10:57:18 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "IOSet.hpp"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

//FDWrapper

FDWrapper(fd_set& set, ssize_t& nfd, int fd) : set(set), nfd(nfd), fd(fd) {}

FDWrapper(const FDWrapper& other) : set(other.set), nfd(other.nfd), fd(other.fd) {}

FDWrapper::~FDWrapper() {}

void	FDWrapper::operator = (bool set)
{
	if (set && !FD_ISSET(this->fd, this->set))
	{
		FD_SET(this->fd, this->set);
		if (this->fd + 1 > this->nfd)
			this->nfd = this->fd + 1;
	}
	else if (!set && FD_ISSET(this->fd, this->set))
	{
		FD_CLR(this->fd, this->set);
		if (this->fd + 1 == this->nfd)
			this->nfd--;
	}
}

FDWrapper::operator bool()
{
	return ((bool)FD_ISSET(this->fd, this->set));
}

//FDSet

FDSet::FDSet(ssize_t& nfd) : set()
{
	FD_ZERO(this->set);
}

FDSet::FDSet(const FDSet& other) : nfd(other.nfd), set(other.set) {}

// only increase nfd, we cant be sure that this FDSet
// is the reason that the nfd is high
// so we cant decrease nfd
FDSet& FDSet::operator = (const FDSet& other)
{
	if (this != &other)
	{
		if (other.nfd > this->nfd)
			this->nfd = other.nfd;
		this->set = other.set;
	}
	return (*this);
}

FDSet::~FDSet() {}

FDWrapper operator [] (int fd)
{
	return (FDWrapper(this->set, nfd, fd));
}

//IOSet

IOSet::IOSet() : nfd(0), read(nfd), write(nfd), except(nfd) {}

IOSet::~IOSet() {}

IOSet::IOSet(const IOSet& other) : nfd(other.nfd), read(nfd), write(nfd), except(nfd) {}

FDSet&	IOSet::operator [] (enum e_IO set)
{
	if (set == FD_READ)
		return (this->read);
	if (set == FD_WRITE)
		return (this->write);
	return (this->except);
}

IOSet& IOSet::operator = (const IOSet& other)
{
	if (this != &other)
	{
		this->nfd = other.nfd;
		this->read = other.read;
		this->write = other.write;
		this->except = other.except;
	}
	return (*this);
}

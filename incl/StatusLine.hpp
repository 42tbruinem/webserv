/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   StatusLine.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/04 18:02:41 by tbruinem      #+#    #+#                 */
/*   Updated: 2021/04/04 19:43:10 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_LINE_HPP
# define STATUS_LINE_HPP

#include <string>

enum	e_status_code_tier
{
	ST_NOTVALID,
	ST_INFORMATIONAL,
	ST_SUCCES,
	ST_REDIRECTION,
	ST_CLIENT_ERROR,
	ST_SERVER_ERROR
};

struct StatusLine
{
	public:
		StatusLine();
		StatusLine(size_t code);
		StatusLine(const StatusLine& other);
		~StatusLine();
		StatusLine& operator = (const StatusLine& other);

		void				operator = (size_t code);
		void				operator = (e_status_code_tier tier);
		e_status_code_tier	tier(void) const;
		operator size_t() const;
		operator std::string(void) const;

	private:
		size_t code;
};

#endif

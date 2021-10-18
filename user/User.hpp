/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:45:02 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/13 15:56:14 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __USER_HPP
#define __USER_HPP


#include <string>
#include <list>

class Channel;

typedef struct	user_mode_s
{
	bool	a;
	bool	i;
	bool	w;
	bool	r;
	bool	small_o;
	bool	big_o;
	bool	s;
	
}				user_mode_t;

class User 
{
	private:
		std::list<Channel *>	_channels;
		std::string				_user_name;
		user_mode_t				_mode;
		std::string				_real_name;
		std::string				_nick_name;
		int						_sock_fd;

	public:
		User();
		~User();
};

#endif 

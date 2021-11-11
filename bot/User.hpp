/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:00:50 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 22:16:06 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <map>

class Game;

class User
{
	std::string							_nick;
	std::string							_buffer_cmd;
	std::map<std::string, std::string> 	_table;
	public:

	User( std::string const & _nick );
	virtual ~User();

	std::string const &					getNick() const;
	std::string const &					getBufferCmd() const;
	std::map<std::string, std::string>	getTabla() const;

	void					setBufferCmd( std::string const & buffer_cmd );
	void					setTabla(std::string const & coo );
	

};


#endif 
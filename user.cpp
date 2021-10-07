/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 20:39:45 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/07 20:50:27 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

User::User()
{
	init_modes();
}

User::~User() { }

std::string User::getUserName() const { return (this->_username); }

std::string User::getNick() const { return (this->_nick); }

bool User::getmode(char mode) const
{
	switch (mode)
	{
		case 'a':
			return (this->_modes.a);
		case 'i':
            return (this->_modes.i);
		case 'w':
            return (this->_modes.w);
		case 'r':
            return (this->_modes.r);
		case 'o':
            return (this->_modes.small_o);
		case 'O':
            return (this->_modes.big_o);
		case 's':
            return (this->_modes.s);
		default:
			return (0);
	}
}

std::string User::setUserName(std::string name) { this->_username = name; }

std::string User::setNick(std::string nick) { this->_nick = nick; }



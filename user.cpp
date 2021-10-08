/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 20:39:45 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 16:07:36 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// Constructor && destructor

User::User() 
{
	init_modes();
}

User::~User() { }

// Getters

std::string User::getUserName() const { return (this->_username); }

std::string User::getNick() const { return (this->_nick); }

bool	User::getmode(char mode) const
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

int		User::getsockfd()  const { return (this->_sock_fd); }

// Setters

void	User::setUserName(std::string name) { this->_username = name; }

void	User::setNick(std::string nick) { this->_nick = nick; }

void	User::setmode(char mode, bool state)
{
	switch (mode)
	{
		case 'a':
			(this->_modes.a = state);
		case 'i':
            (this->_modes.i = state);
		case 'w':
            (this->_modes.w = state);
		case 'r':
            (this->_modes.r = state);
		case 'o':
            (this->_modes.small_o = state);
		case 'O':
            (this->_modes.big_o = state);
		case 's':
            (this->_modes.s = state);
		default:
			break ;
	} 
}

// Overloads

bool	User::operator==(User & obj)
{
	
}

// Other functions

void	User::init_modes()
{
	this->_modes.a = 0;
	this->_modes.i = 0;
	this->_modes.w = 0;
	this->_modes.r = 0;
	this->_modes.small_o = 0;
	this->_modes.big_o = 0;
	this->_modes.s = 0;
}


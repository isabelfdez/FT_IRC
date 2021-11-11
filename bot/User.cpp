/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:10:18 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 22:17:03 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( std::string const & nick )
	: _nick(nick)
{
	this->_buffer_cmd = "";

}

User::~User()
{

}


std::string const & User::getBufferCmd() const { return this->_buffer_cmd; }

void	User::setBufferCmd( std::string const & buffer_cmd )
{
	this->_buffer_cmd = buffer_cmd;
}

std::map<std::string, std::string> User::getTabla () const { return this->_table; }

std::string const & User::getNick() const
{
	return this->_nick;
}

void User::setTabla(std::string const & coo )
{
	this->_table[coo] = "x";
}
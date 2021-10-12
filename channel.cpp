/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:49:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 16:39:34 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Constructor && destructor

Channel::Channel(std::string name) : _name(name) { this->_maxusers = MAX_USERS; }

Channel::~Channel() {  }

// Getters

std::string Channel::getName() const { return(this->_name); }

std::list<User *>	Channel::getUsers() const
{
	return (this->_users);
}

// Setters

void	Channel::setName(std::string name) { this->_name = name; }

// Overloads

bool	Channel::operator==(Channel & obj)
{
	if (this->_name != obj._name)
		return (0);
	return (1);
}

// Other functions

void	Channel::deleteUser(User & user)
{
	for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
		if (**it == user) // Esto no se si esta bien
		{
			this->_users.erase(it);
			return ;	
		}
	}
}

void	Channel::addUser(User * user)
{
	this->_users.push_back(user);
}

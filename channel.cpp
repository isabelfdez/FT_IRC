/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:49:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/05 00:19:31 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

// Constructor && destructor

Channel::Channel(std::string const & name, User *user): _banned(), _name(name), _users(), _operators()
{
	this->_maxusers = MAX_USERS;
	this->_isfull = false;
	this->_invite = false;
	this->_operators.push_back(user);
}

Channel::~Channel() {  }

// Getters

std::string	Channel::getTopic() { return this->_topic; }

std::string Channel::getName() const { return(this->_name); }

size_t Channel::getNumUser() const { return  this->_users.size(); }

std::list<User *> & Channel::getUsers() 
{
	return (this->_users);
}

bool				Channel::isInvite() {return (this->_invite);}

bool				Channel::isInvited(std::string user)
{
	for (std::list<User *>::iterator it = this->_invites.begin(); it != this->_invites.end(); it++)
	{
		if ((*it)->getNick() == user)
			return (true);
	}
	return (false);
}

std::list<User *> & Channel::getBanned() 
{
	return (this->_banned);
}

bool	Channel::getIsFull() const { return(this->_isfull); }

bool	Channel::isOp(User * user)
{
	for (std::list<User *>::iterator it = this->_operators.begin(); it != this->_operators.end(); it++)
	{
		if ((*it)->getNick() == user->getNick())
			return (true);
	}
	return (false);
}

bool	Channel::isBanned(User * user)
{
	for (std::list<User *>::iterator it = this->_banned.begin(); it != this->_banned.end(); it++)
	{
		if ((*it)->getNick() == user->getNick())
			return (true);
	}
	return (false);
}

User    *Channel::getUser(std::string user)
{
    for (std::list<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->getNick() == user)
            return ((*it));
    }
    return (nullptr);
}

bool    Channel::isUser(std::string user)
{
    for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        if ((*it)->getNick() == user)
            return (true);
    }
    return (false);
}



std::string	Channel::userList()
{
	std::string	s;

	for (std::list<User*>::iterator it = this->_users.begin(); it != _users.end(); it++)
	{
		if (isOp(*it))
			s.append("@");
		else
			s.append("+");
		s.append((*it)->getNick());
		s.append(" ");
	}

	return (s);
}


// Setters

void	Channel::setName(std::string const & name) { this->_name = name; }

void    Channel::setOp(User * user)
{
    this->_operators.push_back(user);
}

void	Channel::ban(User * user)
{
	this->_banned.push_back(user);
	this->deleteUser(user);
}

void	Channel::banOff(User * user)
{
	for (std::list<User*>::iterator it = _banned.begin(); it != _banned.end(); it++)
	{
		if (user->getNick() == (*it)->getNick())
		{
			_banned.erase(it);
			return;
		}
	}
}

void	Channel::setTopic(std::string topic) { this->_topic = topic; }

void	Channel::setInvite(bool set) { this->_invite = set; }

void	Channel::pushInvite(User * user)
{
	this->_invites.push_back(user);
}

void    Channel::setOpOff(std::string user, User * usr)
{
	(void)usr;
	for (std::list<User*>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (user == (*it)->getNick())
		{
			_operators.erase(it);
			if (!this->_operators.size())
			{
				this->_operators.push_back(*(this->_users.begin()));
				// (*this->_users.begin())->setAnswer(" :You are now Channel Operator");
				// send_reply("", " :You are now Channel Operator", usr);
			}
			return;
		}
	}
}

// Overloads

bool	Channel::operator==(Channel & obj)
{
	if (this->_name != obj._name)
		return (0);
	return (1);
}

// Other functions

bool	Channel::deleteUser(User * user)
{
	std::string s;
	for (std::list<User *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
		if ((*it)->getNick() == user->getNick())
		{
			this->_users.erase(it);
			for (std::list<User *>::iterator it2 = this->_operators.begin(); it2 != this->_operators.end(); ++it2)
			{
				if ((*it2)->getNick() == user->getNick())
				{
					this->_operators.erase(it2);
					if (!this->_operators.size())
						this->_operators.push_back(*(this->_users.begin()));
					break ;
				}
			}
			return true;
		}
	}
	 return false;
}

void	Channel::addUser(User * user)
{
	this->_users.push_back(user);
	if (_users.size() >= this->_maxusers)
		this->_isfull = true;
}

// krios-fu
void	Channel::sendMsgChannelBlock( std::string msg , int fd)
{
	for (std::list<User*>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if ( (*it)->getsockfd() != fd && !(*it)->getIsSendMsg() )
		{
			send((*it)->getsockfd(), msg.c_str(), msg.length(), 0);
			(*it)->setIsSendMsg( true );
		}
	}
}

void	Channel::sendMsgChannel( std::string msg , int fd)
{
	for (std::list<User*>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if ( (*it)->getsockfd() != fd  )
			send((*it)->getsockfd(), msg.c_str(), msg.length(), 0);
	}
}


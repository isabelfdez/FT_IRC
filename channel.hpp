/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:42:54 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/24 04:52:06 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include "user.hpp"
# include <iostream>

# define MAX_USERS 50

class Channel;
class User;

class Channel
{
	private:
		std::list<User *>	_banned;
		std::string			_name;
		std::string			_topic;
		std::list<User *>	_users;
		std::list<User *>	_operators;
		std::list<User *>	_invites;
		unsigned			_maxusers;
		bool				_isfull;
		bool				_invite;

	public:
		// Construtor && destructor
		Channel(std::string name, User *user);
		~Channel();

		// Getters
		std::string			getName() const;
		std::list<User *>&	getUsers();
		std::list<User *>&	getBanned();
		std::string			getTopic();
		bool				getIsFull() const;
		bool				isOp(User * user);
		bool				isBanned(User * user);
		bool    			isInvited(std::string user);
		bool    			isUser(std::string user);
		bool				isInvite();
		User    			*getUser(std::string user);
		

		std::string			userList();

		// Setters
		void				banOff(User * user);
		void				ban(User *);
		void				setName(std::string name);
		void    			setOp(User * user);
		void				setOpOff(std::string user, User * usr);
		void				setInvite(bool);
		void				pushInvite(User * user);
		void				setTopic(std::string topic);

		
		// Overload

		// Other functions
		void				deleteUser(User * user);
		void				addUser(User * user);
		void				sendMsgChannel(std::string msg);
		//overload para no enviar el mensaje al emisor :) by krios-fu
		bool				operator==(Channel & obj);
		void				sendMsgChannel( std::string msg , int fd) ;

	
};

#endif
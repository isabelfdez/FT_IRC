/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:42:54 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/16 19:28:06 by krios-fu         ###   ########.fr       */
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

typedef std::list<std::string>::iterator	list_str_it;
typedef std::list<User *>::iterator			list_user_it;

class Channel
{
	private:
		std::list<std::string>	_banned;
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
		Channel(std::string const & name, User *user);
		~Channel();

		// Getters
		std::string					getName() const;
		std::list<User *>&			getUsers();
		std::list<std::string>&		getBanned();
		std::string					getTopic();
		bool						getIsFull() const;
		bool						isOp(User * user);
		bool						isBanned(std::string mask);
		bool    					isInvited(std::string user);
		bool    					isUser(std::string user);
		bool						isInvite();
		User    					*getUser(std::string user);
		size_t 						getNumUser() const ;
			std::string				userList2(std::string nick);

		

		std::string			userList();


		// Setters
		bool				banOff(std::string mask);
		void				inviteOff(User * user);
		void				ban(std::string mask);
		void				setName(std::string const & name);
		void    			setOp(User * user);
		void				setOpOff(std::string nick);
		void				setInvite(bool);
		void				pushInvite(User * user);
		void				setTopic(std::string topic);

		
		// Overload

		// Other functions
		void				checkBans(User * usr);
		std::string			showModes();
		bool				deleteUser(User * user);
		void				addUser(User * user);
		//overload para no enviar el mensaje al emisor :) by krios-fu
		bool				operator==(Channel & obj);
		void				sendMsgChannel( std::string msg , int fd) ;
		void				sendMsgChannelBlock( std::string msg , int fd);


	
};

#endif
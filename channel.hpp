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
		std::list<User *>	_users;
		std::list<User *>	_operators;
		unsigned			_maxusers;
		bool				_isfull;

	public:
		// Construtor && destructor
		Channel(std::string const & name, User *user);
		~Channel();

		// Getters
		std::string			getName() const;
		std::list<User *>&	getUsers();
		std::list<User *>&	getBanned();
		bool				getIsFull() const;
		bool				isOp(User * user);
		bool				isBanned(User * user);
		bool    			isUser(std::string user);
		User    			*getUser(std::string user);
		

		std::string			userList();

		// Setters
		void				banOff(User * user);
		void				ban(User *);
		void				setName(std::string const & name);
		void    			setOp(User * user);
		void				setOpOff(std::string user, User * usr);


		
		// Overload

		// Other functions
		void				deleteUser(User * user);
		void				addUser(User * user);
		//overload para no enviar el mensaje al emisor :) by krios-fu
		bool				operator==(Channel & obj);
		void				sendMsgChannel( std::string msg , int fd) ;

	
};

#endif
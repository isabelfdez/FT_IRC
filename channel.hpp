/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:42:54 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/23 15:50:44 by krios-fu         ###   ########.fr       */
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
		std::string			_name;
		std::list<User *>	_users;
		std::list<User *>	_operators;
		unsigned			_maxusers;
		bool				_isfull;

	public:
		// Construtor && destructor
		Channel(std::string name, User *user);
		~Channel();

		// Getters
		std::string			getName() const;
		std::list<User *>	getUsers() const;
		bool				getIsFull() const;
		bool				isOp(User * user);
		std::string			userList();

		// Setters
		void				setName(std::string name);
		
		// Overload
		bool				operator==(Channel & obj);

		// Other functions
		void				deleteUser(User * user);
		void				addUser(User * user);
		void				sendMsgChannel(std::string msg);
		//overload para no enviar el mensaje al emisor :) by krios-fu
		void				sendMsgChannel( std::string msg , int fd) ;

	
};

#endif
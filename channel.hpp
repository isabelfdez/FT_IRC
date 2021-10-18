/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:42:54 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/18 14:28:42 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include "User.hpp"

# define MAX_USERS 50

class Channel;
class User;

class Channel
{
	private:
		std::string			_name;
		std::list<User *>	_users;
		int					_maxusers;

	public:
		// Construtor && destructor
		Channel(std::string name);
		~Channel();

		// Getters
		std::string			getName() const;
		std::list<User *>	getUsers() const;

		// Setters
		void				setName(std::string name);
		
		// Overload
		bool				operator==(Channel & obj);

		// Other functions
		void				deleteUser(User & user);
		void				addUser(User * user);
	
};

#endif
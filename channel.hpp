/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:42:54 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 15:55:45 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include "user.hpp"

# define MAX_USERS 50

class Channel
{
	private:
		std::list<User *>	_users;
		int					_maxusers;

	public:
		// Construtor && destructor
		Channel();
		~Channel();

		// Getters

		// Setters

		// Other functions
		void				deleteUser();
	
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 12:41:29 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 18:48:49 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include "user.hpp"
#include "channel.hpp"


#define MAXFD 100

class Server
{
	private:
		fd_set              _socks;
		int                 _sock;
		int                 _highSock;
		int                 _connectList[MAXFD];
		struct sockaddr_in  _server_address;
		std::list<User*>    _users;
		std::list<Channel*> _channels;

	public:
		// Constructor && destructor
		Server(int port);
		~Server();

		// Getters
		fd_set &			getSocks();
		int	&				getSock();
		int					getHighSock() const;
		struct sockaddr_in &getStruct();
		int	&				getfds(int index);

		// Setters
		void				setSock(int a);
		void				setHighSock(int a);


};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:15:34 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 21:53:19 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <stdio.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h>
# include <string.h>
# include <arpa/inet.h>
# include <string>
# include <iostream>
# include "User.hpp"
# include "utils.hpp"
# include <fcntl.h>
# include <map>
# include <unistd.h>


class Bot 
{
	int								_sock;
	struct sockaddr_in				_addr;
	std::string						_nick;
	struct timeval					_time_out;
	int								_num_read_sock;
	int								_highsock;
	fd_set							_writes;
	fd_set							_reads;
	std::map<std::string, User*>	_users;



	
	
	public:

	Bot(std::string const & nick, std::string const & IP,int const & port );
	virtual ~Bot();
	
	std::string const &	getNick() const;
	int const &			getSocket() const ;
	int const &			getNumReadSock( void ) const;
	fd_set	const &		getWrites() const;
	fd_set	const &		getReads() const ;


	void				setNumReadSock( void );
	void				build_select_list( void );
	void				attendServer();
	void 				read_serve();
	void 				parse( std::string const & buffer );
	void				ticTacToe( std::string const & message, std::string const & nick );


	
};

#endif
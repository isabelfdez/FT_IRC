/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:15:34 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/18 17:20:18 by krios-fu         ###   ########.fr       */
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
# include <deque>
#include <fstream>
#include <sstream>
       #include <sys/types.h>
       #include <sys/stat.h>
	    #include <netdb.h>
		#include <limits.h>


class Bot 
{
	int								_sock;
	int								_sock_c;
	struct sockaddr_in				_addr;
	struct sockaddr_in				_addr_c;

	struct sockaddr_in				_send;
	std::string						_nick;
	struct timeval					_time_out;
	int								_num_read_sock;
	int								_num_read_sock_c;

	int								_highsock;
	fd_set							_writes;
	fd_set							_reads;
	fd_set							_writes_c;
	fd_set							_reads_c;
	std::deque<User *>				_send_message;

	std::map<std::string, User*>	_users;



	
	
	public:

	Bot(std::string const & nick, std::string const & IP,int const & port );
	virtual ~Bot();
	
	std::string const &	getNick() const;
	int const &			getSocket() const ;
	int const &			getNumReadSock( void ) const;
	int const &			getNumReadSockC( void ) const;

	fd_set	const &		getWrites() const;
	fd_set	const &		getReads() const ;
	struct sockaddr_in const &getAddress();


	void				setNumReadSock( void );
	void				setNumReadSockC( void );

	void				build_select_list( void );
	void				attendServer();
	void 				read_serve();
	void 				parse( std::string const & buffer );
	void				ticTacToe( std::string const & message, std::string const & nick );
	void				sendRequest(User *usr);
	void				deleteDequeUser ( User * usr );
	bool				isAnswerUser( User *usr );
	void				send_message(std::string _message, User * usr);
	void				draw(User *usr);
	void				sendFile( std::string nick  );
	void				recv_file( std::vector<std::string> token );

	char const  *		getIp( unsigned long ip ) ;
	unsigned long 		getIpLong();







	
};

#endif



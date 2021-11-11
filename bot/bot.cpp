/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:33:37 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 02:46:38 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(std::string const & nick, std::string const & IP,int const & port )
	: _nick(nick)
{
	this->_sock = 0;
	if ((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	this->_highsock = this->_sock;
	memset(&this->_addr, 0, sizeof(this->_addr));

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	
	if (inet_pton(AF_INET, IP.c_str(), &this->_addr.sin_addr) <= 0)
	{
		perror("INET_PTON");
		exit(EXIT_FAILURE);
	}
	if (connect(this->_sock, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) < 0 )
	{
		perror("Connect");
		exit(EXIT_FAILURE);
	}
}
Bot::~Bot()
{
	
}

void Bot::setNumReadSock( void )
{
	this->_time_out.tv_sec = 30;
	this->_time_out.tv_usec = 0;
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_reads, &this->_writes, (fd_set *) 0 , &this->_time_out);
}

void Bot::build_select_list( void )
{
	FD_ZERO( &this->_reads );
	FD_ZERO( &this->_writes );
	FD_SET(this->_sock, &this->_reads);
	FD_SET(this->_sock, &this->_writes);
}

void Bot::attendServer()
{
	if (FD_ISSET(this->_sock, &this->_reads) )
	{	
		int byte;
		char buffer[513];
		while ((byte = recv(this->getSocket(), buffer, 512,0)) > 0 )
		{
			buffer[byte] = '\0';

			std::cout << buffer;
		}
		
	}
	if (FD_ISSET(this->_sock, &this->_writes) )
	{
		
	}

}

int const & Bot::getSocket() const 
{
	return this->_sock;
}
int const &  Bot::getNumReadSock( void ) const
{
	return this->_num_read_sock;
}

fd_set	const & Bot::getWrites() const 
{
	return this->_writes;
}

fd_set	const & Bot::getReads() const 
{
	return this->_reads;
}


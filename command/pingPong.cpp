/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingPong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:50:59 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/28 14:57:53 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

static std::string generatePing()
{
	char ping[13];
    srand( time( NULL ) );
    for( size_t i = 0; i <= 11; i++ )
	{
        ping[ i ] = 33 + rand() % ( 126 - 33 );
	}
	ping[ 12 ] = '\0';
	return static_cast<std::string>( ping );
}


void	Server::sendPing()
{
	typedef std::map<int , User *>::iterator iterator;
	User * usr;
	std::string ping = "PING : ";
	
	iterator start = this->_fd_users.begin();
	iterator end = this->_fd_users.end();
	for ( ;  start != end; ++start )
	{
		usr = this->_fd_users[ start->first ] ;
		if ( ( getTime() - usr->getLastTime() ) > usr->getTimePing() ) 
		{
			if ( ( usr->getPingStatus() || !usr->getRegistered() )
				&& ( getTime() - usr->getLastTime() ) > ( usr->getTimePing() + 30000 ) ) // si a los 30 segundo ha devuelto el pong 
			{
				if ( usr->getPingStatus() )
					send_reply("ERROR :Closing link:", " [Ping timeout]", usr);
				else
					send_reply("ERROR :Closing link:", " [Registration timeout]", usr);
				this->deleteUser( start->first );
				return ;
			}
			else if ( usr->getPingStatus() == false
				&& usr->getRegistered() )
			{
				usr->setPing( generatePing() );
				ping += usr->getPing() + "\n";
				usr->setPingStatus( true );
				send( usr->getsockfd(), ping.c_str(), ping.length(), 0);
				std::cout << "\r";
				displayLog("Ping send", "", usr);

			if ( usr->getTimePing() == 0)
			{
				send_reply("396", "ft_irc.com :is now your display host", usr);
				send_reply("MODE", ":+xwT", usr);
			}
			}
		}
	}
}

void	Server::pong_command( int fd, char *buffer)
{
	User *usr = this->_fd_users[fd];
	buffer  = buffer  + 4;
	while (*buffer  == ' ')
		buffer ++;
	if (*buffer == ':')
		buffer++;
	std::vector<std::string> token = split( buffer, ' ' );
	if ( token[0] == usr ->getPing() )
	{
		if (this->_fd_users[fd]->getTimePing() == 0)
			this->welcome( fd );
		usr ->setPingStatus( false );
		usr ->setTimePing( 120000 );
		std::cout << "\r";
		displayLog("Ping received", "", usr);
	}
	else
	{
		send_reply("ERROR :Closing link:", " [Incorrect ping reply for registration]", usr);
		this->deleteUser( fd );
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingPong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:50:59 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/18 18:47:02 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

static std::string generatePing()
{
	char ping[13];
	char	c;
    srand( time( NULL ) );
    for( size_t i = 0; i <= 11; i++ )
	{
		c = 33 + rand() % ( 126 - 33 );
		if ( c == ':')
			i--;
		else
			ping[ i ] = c;
	}
	ping[ 12 ] = '\0';
	return static_cast<std::string>( ping );
}


void	Server::sendPing()
{
	typedef std::map<int , User *>::iterator iterator;
	User * usr;
	std::string ping = "PING :";

	iterator start = this->_fd_users.begin();
	iterator end = this->_fd_users.end();
	for ( ;  start != end; ++start )
	{
		usr = this->_fd_users[ start->first ] ;
		if ( ( getTime() - usr->getLastTime() ) > usr->getTimePing() )
		{
			if ( ( usr->getPingStatus() || !usr->getRegistered() )
				&& ( getTime() - usr->getLastTime() ) > ( usr->getTimePing() + 30000 ) )
			{
				if ( usr->getPingStatus() )
					send_reply("ERROR :Closing link:", " [Ping timeout]", usr);
				else
					send_reply("ERROR :Closing link:", " [Registration timeout]", usr);
				this->deleteUser( usr , "Ping timeout: 120 seconds");
				return ;
			}
			else if (!usr->getPingStatus()
					 && usr->getRegistered() )
			{
				usr->setPing( generatePing() );
				ping += usr->getPing() + "\n";
				usr->setPingStatus( true );
				send( usr->getsockfd(), ping.c_str(), ping.length(), 0);
				std::cout << "\r";
				displayLog("Ping sent", "", usr);

				if ( usr->getTimePing() == 0)
				{
					send_reply("396", " " +  usr->getIp() + " :is now your displayed host", usr);
					send_reply("MODE", ((usr->getmode('i')) ? " :i" : ":") , usr);

				}
			}
		}
	}
}

void	Server::pong_command( std::vector<std::string> const & parse, User *usr )
{

	if ( parse.size() > 1 && parse[1] == usr ->getPing() )
	{
		if (usr->getTimePing() == 0)
			this->welcome( usr->getsockfd() );
		usr ->setPingStatus( false );
		usr ->setTimePing( 120000 );
		std::cout << "\r";
		displayLog("Ping received", "", usr);
	}
	else
	{
		send_reply("ERROR :Closing link:", " [Incorrect ping reply for registration]", usr);
		this->deleteUser( usr, "[Incorrect ping reply for registration]");
	}
}
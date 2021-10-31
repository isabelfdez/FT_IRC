/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:28:55 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/31 23:20:17 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void Server::list_command( char *buffer, int fd)
{
	typedef std::map<std::string ,Channel *>::iterator		it_channel;
	Channel			*channel;
	std::string		message;
	size_t			pos;
	User			*usr;

	buffer  = buffer  + 4;
	while (*buffer  == ' ')
		buffer ++;
	if (*buffer == ':')
		buffer++;

	usr = this->_fd_users.at( fd );
	std::vector<std::string> token = split(buffer, ',');

	if ( token.size() <= 0 )
	{
		it_channel start = this->_name_channel.begin();
		it_channel end = this->_name_channel.end();

		for (; start != end ; ++start )
		{
			channel = start->second;
			message = " " + channel->getName() + " " + std::to_string( channel->getNumUser() )	 + " :" + channel->getTopic();
			send_reply(RPL_LIST, message, usr);
		}
	}
	for (size_t i = 0; i < token.size(); i++)
	{
		pos = token[i].find(' ');
		if ( pos != std::string::npos )
			token[i] = token[i].substr(0, pos);
		channel = this->_name_channel[ token[i] ];
		if ( channel )
		{
			 message = " " + channel->getName() + " " + std::to_string(channel->getNumUser())	 + " : " + channel->getTopic();
			 send_reply(RPL_LIST, message, usr);
		}
	}
	send_reply(RPL_LISTEND, " :End of channel list", usr);
}
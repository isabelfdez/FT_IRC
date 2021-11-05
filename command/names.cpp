/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 21:18:10 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/05 18:59:32 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"


void Server::names_command(char *buffer , int fd )
{
	std::string		message;
	size_t			pos;
	Channel			*channel;
	User			*usr;
	bool			flags;

	flags = false;

	usr = this->_fd_users.at( fd );
	std::vector<std::string> token = split(buffer, ',');

	for( size_t i = 0; i < token.size(); i++ )
	{
		pos = token[i].find(' ');
		if ( pos != std::string::npos )
			token[i] = token[i].substr(0, pos);
		channel = this->_name_channel[ token[i] ];
		if ( channel )
		{
				message = " = " + channel->getName() + " :" + channel->userList();
				send_reply(RPL_NAMREPLY, message, usr);
				send_reply(RPL_ENDOFNAMES	," " + channel->getName() + " :End of /NAMES list", usr);
				flags = true;
		}
	}
	if ( !flags )
		send_reply(RPL_ENDOFNAMES	, " :End of /NAMES list", usr);
}
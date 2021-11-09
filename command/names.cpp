/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 21:18:10 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/08 16:58:17 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"


void Server::names_command( std::vector<std::string> const & parse, User *usr )
{
	std::string		message;
	Channel			*channel;
	User			*usr;
	bool			flags;

	flags = false;

	for( size_t i = 0; i < parse.size(); i++ )
	{
		channel = this->_name_channel[ parse[i] ];
		if ( channel )
		{
				message = " = " + channel->getName() + " :" + channel->userList();
				send_reply(RPL_NAMREPLY, message, usr);
				send_reply(RPL_ENDOFNAMES," " + channel->getName() + " :End of /NAMES list", usr);
				flags = true;
		}
	}
	if ( !flags )
		send_reply(RPL_ENDOFNAMES	, " :End of /NAMES list", usr);
}
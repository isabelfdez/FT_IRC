/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:28:55 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/08 17:17:07 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void Server::list_command( std::vector<std::string> const & parse, User *usr )
{
	Channel			*channel;
	std::string		message;
	User			*usr;


	std::vector<std::string> token = split(parse[0], ',');

	if ( token.size() <= 0 )
	{
		map_channel_it start = this->_name_channel.begin();
		map_channel_it end = this->_name_channel.end();

		for (; start != end ; ++start )
		{
			channel = start->second;
			message = " " + channel->getName() + " " + std::to_string( channel->getNumUser() )	 + " :" + channel->getTopic();
			send_reply(RPL_LIST, message, usr);
		}
	}
	
	send_reply(RPL_LISTEND, " :End of channel list", usr);
}
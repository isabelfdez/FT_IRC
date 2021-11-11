/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:04:46 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 16:41:14 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void Server::who_command( std::vector<std::string> const & parse, User *usr )
{
	std::vector<std::string>	token;
	Channel						*channel;
	bool						flags;

	flags = false;

	if( parse.size() > 1 )
		token = split(parse[1], ',');
	
	for( size_t i = 0; i < token.size(); i++ )
	{
		channel = this->_name_channel[ token[i] ];
		if ( channel )
		{
			std::vector<std::string> users = split( channel->userList(), ' ' );
			
			for (size_t i = 0; i < users.size(); i++)
			{
				User *usr_info = channel->getUser( static_cast<std::string>(&users[i][1]));
				if (usr_info)
				{
					std::string	message;
					std::cout << "WHO " << i <<  " *\n";
					message.append(" ");
					message.append(channel->getName());
					message.append(" ");
					message.append( usr_info->getUserName() );
					message.append(" ");
					message.append( usr_info->getIp() );
					message.append(" ");
					message.append("ft_irc.com");
					message.append(" ");
					message.append( usr_info->getNick() );
					message.append(" ");
					message.append("H");
					if (channel->isOp(usr_info))
						message.append("@");
					else
						message.append("+");
					message.append(" ");
					message.append(":0");
					message.append(" ");
					message.append( usr_info->getRealName() );
				
					send_reply(RPL_WHOREPLY, message, usr);
					send_reply(RPL_ENDOFWHO," " + channel->getName() + " :End of WHO list.", usr);
					flags = true;
				}
			}
			
		}
	}
	if( !flags )
		send_reply(RPL_ENDOFWHO," :End of WHO list.", usr);

}
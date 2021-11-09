/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:17:03 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/09 18:50:15 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::notice_command( std::vector<std::string> const & parse, User *usr )
{
	
	list_user_it	start_usr = this->_connected_users.begin();
	list_user_it	end_usr = this->_connected_users.end();
	User			*usr_dest;
	Channel			*chnl_dest;
	std::string		dest = parse[1];
	std::string		message;
	bool			dest_user_b, dest_chnl_b;

	dest_user_b = false;
	dest_chnl_b = false;
	

	for (; start_usr != end_usr ; ++start_usr )
	{
		if ( ft_toupper( (*start_usr)->getNick() ) == ft_toupper(dest) )
		{
			usr_dest = *start_usr;
			dest_user_b = true;
			break ;
		}
	}

	if( this->_name_channel.count(dest) > 0 )
	{
		dest_chnl_b = true;
		chnl_dest =  this->_name_channel.at(dest);
	}

	{
		std::string tmp;
		for (size_t i = 2; i < parse.size(); i++ )
		{
			tmp.append( parse[i] );
			if ( i < ( parse.size() - 1) )
				tmp.append(" ");
		}
			message.append("NOTICE " + dest + " :" + tmp);
	}
	if (dest_user_b)
		return send_message( message, usr_dest, usr );
	if (dest_chnl_b)
		return send_message_channel( message, usr, chnl_dest ) ;
}

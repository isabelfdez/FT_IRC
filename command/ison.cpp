/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ison.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 01:36:55 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 16:53:52 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void			Server::ison_command(std::vector<std::string> const& parse, User *usr)
{
	std::string message = " :";
	
	if ( parse.size() == 1 )
		return send_error(ERR_NEEDMOREPARAMS, "ISON :Not enough parameters", usr);
	for ( size_t i = 1; i < parse.size() ; i++ )
	{
		list_str_it start = this->_nicks.begin();
		list_str_it end = this->_nicks.end();

		for (; start != end ; ++start )
		{
			if ( *start == parse[i] )
			{
				message.append(parse[i]);
				break ;
			}
			if (i < parse.size() - 1)
				message.append(" ");
			
		}
	}
	send_reply(RPL_ISON ,message, usr);
}

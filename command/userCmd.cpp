/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:57:22 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/09 19:43:20 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::user_command(std::vector<std::string> const & parse, User *usr)
{

	if (!usr->getPassState())
		return send_error(ERR_NOPASSWD, "USER :No password entered", usr);
	if ( parse.size() < 5 )
		return send_error(ERR_NEEDMOREPARAMS, "USER :Not enough parameters", usr);
	if ( usr->getRegistered() )
		return send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", usr);
	if ( !ft_isalnum( parse[1][0] ) )
		return send_error(ERR_USERNAMEINVALID, " USER :Your username is not valid", usr);
	usr->setUserName(parse[1]);
	
	if ( std::atoi(parse[2].c_str() ) & 0b1000 )
		usr->setmode('i', true);
	usr->setRealName(parse[4]);
	if ( usr->getNick().size() > 0 && !usr->getRegistered() )
	{
		usr->setRegistered(true);
		usr->setTimePing(0);
		usr->setMask();
		this->_connected_users.push_back( usr );
		displayLog("User created", "", usr);
	}
}
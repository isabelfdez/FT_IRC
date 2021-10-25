/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:57:22 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/25 21:50:59 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::user_command( int fd, char *buffer )
{
	buffer  = buffer  + 4;
	while (*buffer  == ' ')
		buffer ++;
	if (*buffer == ':')
		buffer++;
	std::vector<std::string> token = split(buffer, ' ');
	User * tmp = this->_fd_users.at(fd);
	if ( token.size() < 4 )
		return send_error(ERR_NEEDMOREPARAMS, "USER :Not enough parameters", fd);
	if ( tmp->getRegistered() )
		return send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
	tmp->setUserName(token[0]);
	tmp->setmode(token[1][0], true);
	tmp->setRealName(token[3]);
	if ( tmp->getNick().size() > 0 && !tmp->getRegistered() )
	{
		
		tmp->setRegistered(true);
		tmp->setTimePing(0);
		this->_connected_users.push_back( tmp );
		std::cout << std::endl;
		displayTimestamp();
		std::cout << " : User created,        IP: " << tmp->getIp() << " Socket: " << fd;
	}
}
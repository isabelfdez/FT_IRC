/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/02 14:16:58 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

//:Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(int fd, char *buffer)
{
	typedef std::list<Channel *>::iterator iteratorChannel;
	char *tmp2;
	User *usr = this->_fd_users.at(fd);

	send_reply("ERROR :Closing link: ", "[Signed off]", usr);

	if ( ( tmp2 = strchr( buffer, '\r' ) ) || ( tmp2 = strchr( buffer, '\n' ) ) )
	 	*tmp2 = 0;
	std::string msg_quit_users = buffer;

	iteratorChannel channel =usr->getChannels().begin();
	iteratorChannel end = usr->getChannels().end();

	for (; channel != end ; ++channel )
		send_message_channel_block(msg_quit_users, usr ,*channel);


	displayLog("Quit success", "", usr);
	
	this->deleteUser( fd );
} 
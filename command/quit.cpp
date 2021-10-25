/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/25 22:07:44 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

//:Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(int fd, char *buffer)
{
	typedef std::list<Channel *>::iterator iteratorChannel;
	User *tmp = this->_fd_users.at(fd);

	std::string msg_quit = "ERROR :Closing link: (" + tmp->getNick()
		+ "@" + tmp->getIp() + ") [Signed off]\n";

	std::string msg_quit_users = ": " + tmp->getNick() + "! " + buffer;
	send(fd, msg_quit.c_str(),  msg_quit.length(), 0);

	iteratorChannel channel = tmp->getChannels().begin();
	iteratorChannel end = tmp->getChannels().end();

	for (; channel != end ; ++channel )
		(*channel)->sendMsgChannel( msg_quit_users, fd );
	this->deleteUser( fd );
}
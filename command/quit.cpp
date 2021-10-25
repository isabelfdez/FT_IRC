/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/25 17:02:04 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

//:Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(int fd, char *buffer)
{
	
	User *tmp = this->_fd_users.at(fd);

	std::string msg_quit = "ERROR :Closing link: (" + tmp->getNick()
		+ "@" + inet_ntoa(this->_addr_server.sin_addr) + ") [Signed off]\n";

	std::string msg_quit_users = ": " + tmp->getNick() + "! " + buffer;
	send(fd, msg_quit.c_str(),  msg_quit.length(), 0);

	std::list<Channel *>::iterator channel = tmp->getChannels().begin();
	std::list<Channel *>::iterator end = tmp->getChannels().end();

	for (; channel != end ; ++channel )
	{
		(*channel)->sendMsgChannel( msg_quit_users, fd );
		(*channel)->getUsers().remove(tmp);
	}
	this->close_fd( fd );
	this->_connected_users.remove(tmp);
	this->_fd_users.erase( fd );
	this->_nicks.remove(tmp->getNick());
	delete tmp;
}
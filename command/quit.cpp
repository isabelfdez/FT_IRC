/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/08 16:50:44 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

//:Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(std::vector<std::string> const & parse, User *usr)
{
	typedef std::list<Channel *>::iterator iteratorChannel;


	send_reply("ERROR :Closing link: ", "[Signed off]", usr);

	std::string msg_quit_users = parse[1];

	iteratorChannel channel =usr->getChannels().begin();
	iteratorChannel end = usr->getChannels().end();

	for (; channel != end ; ++channel )
		send_message_channel_block(msg_quit_users, usr ,*channel);


	displayLog("Quit success", "", usr);
	
	this->deleteUser( usr->getsockfd() );
} 
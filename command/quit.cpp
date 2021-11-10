/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/10 02:21:22 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

//:Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(std::vector<std::string> const & parse, User *usr)
{
	std::string msg_quit_users;
	send_reply("ERROR :Closing link: ", "[Signed off]", usr);

	if ( parse.size() > 1 )
		msg_quit_users = "QUIT " + parse[1];
	else
		msg_quit_users = "QUIT :[Signed off]";

	list_chnl_it channel = usr->getChannels().begin();
	list_chnl_it end = usr->getChannels().end();

	for (; channel != end ; ++channel )
	{
		send_message_channel(msg_quit_users, usr ,*channel);
	}

	displayLog("Quit success", " ", usr);
	this->deleteUser( usr->getsockfd() );
} 
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:54:41 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/10 16:52:30 by krios-fu         ###   ########.fr       */
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
		msg_quit_users = "QUIT :" + parse[1];
	else
		msg_quit_users = "QUIT :[Signed off]";
	displayLog("Quit success", " ", usr);
	this->deleteUser( usr, msg_quit_users );
} 
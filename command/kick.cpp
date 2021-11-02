/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:33:40 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/02 15:16:49 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::kick_command(char *str, int & fd)
{
	std::vector<std::string>::iterator it1;
	std::vector<std::string>::iterator it2;
    std::string message = "";
    bool        flag;

    str = str + 4;
    while (*str == ' ')
        str++;

    std::vector<std::string>    v = split(str, ' ');
    if (v.size() < 2)
		return (send_error(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters", fd));

    std::string channels = v.front();
    v.erase(v.begin());
    std::string users = v.front();
    v.erase(v.begin());
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        message += *it + " ";
    }
    if (message.size() > 0)
        flag = 1;
    std::vector<std::string>    v_channels = split(const_cast<char *>(channels.c_str()), ',');
    std::vector<std::string>    v_users = split(const_cast<char *>(users.c_str()), ',');
    
    if (v_channels.size() > 1 && v_channels.size() != v_users.size())
		return (send_error(ERR_SINTAX, "KICK :Bad sintax", fd));
    // CASO 1: Hay un solo canal (tenemos que expulsar a todos los usuarios de ese canal)
    if (v_channels.size() == 1)
    {
        if (!isChannel(v_channels.front()))
                return (send_error(ERR_NOSUCHCHANNEL, v_channels.front() + " :No such channel", fd));
        if (!this->_fd_users[fd]->getmode('o') && !this->_name_channel[v_channels.front()]->isOp(this->_fd_users[fd]))
            return (send_error(ERR_CHANOPRIVSNEEDED, "KICK :You are not channel operator", fd));
        for (it1 = v_users.begin(); it1 != v_users.end(); it1++)
        {
            if (this->_name_channel[v_channels.front()]->isUser(*it1))
            {
                this->_name_channel[v_channels.front()]->deleteUser(getUserWithNick(*it1));
                send_reply("", " KICK: you have being kicked from channel " + v_channels.front() + " " + message, getUserWithNick(*it1));
                getUserWithNick(*it1)->deleteChannel(this->_name_channel[v_channels.front()]);
                if (this->_name_channel[v_channels.front()]->getUsers().size() < 1)
                    deleteChannel(v_channels.front());
            }
            else
                return (send_error(ERR_NOTONCHANNEL, (*it1) + " :Is not on that Channel", fd));
        }
    }
    // CASO 2: Hay varios canales (tenemos que echar al i-ésimo usuario del i-ésimo canal)
    else if (v_channels.size() > 1)
    {
        it2 = v_channels.begin();
        for (it1 = v_users.begin(); it1 != v_users.end(); it1++)
        {
            if (!isChannel(*it2))
                return (send_error(ERR_NOSUCHCHANNEL, (*it2) + " :No such channel", fd));
            if (!this->_fd_users[fd]->getmode('o') && !this->_name_channel[(*it2)]->isOp(this->_fd_users[fd]))
                return (send_error(ERR_CHANOPRIVSNEEDED, "KICK :You are not channel operator", fd));
            if (this->_name_channel[(*it2)]->isUser(*it1))
            {
                this->_name_channel[(*it2)]->deleteUser(getUserWithNick(*it1));
                send_reply("", " :You have being kicked from channel " + v_channels.front() + " " + message, getUserWithNick(*it1));
                getUserWithNick(*it1)->deleteChannel(this->_name_channel[(*it2)]);
                if (this->_name_channel[(*it2)]->getUsers().size() < 1)
                    deleteChannel((*it2));
            }
            else
                return (send_error(ERR_NOTONCHANNEL, (*it1) + " :Is not on that Channel", fd));
            it2++;
        }
    }
}

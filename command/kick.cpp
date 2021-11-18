/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:33:40 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/18 18:42:33 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::kick_command(std::vector<std::string> const & parse, User * usr)
{
    if (parse.size() < 3)
		return (send_error(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters", usr));
    std::vector<std::string> v_channels = split(parse[1], ',');
    std::vector<std::string> v_users = split(parse[2], ',');
    std::string message = "";

    if (parse.size() > 3)
        message = ":" + parse[3];
    if (v_channels.size() > 1 && v_channels.size() != v_users.size())
		return (send_error(ERR_SINTAX, "KICK :Bad sintax", usr));
    if (v_channels.size() == 1)
    {
        if (!isChannel(v_channels[0]))
                return (send_error(ERR_NOSUCHCHANNEL, v_channels[0] + " :No such channel", usr));
        Channel *chann = this->_name_channel[v_channels[0]];
        if (!usr->getmode('o') && !chann->isOp(usr))
            return (send_error(ERR_CHANOPRIVSNEEDED, "KICK :You are not channel operator", usr));
        vector_str_it start = v_users.begin();
        vector_str_it end = v_users.end();
        for (; start != end; start++)
        {
            if (chann->isUser(*start))
            {
			    send_message_channel("KICK " + v_channels[0] + " " + *start + " " + message, usr, chann);
                chann->deleteUser(getUserWithNick(*start));
                getUserWithNick(*start)->deleteChannel(chann);
                if (chann->getUsers().size() < 1)
                    deleteChannel(v_channels.front());
            }
            else
                return (send_error(ERR_NOTONCHANNEL, (*start) + " :Is not on that Channel", usr));
        }
    }
    else if (v_channels.size() > 1)
    {
        vector_str_it start_usrs = v_users.begin();
        vector_str_it end_usrs = v_users.end();
        vector_str_it start_channs = v_channels.begin();
        for (; start_usrs != end_usrs; start_usrs++)
        {
            if (!isChannel(*start_channs))
                return (send_error(ERR_NOSUCHCHANNEL, (*start_channs) + " :No such channel", usr));
            Channel *chann = this->_name_channel[*start_channs];
            if (!usr->getmode('o') && !chann->isOp(usr))
                return (send_error(ERR_CHANOPRIVSNEEDED, "KICK :You are not channel operator", usr));
            if (chann->isUser(*start_usrs))
            {
			    send_message_channel("KICK " + *start_channs + " " + *start_usrs + " " +message, usr, chann);
                chann->deleteUser(getUserWithNick(*start_usrs));
                getUserWithNick(*start_usrs)->deleteChannel(chann);
                if (this->_name_channel[(*start_channs)]->getUsers().size() < 1)
                    deleteChannel((*start_channs));
            }
            else
                return (send_error(ERR_NOTONCHANNEL, (*start_usrs) + " :Is not on that Channel", usr));
            start_channs++;
        }
    }
}

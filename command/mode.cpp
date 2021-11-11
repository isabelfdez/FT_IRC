/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:16:44 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/05 14:02:03 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"


void    Server::mode_user(std::vector<std::string> parse, User * usr)
{
    if (parse.size() < 3)
        return (send_reply(RPL_UMODEIS, parse[1] + " user modes are :+" + this->getUserWithNick(parse[1])->showModes(),  usr));
    if (usr->getNick() != parse[1])
        return (send_error(ERR_USERSDONTMATCH, "MODE :Cannot change mode for other users", usr));
    else if (parse[2].size() < 2)
        return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr));
    else if (parse[2][0] == '+')
    {
        size_t i = 1;
        while (i < parse[2].size())
        {
            if (parse[2][i] == 'o')
                send_error(ERR_NOPRIVILEGES, "MODE :Permission Denied- You need to use OPER COMMAND", usr);
            else if (parse[2][i] == 'i' && !usr->getmode('i'))
                send_reply("", ": MODE :+i", usr);
            else
                send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
            i++;
        }
    }
    else if (parse[2][0] == '-')
    {
        size_t i = 1;
        while (i < parse[2].size())
        {
            if (parse[2][i] == 'o' && usr->getmode('o'))
            {
                usr->setmode('o', false);
                send_reply(RPL_YOUREOPER, " :You are not an IRC operator anymore", usr);
            }
            else if (parse[2][i] == 'i' && usr->getmode('i'))
            {
                usr->setmode('i', false);
                send_reply("", ": MODE :-i", usr);
            }
            else
                send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
            i++;
        }
    }
    else
        return (send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr));
}


void    Server::mode_chann(std::vector<std::string> parse, User * usr)
{
    Channel *chann = this->_name_channel[parse[1]];
    if (parse.size() < 3)
        return (send_reply("", " " + parse[1] + " channel modes are :+" + chann->showModes(),  usr));
    else if (!chann->isOp(usr) && !usr->getmode('o'))
            return (send_error(ERR_CHANOPRIVSNEEDED, "MODE :You are not channel operator", usr));
    else if (parse[2].size() < 2)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr));
    else if (parse[2][0] == '+')
    {
        size_t i = 1;
        while (i < parse[2].size())
        {
            if (parse[2][i] == 'o')
            {
                User * usr_dest = getUserWithNick(parse[3]);
                if (parse.size() < 4)
                    send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr);
                if (chann->isOp(usr_dest))
                    continue;
                else if (chann->isUser(parse[3]))
                {
                    chann->setOp(usr_dest);
                    send_reply("", " :" + parse[3] + " :is Channel Operator of " + parse[1], usr);
                    send_reply("", " :you are now Channel Operator of " + parse[1], usr_dest);                    
                }
                else
                    send_error(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", usr);
            }
            else if (parse[2][i] == 'b')
            {
                User * usr_dest = getUserWithNick(parse[3]);
                if (parse.size() < 4)
                    send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr);
                else
                {
                    chann->ban(usr_dest->getMask());
                    if (chann->isBanned(usr_dest->getMask()))
                    {
                        chann->deleteUser(usr_dest);
                        send_message_channel( "has left the channel " + chann->getName() , usr, chann);
                    }
                    send_message("You have being banned from " + parse[1], usr_dest, usr);
                }
            }
            else if (parse[2][i] == 'i')
            {
                if (chann->isInvite())
                    continue;
                else
                {
                    chann->setInvite(true);
                    chann->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " needs invitation\n", usr->getsockfd());
                    send_reply("", ": Channel " + parse[1] + " invitation mode set", usr);
                }
            }
            else
                send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
            i++;
        }
    }
    else if (parse[2][0] == '-')
    {
        size_t i = 1;
        while (i < parse[2].size())
        {
            if (parse[2][i] == 'o')
            {
                if (parse.size() < 4)
                    send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr);
                else if (!chann->isUser(parse[3]))
                    send_error(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", usr);
                else if (chann->isOp(getUserWithNick(parse[3])))
                {
                    chann->setOpOff(parse[3]);
                    send_reply("", " :" + parse[3] + " :is Channel Operator of " + parse[1], usr);
                    send_reply("", " :You are now Channel Operator of " + parse[1], getUserWithNick(parse[3]));   
                }
                else
                    send_error("", " :" + parse[3] + " is not Channel Operator of " + parse[1], usr);
            }
            else if (parse[2][i] == 'b')
            {
                if (parse.size() < 4)
                    send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", usr);
                else
                {
                    chann->banOff(getUserWithNick(parse[3])->getMask());
                    send_message("You have being unbanned from " + parse[1], getUserWithNick(parse[3]), usr);
                    send_reply("", " :" + getUserWithNick(parse[3])->getMask() + " unbanned", usr);
                }
            }
            else if (parse[2][i] == 'i')
            {
                if (!chann->isInvite())
                    continue;
                else
                {
                    chann->setInvite(false);
                    chann->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " no needs invitation\n", usr->getsockfd());
                    send_reply("", ": Channel " + parse[1] + " invitation mode set off", usr);
                }
            }
            else
                send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
            i++;
        }
    }
    else
        send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
}

void    Server::mode_command(std::vector<std::string> const & parse, User * usr)
{
    if (parse.size() < 2)
        return (send_reply(RPL_UMODEIS, "Your user mode are :+" + usr->showModes(),  usr));
    else if (isChannel(parse[1]))
        mode_chann(parse, usr);
    else if (isUsr(parse[1]))
        mode_user(parse, usr);
    else
        return (send_error("", "MODE :Not a Channel or a User", usr));
}

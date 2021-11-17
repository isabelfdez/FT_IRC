/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 17:16:44 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/16 19:02:42 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"


void    Server::mode_user(std::vector<std::string> parse, User * usr)
{
    if (parse.size() < 3)
        return (send_reply(RPL_UMODEIS, parse[1] + " user modes are :+" + this->getUserWithNick(parse[1])->showModes(),  usr));
    if (usr->getNick() != parse[1])
        return (send_reply(ERR_USERSDONTMATCH, "MODE :Cannot change mode for other users", usr));
    else if (parse[2].size() < 2)
        return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
    else if (parse[2][0] == '+')
    {
        size_t i = 1;
        while (i < parse[2].size())
        {
            if (parse[2][i] == 'o')
                send_reply(ERR_NOPRIVILEGES, "MODE :Permission Denied- You need to use OPER COMMAND", usr);
            else if (parse[2][i] == 'i' && !usr->getmode('i'))
                send_reply("", ": MODE :+i", usr);
            else
                send_reply(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
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
                send_reply(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
            i++;
        }
    }
    else
        return (send_reply(ERR_UMODEUNKNOWNFLAG, " MODE :Unknown MODE flag", usr));
}


void    Server::mode_chann(std::vector<std::string> parse, User * usr)
{
    Channel *chann = this->_name_channel[parse[1]];
    if (parse.size() < 3)
        return (send_reply(RPL_CHANNELMODEIS, " " + parse[1] + " +" + chann->showModes(),  usr));
    else if (!chann->isOp(usr) && !usr->getmode('o'))
            return (send_reply(ERR_CHANOPRIVSNEEDED, " " + parse[1] + " :You are not channel operator", usr));
    else if (parse[2].size() < 2)
            return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
    else if (parse[2][0] == '+')
    {
        if (parse[2][1] == 'o')
        {
            if (!isUsr(parse[3]))
                return (send_reply(ERR_USERNOTINCHANNEL, " :" + parse[3] + " is not in that Channel", usr));
            User * usr_dest = getUserWithNick(parse[3]);
            if (parse.size() < 4)
                return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
            if (chann->isOp(usr_dest))
                return;
            else if (chann->isUser(parse[3]))
            {
                chann->setOp(usr_dest);
                return (send_message_channel( "MODE " + parse[1] + " +o " + parse[3], usr, chann));              
            }
            else
                return (send_reply(ERR_USERNOTINCHANNEL, " :" + parse[3] + " is not in that Channel", usr));
        }
        else if (parse[2][1] == 'b')
        {
            if (parse.size() < 4)
                return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
            else
            {
                chann->ban(parse[3]);
                send_message_channel("MODE " + chann->getName() + " +b " + parse[3], usr, chann);
                // this->checkBans(chann);
            }
        }
        else if (parse[2][1] == 'i')
        {
            if (chann->isInvite())
                return;
            else
            {
                chann->setInvite(true);
                chann->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " needs invitation\n", usr->getsockfd());
                send_reply("", ": Channel " + parse[1] + " invitation mode set", usr);
            }
        }
        else
            send_reply(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
    }
    else if (parse[2][0] == '-')
    {
        if (parse[2][1] == 'o')
        {
            if (parse.size() < 4)
                return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
            else if (!chann->isUser(parse[3]))
                return (send_reply(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", usr));
            else if (chann->isOp(getUserWithNick(parse[3])))
            {
                chann->setOpOff(parse[3]);
                return (send_message_channel( "MODE " + parse[1] + " -o " + parse[3], usr, chann));
            }
            else
                return (send_reply("", " :" + parse[3] + " is not Channel Operator of " + parse[1], usr));
        }
        else if (parse[2][1] == 'b')
        {
            if (parse.size() < 4)
                return (send_reply(ERR_NEEDMOREPARAMS, " MODE :Not enough parameters", usr));
            else
            {
                if (chann->banOff(parse[3]))
                    return (send_message_channel("MODE " + chann->getName() + " +b " + parse[3], usr, chann));
                return ;
            }
        }
        else if (parse[2][1] == 'i')
        {
            if (!chann->isInvite())
                return;
            else
            {
                chann->setInvite(false);
                chann->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " no needs invitation\n", usr->getsockfd());
                return (send_reply("", ": Channel " + parse[1] + " invitation mode set off", usr));
            }
        }
        else
            send_reply(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", usr);
    }
    else
        send_reply("472", " " + static_cast<std::string>(&parse[2][1]) + " is unknown mode char to me", usr);
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
        return (send_reply(ERR_NOSUCHNICK, " " + parse[1] + " :No such nick/channel", usr));
}

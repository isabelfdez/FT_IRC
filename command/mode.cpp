#include "../server/Server.hpp"

// HAY QUE VER SI EL MODO ES VALIDO

void    Server::mode_command(char * str, int & fd)
{
    std::vector<std::string> parse;
    std::string s;
    size_t  i = 0;

    parse = split(str, ' ');
    i = parse.size();
    if (i < 3)
        return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
    // CANAL
    if (isChannel(parse[1]))
    {
        if (parse.size() < 3)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (!this->_name_channel[parse[1]]->isOp(this->_fd_users[fd]) && !this->isOper(this->_fd_users[fd]->getNick()))
            return (send_error(ERR_CHANOPRIVSNEEDED, "MODE :You are not channel operator", fd));
        else if (parse[2].size() < 2)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (parse[2][0] == '+')
        {
            if (parse[2][1] == 'o')
            {
                if (parse.size() < 4)
                    return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
                if (this->_name_channel[parse[1]]->isUser(parse[3]))
                {
                    this->_name_channel[parse[1]]->setOp(getUserWithNick(parse[3]));
                    send_reply("", " :" + parse[3] + " :is Channel Operator of " + parse[1], this->_fd_users[fd]);
                    send_reply("", " :you are now Channel Operator of " + parse[1], getUserWithNick(parse[3]));                    
                }
                else
                    return (send_error(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", fd));
            }
            else if (parse[2][1] == 'b')
            {
                if (parse.size() < 4)
                    return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
                if (!this->_name_channel[parse[1]]->isUser(parse[3]))
                    return (send_error(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", fd));
                else if (this->_name_channel[parse[1]]->isOp(getUserWithNick(parse[3])) || this->isOper(parse[3]))
                    return (send_error("", parse[3] + " is Operator, cannot be banned", fd));
                else
                {
                    this->_name_channel[parse[1]]->ban(getUserWithNick(parse[3]));
                    send_reply("", " :you have being banned from Channel " + parse[1], getUserWithNick(parse[3]));
                }
            }
            else if (parse[2][1] == 'i')
            {
                this->_name_channel[parse[1]]->setInvite(true);
		        this->_name_channel[parse[1]]->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " needs invitation\n", fd);
            }

        }
        else if (parse[2][0] == '-')
        {
            if (parse[2][1] == 'o')
            {
                if (parse.size() < 4)
                    return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
                else if (!this->_name_channel[parse[1]]->isUser(parse[3]))
                    return (send_error(ERR_USERNOTINCHANNEL, parse[3] + "MODE :Is not in that Channel", fd));
                if (this->_name_channel[parse[1]]->isOp(getUserWithNick(parse[3])))
                {
                    send_reply("", " :" + parse[3] + " is not Channel Operator " + parse[1], this->_fd_users[fd]);
                    this->_name_channel[parse[1]]->setOpOff(parse[3], this->_fd_users[fd]);
                }
            }
            else if (parse[2][1] == 'i')
            {
                this->_name_channel[parse[1]]->setInvite(false);
		        this->_name_channel[parse[1]]->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " doesn't need invitation\n", fd);
            }
        }
        else
            //return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
            return (send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd));

    }
    // USUARIO
    else if (isUsr(parse[1]))
    {
        if (parse.size() < 3)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (this->_fd_users[fd]->getNick() != parse[1])
            return (send_error(ERR_USERSDONTMATCH, "MODE :Cannot change mode for other users", fd));
        else if (parse[2].size() < 2)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (parse[2][0] == '+')
        {
            if (parse[2][1] == 'o')
            {
                if (!this->_fd_users[fd]->getmode('o'))
                    return (send_error(ERR_NOPRIVILEGES, "MODE :Permission Denied- You're not an IRC operator", fd));
            }
            else if (parse[2][1] == 'i' && !this->_fd_users[fd]->getmode('i'))
            {
                s.assign("MODE ");
                s.append(this->_fd_users[fd]->getNick());
                s.append(" :+i");
                this->_fd_users[fd]->setmode('i', true);
                send_message(s, fd, this->_fd_users[fd]);
            }
        }
        else if (parse[2][0] == '-')
        {
            if (parse[2][1] == 'o')
            {
                if (this->_fd_users[fd]->getmode('o'))
                {
                    this->_fd_users[fd]->setmode('o', false);
                    return (send_reply("", " :you are no longer Channel Operator", getUserWithNick(parse[1])));                    
                }

            }
            else if (parse[2][1] == 'i' && this->_fd_users[fd]->getmode('i'))
            {
                s.assign("MODE ");
                s.append(this->_fd_users[fd]->getNick());
                s.append(" :-i");
                send_message(s, fd, this->_fd_users[fd]);
                this->_fd_users[fd]->setmode('i', false);
            }
        }
        else
            return (send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd));
    }
    else
        return (send_error("", "MODE :Not a Channel or a User", fd));
}
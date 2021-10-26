#include "../server/Server.hpp"

void    Server::mode_command(char * str, int & fd)
{
    std::vector<std::string> parse;
    size_t  i = 0;

    parse = split(str, ' ');
    i = parse.size();
    if (i < 3)
        return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
    if (isChannel(parse[1]))
    {
        if (parse.size() < 4)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        if (!this->_name_channel.count(parse[1]))
            return (send_error(ERR_NOSUCHCHANNEL, "MODE :No such channel", fd));
        else if (!this->_name_channel[parse[1]]->isOp(this->_fd_users[fd]) && !this->isOper(parse[3]))
            return (send_error(ERR_CHANOPRIVSNEEDED, "MODE :You are not channel operator", fd));
        else if (parse[2].size() < 2)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (parse[2][0] == '+')
        {
            if (parse[2][1] == 'o')
            {
                if (this->_name_channel[parse[1]]->isUser(parse[3]))
                {
                    this->_name_channel[parse[1]]->setOp(getUserWithNick(parse[3]));
                    send_reply("", parse[3] + " is Channel Operator of " + parse[1], fd);
                }
                else
                    return (send_error(ERR_USERNOTINCHANNEL, parse[3] + " :Is not in that Channel", fd));
            }
        }
        else if (parse[2][0] == '-')
        {
            if (parse[2][1] == 'o')
            {
                if (this->_name_channel[parse[1]]->isOp(getUserWithNick(parse[3])))
                {
                    this->_name_channel[parse[1]]->setOpOff((parse[3]));
                    send_reply("", parse[3] + " is not Channel Operator " + parse[1], fd);
                }
            }
        }
        else
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
    }
    else if (isUsr(parse[1]))
    {
        std::cout << "sdf\n";
    }
    else
        return (send_error("", "MODE :Not a Channel or a User", fd));
}
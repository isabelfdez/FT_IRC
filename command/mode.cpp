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

// HAY QUE HACER EL MODE SIN PARAMETROS (QUE ME DIGA QUE MODES TENGO)
// SE PUEDE DESBANNEAR A ALGUIEN?

void    mode_user(std::vector<std::string> vector, User * usr)
{

}

void    mode_chann(std::vector<std::string> vector, User * usr)
{

}

void    Server::mode_command(std::vector<std::string> const & parse, User * usr)
{
    if (parse.size() < 2)
        return (showModes(usr));
    else if (isChannel(parse[1]))
        mode_chann(parse, usr);
    else if (isUsr(parse[1]))
        mode_user(parse, usr);
    else
        return (send_error("", "MODE :Not a Channel or a User", usr));
}

void    Server::mode_command(char * str, int & fd)
{
    std::vector<std::string> parse;
    std::string s;
    size_t  i = 0;
    int     j = 1;

    parse = split(str, ' ');
    i = parse.size();
    if (i < 2)
        return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
    // CANAL
    if (isChannel(parse[1]))
    {
        if (parse.size() < 3)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (!this->_name_channel[parse[1]]->isOp(this->_fd_users[fd]) && !this->_fd_users[fd]->getmode('o'))
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
                int pos0 = (parse[3].find('!') != std::string::npos ? parse[3].find('!') : 0);
                std::string nick = parse[3].substr(0, pos0);
                std::cout << "nick is " << nick << '\n';
                int pos1 = (parse[3].find('@') != std::string::npos ? parse[3].find('@') : 0);
                std::string user = parse[3].substr(pos0 + 1, pos1 - pos0 - 1);
                std::cout << "user is " << user << '\n';
                int pos2 = (parse[3].find('\r') != std::string::npos ? parse[3].find('\r') : 0);
                std::string ip = parse[3].substr(pos1 + 1, pos2 - pos1 - 1);
                if (!this->_name_channel[parse[1]]->isUser(nick))
                    return (send_error(ERR_USERNOTINCHANNEL, nick + " MODE :Is not in that Channel", fd));
                else if (this->_name_channel[parse[1]]->isOp(getUserWithNick(nick)) || getUserWithNick(nick)->getmode('o'))
                    return (send_error("", nick + " is Operator, cannot be banned", fd));
                else
                {
                    if (getUserWithNick(nick)->getUserName() == user && ip == "127.0.0.1")
                    {
                        this->_name_channel[parse[1]]->ban(getUserWithNick(nick));
                        send_reply("", " :you have being banned from Channel " + parse[1], getUserWithNick(nick));
                    }
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
            else if (parse[2][1] == 'b')
            {
                if (parse.size() < 4)
                    return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
                int pos0 = (parse[3].find('!') != std::string::npos ? parse[3].find('!') : 0);
                std::string nick = parse[3].substr(0, pos0);
                std::cout << "nick is " << nick << '\n';
                int pos1 = (parse[3].find('@') != std::string::npos ? parse[3].find('@') : 0);
                std::string user = parse[3].substr(pos0 + 1, pos1 - pos0 - 1);
                std::cout << "user is " << user << '\n';
                int pos2 = (parse[3].find('\r') != std::string::npos ? parse[3].find('\r') : 0);
                std::string ip = parse[3].substr(pos1 + 1, pos2 - pos1 - 1);
                if (!this->_name_channel[parse[1]]->isUser(nick))
                    return (send_error(ERR_USERNOTINCHANNEL, nick + " MODE :Is not in that Channel", fd));
                else if (this->_name_channel[parse[1]]->isBanned(getUserWithNick(nick)))
                {
                    if (getUserWithNick(nick)->getUserName() == user && ip == "127.0.0.1")
                    {
                        this->_name_channel[parse[1]]->banOff(getUserWithNick(nick));
                        send_reply("", " :you have being unbanned from Channel " + parse[1], getUserWithNick(nick));
                    }
                }
            }
        }
        else
            //return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
            return (send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd));

    }
    // USUARIO
    else if (isUsr(parse[1]))
    {
        if (this->_fd_users[fd]->getNick() != parse[1])
            return (send_error(ERR_USERSDONTMATCH, "MODE :Cannot change mode for other users", fd));
        if (parse.size() == 2)
        {
            std::string str;
            if (this->_fd_users[fd]->getmode('o') || this->_fd_users[fd]->getmode('i'))
            {
                str.assign("+");
                if (this->_fd_users[fd]->getmode('o'))
                    str.append("o");
                if (this->_fd_users[fd]->getmode('i'))
                    str.append("i");
            }
            return (send_reply(RPL_UMODEIS, " MODE :" + str, this->_fd_users[fd]));
        }
        else if (parse[2].size() < 2)
            return (send_error(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters", fd));
        else if (parse[2][0] == '+')
        {
            while (parse[2][j])
            {
                if (parse[2][j] == 'o')
                {
                    if (!this->_fd_users[fd]->getmode('o'))
                        send_error(ERR_NOPRIVILEGES, "MODE :Permission Denied- You're not an IRC operator", fd);
                }
                else if (parse[2][j] == 'i' && !this->_fd_users[fd]->getmode('i'))
                {
                    s.assign("MODE ");
                    s.append(this->_fd_users[fd]->getNick());
                    s.append(" :+i");
                    this->_fd_users[fd]->setmode('i', true);
                    send_message(s, fd, this->_fd_users[fd]);
                }
                else if (parse[2][j] != 'o' && parse[2][j] != 'i')
                    send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd);
                j++;
            }
        }
        else if (parse[2][0] == '-')
        {
            while (parse[2][j])
            {
                if (parse[2][j] == 'o')
                {
                    if (this->_fd_users[fd]->getmode('o'))
                    {
                        s.assign("MODE ");
                        s.append(this->_fd_users[fd]->getNick());
                        s.append(" :-o");
                        this->_fd_users[fd]->setmode('o', false);
                        send_message(s, fd, this->_fd_users[fd]);
                    }
                }
                else if (parse[2][j] == 'i' && this->_fd_users[fd]->getmode('i'))
                {
                    s.assign("MODE ");
                    s.append(this->_fd_users[fd]->getNick());
                    s.append(" :-i");
                    send_message(s, fd, this->_fd_users[fd]);
                    this->_fd_users[fd]->setmode('i', false);
                }
                else if (parse[2][j] != 'o' && parse[2][j] != 'i')
                    send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd);
                j++;
            }
        }
        else
            return (send_error(ERR_UMODEUNKNOWNFLAG, "MODE :Unknown MODE flag", fd));
    }
    else
        return (send_error("", "MODE :Not a Channel or a User", fd));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/18 18:43:11 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void    Server::nick_command(std::vector<std::string> const& parse, User *usr)
{
    std::string s;
    std::string nick = usr->getNick();

    if (!usr->getPassState())
        return send_error(ERR_NOPASSWD, "NICK :No password entered", usr);
    if (parse.size() < 2)
        return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", usr));
    else if (parse[1].size() > 9)
        return (send_error(ERR_ERRONEUSNICKNAME, parse[1] + " :Erroneous nickname", usr));
    int j = 0;
    while (parse[1][j])
    {
        if (!ft_isalnum(parse[1][j]) && !ft_isspecial(parse[1][j]) && parse[1][j] != '-')
            return (send_error(ERR_ERRONEUSNICKNAME, parse[1] + " :Erroneous nickname", usr));
        j++;
    }
    list_str_it start = this->_nicks.begin();
    list_str_it end = this->_nicks.end();
    for (;start != end; ++start)
    {
        if (ft_toupper(*start) == ft_toupper(parse[1]))
        {
            if ((nick.size() && (ft_toupper(*start) != ft_toupper(nick)))
                 || !nick.size())
                return (send_error(ERR_NICKNAMEINUSE, parse[1] + " :Nickname is already in use", usr));
        }
    }
    if (nick.size())
    {
        for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
        {
            if ((*it) == nick)
                it = this->_nicks.erase(it);
        }
        this->_nicks.push_back(parse[1]);

        typedef std::list<Channel *>::iterator it_channel;

        it_channel start = usr->getChannels().begin();
        it_channel end = usr->getChannels().end();
        std::string s;
        for (; start != end ; ++start)
            send_message_channel("NICK :" + parse[1], usr, *start);

        usr->setNick(parse[1]);
    }
    else if (!nick.size())
    {
        std::cout << "nick : " << parse[1] << std::endl;
        this->_nicks.push_back(parse[1]);
        usr->setNick(parse[1]);
    }
    if (usr->getUserName().size() > 0 && !usr->getRegistered())
    {
        usr->setRegistered(true);
        this->_connected_users.push_back(usr);
        usr->setTimePing(0);
        usr->setMask();
  
        displayLog("User created", "", usr);

    }
}

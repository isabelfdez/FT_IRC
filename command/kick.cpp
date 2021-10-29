/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:33:40 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/29 13:46:32 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::kick_command(char *str, int & fd)
{
	std::vector<std::string>::iterator it1;
    
    if (!isOper(this->_fd_users[fd]->getNick()))
        return (send_error(ERR_CHANOPRIVSNEEDED, "KICK :You are not channel operator", fd));
    str = str + 4;
    while (*str == ' ')
        str++;

    std::vector<std::string>    v = split(str, ' ');
    if (v.size() == 1)
		return (send_error(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters", fd));

    std::string channels = v.front();
    v.erase(v.begin());
    std::string users = v.front();
    v.erase(v.begin());
    std::vector<std::string>    v_channels = split(const_cast<char *>(channels.c_str()), ',');
    std::vector<std::string>    v_users = split(const_cast<char *>(users.c_str()), ',');
    
    if (v_channels.size() > 1 && v_channels.size() != v_users.size())
		return (send_error(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters", fd));
    // CASO 1: Hay un solo canal (tenemos que expulsar a todos los usuarios de ese canal)
    if (v_channels.size() == 1)
    {
        for (it1 = v_users.begin(); it1 != v_users.end(); it1++)
        {
            if (this->_name_channel[v_channels.front()]->isUser(*it1))
            {
                // Esto lo haces tu Gonzalo que se te da mejor. Basicamente este if devuelve
                // true si ha encontrado al usuario y false si no
                // Si encuentra true, solo falta echarlo.
            }
        }
    }
    // CASO 2: Hay varios canales (tenemos que echar al i-ésimo usuario del i-ésimo canal)
    else if (v_channels.size() > 1)
    {
        for (it1 = v_users.begin(); it1 != v_users.end(); it1++)
        {
            if (this->_name_channel[v_channels.front()]->isUser(*it1))
            {
                // Esto lo haces tu Gonzalo que se te da mejor. Basicamente este if devuelve
                // true si ha encontrado al usuario y false si no
                // Si encuentra true, solo falta echarlo.
            }
            v_channels.erase(v_channels.begin());
        }
    }
}

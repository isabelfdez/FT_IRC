/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/02 17:43:18 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void    Server::nick_command(char * str, int & fd)
{
    std::vector<std::string> parse;

    std::string s;

    str = str + 4;
    while (*str == ' ')
		str++;
	if (*str== ':')
		str++;
    parse = split(str, ' ');
    size_t i = 0;
    while (i < parse.size() && parse[i].size() )
        i++;
    if (!parse.size())
        return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", fd));
    // Comprobamos que el nick que nos pasan el válido (de acuerdo con el RFC)      
    else if (i > 1 && parse[1].size())
    {
        i = 0;

        while (parse[i].size())
        {
            s.append(parse[i]);
            if (parse[i + 1].size())
                s.append(" ");
            i++;
        }
        s.append(" :Erroneous nickname");
        return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
    }
    else if (!ft_isalpha(parse[0][0]) && !ft_isspecial(parse[0][0]))
    {
        s.assign(parse[0]);
        s.assign(" :Erroneous nickname");
        return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
    }
    else if (parse[0].size() > 9)
    {
        s.assign(parse[0]);
        s.assign(" :Erroneous nickname");
        return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
    }
    int j = 1;
    while (parse[0][j])
    {
        if (!ft_isalnum(parse[0][j]) && !ft_isspecial(parse[0][j]) && parse[0][j] != '-')
        {
            s.assign(parse[0]);
            s.assign(" :Erroneous nickname");
            return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
        }
        j++;
    }
    for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
    {
        if (ft_toupper(*it) == ft_toupper(parse[0]))
        {
            s.append(parse[0]);
            s.append(" :Nickname is already in use");
            return (send_error(ERR_NICKNAMEINUSE, s, fd));
        }
    }
    // Si hemos llegado hasta aquí, el nick recibido es válido
    // CASO 1: El usuario ya tenía nick y está solicitando un cambio
    if (this->_fd_users[fd]->getNick().size())
    {
        // PASO 1: Borrar su antiguo nick de la lista de nicks
        for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
        {
            if ((*it) == this->_fd_users[fd]->getNick())
                it = this->_nicks.erase(it);
        }           
        // PASO 2: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[0]);

        // PASO 3: Cambiar el nick del usuario      
        this->_fd_users[fd]->setNick(parse[0]);

    }
    // CASO 2: El usuario se asigna un nick por primera vez
    else if (!this->_fd_users[fd]->getNick().size())
    {
        // PASO 1: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[0]);
        // PASO 2: Cambiar el nick del usuario      
        this->_fd_users[fd]->setNick(parse[0]);
    }
    // Por último, miramos si esta llamada a NICK ha hecho que el usuario complete su proceso de registro
    if (this->_fd_users[fd]->getUserName().size() > 0 && !this->_fd_users[fd]->getRegistered())
    {
        // Cambiamos el valor del estado de registro
        this->_fd_users[fd]->setRegistered(true);
        // Añadimos el usuario a la lista de usuarios
        this->_connected_users.push_back(this->_fd_users[fd]);
        this->_fd_users[fd]->setTimePing(0);
  
	    displayLog("User created", "", this->_fd_users[fd]);

    }
}
// Falta gestionar TOO MANY TARGETS. No sé si el error está bien.





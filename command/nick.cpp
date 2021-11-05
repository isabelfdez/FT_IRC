/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/05 17:34:23 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void    Server::nick_command(char * str, int & fd)
{
    std::vector<std::string> parse;

    std::string s;
    User        *usr = this->_fd_users[fd];
    
    if (!usr->getPassState())
		return send_error(ERR_NOPASSWD, "NICK :No password entered", fd);
    str = str + 4;
    while (*str == ' ')
		str++;
	if (*str && *str== ':')
		str++;
    parse = split(str, ' ');
    if (!parse.size())
        return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", fd));
    else if (parse.size() > 1)
    {
        s.assign(str);
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
            if ((usr->getNick().size() && (ft_toupper(*it) != ft_toupper(usr->getNick())))
                 || !usr->getNick().size())
            {
                s.append(parse[0]);
                s.append(" :Nickname is already in use");
                return (send_error(ERR_NICKNAMEINUSE, s, fd));
            }
        }
    }
    // Si hemos llegado hasta aquí, el nick recibido es válido
    // CASO 1: El usuario ya tenía nick y está solicitando un cambio
    if (usr->getNick().size())
    {
        // PASO 1: Borrar su antiguo nick de la lista de nicks
        for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
        {
            if ((*it) == usr->getNick())
                it = this->_nicks.erase(it);
        }           
        // PASO 2: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[0]);

        // PASO 3: Notificar el cambio a los usuarios que compartan canal con él
        typedef std::list<Channel *>::iterator it_channel;

        it_channel start = usr->getChannels().begin();
        it_channel end = usr->getChannels().end();
        std::string s;
        for (; start != end ; ++start)
        {
            s.assign("NICK :");
            s.append(parse[0]);
            send_message_channel_block(s, usr, *start);
        }

        // PASO 4: Cambiar el nick del usuario      
        usr->setNick(parse[0]);
    }
    // CASO 2: El usuario se asigna un nick por primera vez
    else if (!usr->getNick().size())
    {
        // PASO 1: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[0]);
        // PASO 2: Cambiar el nick del usuario      
        usr->setNick(parse[0]);
    }
    // Por último, miramos si esta llamada a NICK ha hecho que el usuario complete su proceso de registro
    if (usr->getUserName().size() > 0 && !usr->getRegistered())
    {
        // Cambiamos el valor del estado de registro
        usr->setRegistered(true);
        // Añadimos el usuario a la lista de usuarios
        this->_connected_users.push_back(usr);
        usr->setTimePing(0);
  
	    displayLog("User created", "", usr);

    }
}
// Falta gestionar TOO MANY TARGETS. No sé si el error está bien.

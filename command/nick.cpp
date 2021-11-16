/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/16 18:50:31 by krios-fu         ###   ########.fr       */
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
    // Si hemos llegado hasta aquí, el nick recibido es válido
    // CASO 1: El usuario ya tenía nick y está solicitando un cambio
    if (nick.size())
    {
        // PASO 1: Borrar su antiguo nick de la lista de nicks
        for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
        {
            if ((*it) == nick)
                it = this->_nicks.erase(it);
        }
        // PASO 2: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[1]);

        // PASO 3: Notificar el cambio a los usuarios que compartan canal con él
        typedef std::list<Channel *>::iterator it_channel;

        it_channel start = usr->getChannels().begin();
        it_channel end = usr->getChannels().end();
        std::string s;
        for (; start != end ; ++start)
            send_message_channel("NICK :" + parse[1], usr, *start);

        // PASO 4: Cambiar el nick del usuario
        usr->setNick(parse[1]);
    }
    // CASO 2: El usuario se asigna un nick por primera vez
    else if (!nick.size())
    {
        std::cout << "nick : " << parse[1] << std::endl;
        // PASO 1: Añadir el nuevo nick a la lista de nicks
        this->_nicks.push_back(parse[1]);
        // PASO 2: Cambiar el nick del usuario
        usr->setNick(parse[1]);
    }
    // Por último, miramos si esta llamada a NICK ha hecho que el usuario complete su proceso de registro
    if (usr->getUserName().size() > 0 && !usr->getRegistered())
    {
        // Cambiamos el valor del estado de registro
        usr->setRegistered(true);
        // Añadimos el usuario a la lista de usuarios
        this->_connected_users.push_back(usr);
        usr->setTimePing(0);
        usr->setMask();
  
        displayLog("User created", "", usr);

    }
}
// Falta gestionar TOO MANY TARGETS. No sé si el error está bien.

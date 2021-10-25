/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/25 17:20:20 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::nick_command(char * str, int & fd)
{
	char 		**parse;
	
	std::string	s;

	char 		*substr;

	substr = ft_substr(str, '\r'); 
	parse = ft_split(substr, ' ');
	int i = 0;
	while (parse[i])
		i++;
	if (!parse[1])
		return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", fd));
	// Comprobamos que el nick que nos pasan el válido (de acuerdo con el RFC)
	else if (parse[2])
	{
		int	i = 1;
		while (parse[i])
		{
			s.append(parse[i]);
			if (parse [i + 1])
				s.append(" ");
			i++;
		}
		s.append(" :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	else if (!ft_isalpha(parse[1][0]) && !ft_isspecial(parse[1][0]))
	{
		s.assign(parse[1]);
		s.assign(" :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	else if (ft_strlen(parse[1]) > 9)
	{
		s.assign(parse[1]);
		s.assign(" :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	int j = 1;
	while (parse[1][j])
	{
		if (!ft_isalnum(parse[1][j]) && !ft_isspecial(parse[1][j]) && parse[1][j] != '-')
		{
			s.assign(parse[1]);
			s.assign(" :Erroneous nickname");
			return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
		}
		j++;
	}
	for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
	{
		if (*it == parse[1])
		{
			s.append(parse[1]);
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
			if (*it == this->_fd_users[fd]->getNick())
				this->_nicks.erase(it);
		// PASO 2: Añadir el nuevo nick a la lista de nicks
		this->_nicks.push_back(parse[1]);
		// PASO 3: Cambiar el nick del usuario		
		this->_fd_users[fd]->setNick(parse[1]);
	}
	// CASO 2: El usuario se asigna un nick por primera vez
	else if (!this->_fd_users[fd]->getNick().size())
	{
		// PASO 1: Añadir el nuevo nick a la lista de nicks
		this->_nicks.push_back(parse[1]);
		// PASO 2: Cambiar el nick del usuario		
		this->_fd_users[fd]->setNick(parse[1]);
	}
	// Por último, miramos si esta llamada a NICK ha hecho que el usuario complete su proceso de registro
	if (this->_fd_users[fd]->getUserName().size() > 0 && !this->_fd_users[fd]->getRegistered())
	{
		// Cambiamos el valor del estado de registro
		this->_fd_users[fd]->setRegistered(true);
		// Añadimos el usuario a la lista de usuarios
		this->_connected_users.push_back(this->_fd_users[fd]);
		std::cout << std::endl;
		displayTimestamp();
		std::cout << " : User created,        IP: " << this->getIpUser() << " Socket: " << fd;

	}
}

// Falta gestionar TOO MANY TARGETS. No sé si el error está bien.





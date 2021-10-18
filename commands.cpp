/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/18 19:11:30 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include "utils.hpp"

void	Server::nick_command(std::string command, char * str, int fd)
{
	char 		**parse;
	std::string	s;

	(void) command;

	parse = ft_split(str, ' ');
	if (!parse[1])
		return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", fd));
	// Comprobamos que el nick que nos pasan el válido (de acuerdo con el RFC)
	else if (parse[2])
	{
		s.assign("<");
		int	i = 1;
		while (parse[i])
		{
			s.append(parse[i]);
			i++;
		}
		s.append("> :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	else if (!ft_isalpha(parse[1][0]) && !ft_isspecial(parse[1][0]))
	{
		s.assign("<");
		s.append(parse[1]);
		s.append("> :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	else if (ft_strlen(parse[1]) > 9)
	{
		s.assign("<");
		s.append(parse[1]);
		s.append("> :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	int j = 1;
	while (parse[1][j])
	{
		if (!ft_isalnum(parse[1][j]) && !ft_isspecial(parse[1][j]) && parse[1][j] != '-')
		{
			s.assign("<");
			s.append(parse[1]);
			s.append("> :Erroneous nickname");
			return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
		}
		j++;
	}
	for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
	{
		if (*it == parse[1])
		{
			s.assign("<");
			s.append(parse[1]);
			s.append("> :Erroneous nickname");
			return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
		}
	}
	// Si hems llegado hasta aquí, el nock recibido es válido
	// CASO 1: El usuario ya tenía nick y está solicitando un cambio
	if (this->_fd_users[fd]->getNick().size())
	{
		// PASO 1: Borrar su antiguo nick de la lista de nicks
		for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
			if (*it == this->_fd_users[fd]->getNick())
				this->_nicks.erase(it);
		// PASO 2: Añadir el nuevo nick a la lista de nicks
		this->_nicks.push_back(parse[1]);
		// PASO 3: Cambiar el nick del usuario en el mapa nick-usuario
		
		this->_fd_users[fd]->setNick(parse[1]);
	}
	// CASO 2: El usuario se asigna un nick por primera vez
	
}



/* 
** Token User devuelve un puntero con los 4 token, y  en caso de que los numeros de token sea 
** < a 4 devuelve un null.
** @krios-fu
**
*/
std::string * token_user(char *buffer)
{
	std::string tokens[4];
	std::string  token, s_buffer = buffer;
	size_t pos = 0, space = 0, i  = 0;

	while ( (pos = s_buffer.find(" ")) != std::string::npos && i < 4)
	{
		token = s_buffer.substr(0, pos + space);
		if ( token.length() > 0 )
		{
			tokens[i] = token;
			i++;
		}
		std::string::iterator start = s_buffer.begin();
		while ( *start == ' ' && start != s_buffer.end() )
		{
			space++;
			start++;
		}
		s_buffer.erase(0, pos + space);
		space = 0;
		std::cout << token << token.length() <<std::endl;
	}
	if (i != 4)
		return NULL;
	return tokens;
}

void Server::user_command( int fd, char *buffer )
{
	std::string *token = token_user(buffer);

	User * tmp = this->_fd_users.at(fd);
	
	if (!token)
		return send_error(ERR_NEEDMOREPARAMS, "User :Not enough parameters", fd);
	if ( tmp->getRegistered() )
		return send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
		
	tmp->setUserName(token[0]);
	tmp->setmode(token[1][0], true);
	tmp->setRealName(token[3]);

	if ( tmp->getNick().size() > 0 && !tmp->getRegistered() )
	{
		tmp->setRegistered(true);
		this->_connected_users.push_back(tmp);
	}
}

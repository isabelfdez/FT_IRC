/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/14 19:51:38 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "utils.hpp"

// void	Server::nick_command(std::string command, char * str, int fd)
// {
// 	char 		**parse;
// 	std::string	s;

// 	parse = ft_split(str, ' ');
// 	if (!parse[1])
// 		return (send_error(ERR_NONICKNAMEGIVEN, ":No nickname given", fd));
// 	// Comprobamos que el nick que nos pasan el válido (de acuerdo con el RFC)
// 	else if (parse[2])
// 	{
// 		s.assign("<");
// 		int	i = 1;
// 		while (parse[i])
// 		{
// 			s.append(parse[i]);
// 			i++;
// 		}
// 		s.append("> :Erroneous nickname");
// 		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
// 	}
// 	else if (!ft_isalpha(parse[1][0]) && !ft_isspecial(parse[1][0]))
// 	{
// 		s.assign("<");
// 		s.append(parse[1]);
// 		s.append("> :Erroneous nickname");
// 		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
// 	}
// 	else if (ft_strlen(parse[1]) > 9)
// 	{
// 		s.assign("<");
// 		s.append(parse[1]);
// 		s.append("> :Erroneous nickname");
// 		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
// 	}
// 	int j = 1;
// 	while (parse[1][j])
// 	{
// 		if (!ft_isalnum(parse[1][j]) && !ft_isspecial(parse[1][j]) && parse[1][j] != '-')
// 		{
// 			s.assign("<");
// 			s.append(parse[1]);
// 			s.append("> :Erroneous nickname");
// 			return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
// 		}
// 		j++;
// 	}
// 	for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
// 	{
// 		if (*it == parse[1])
// 		{
// 			s.assign("<");
// 			s.append(parse[1]);
// 			s.append("> :Erroneous nickname");
// 			return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
// 		}
// 	}
// 	// Si hems llegado hasta aquí, el nock recibido es válido
// 	// CASO 1: El usuario ya tenía nick y está solicitando un cambio
// 	if (this->_fd_users[fd]->getNick().size())
// 	{
// 		// PASO 1: Borrar su antiguo nick de la lista de nicks
// 		for (std::list<std::string>::iterator it = this->_nicks.begin(); it != this->_nicks.end(); ++it)
// 			if (*it == this->_fd_users[fd]->getNick())
// 				this->_nicks.erase(it);
// 		// PASO 2: Añadir el nuevo nick a la lista de nicks
// 		this->_nicks.push_back(parse[1]);
// 		// PASO 3: Cambiar el nick del usuario en el mapa nick-usuario
		
// 		this->_fd_users[fd]->setNick(parse[1]);
// 	}
// 	// CASO 2: El usuario se asigna un nick por primera vez
	
// }

void	Server::join_channel(char * str, int fd)
{
			std::cout << "hola\n";
	std::string s;
	std::string	str1(str);
	if (str1[0] != '#')
	{
		s.assign(str1);
		s.append(" :No such channel");
		return (send_error(ERR_NOSUCHCHANNEL, s, fd));
	}
	if (str1.size() > 12)
	{
		s.assign("<");
		s.append(str1);
		s.append("> :Erroneous nickname");
		return (send_error(ERR_NOSUCHCHANNEL, s, fd));
	}
	if (this->_fd_users[fd]->getMaxChannels())
	{
		s.assign(this->_fd_users[fd]->getNick());
		s.append(" :You have joined too many channels");
		return (send_error(ERR_TOOMANYCHANNELS, s, fd));
	}
	if (this->_name_channel.count(str1) && this->_name_channel[str1]->getIsFull())
	{
		s.assign(str1);
		s.append(" :Cannot join channel (+l)");
		return (send_error(ERR_CHANNELISFULL, s, fd));
	}
	else if (this->_name_channel[str1])
	{
	 	// User join channel
		this->_name_channel[str1]->addUser(this->_fd_users[fd]);
	}
	else
	{
		// Create channel
		this->_name_channel[str1] = new Channel(str1);
		this->_name_channel[str1]->addUser(this->_fd_users[fd]);

	}
}

void	Server::join_command(std::string strin, char * str, int fd)
{
	char 		**parse;
	char		**parse2;
	std::string	s;

	char	*tmp;
	int		i = 0;

	tmp = strchr(str, '\r');
	*tmp = 0;
	parse = ft_split(str, ' ');
	if (!parse[1])
	{
		strin.erase(std::remove(strin.begin(), strin.end(), ' '), strin.end());
		std::transform(strin.begin(), strin.end(),strin.begin(), ::toupper);
		s.assign(strin);
 		s.append(" :Not enough parameters");
		return (send_error(ERR_NEEDMOREPARAMS, s, fd));
	}
	if (parse[1][0] == ':')
		parse2 = ft_split(&parse[1][1], ',');
	else
		parse2 = ft_split(parse[1], ',');
	std::cout << "hola1\n";
	while (parse2[i] && i < 16)
	{
		Server::join_channel(parse2[i], fd);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/20 15:34:55 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include "utils.hpp"

static std::string * token_user(char *buffer)
{
	std::string* tokens = new std::string[ 5 ];
	std::string token, s_buffer = buffer;
	size_t pos = 0, space = 0, i  = 0;
	while ( (pos = s_buffer.find(" ") ) != std::string::npos ||  i < 5)
	{
		token = s_buffer.substr(0, pos + space);
		if ( token.length() > 0 )
		{
			tokens[i] = token;
			i++;
		}
		std::string::iterator start = s_buffer.begin();
		while ( *start++ == ' ' && start != s_buffer.end() )
		{
			space++;
			start++;
		}
		s_buffer.erase(0, pos + space);
		space = 0;
		if ( pos == std::string::npos )
			break ;
	}
	if ( i != 5 )
	{
		delete [] tokens;
		return NULL;
	}
	return tokens;
}

void Server::send_msg_chanell( Channel * channel, std::string message )
{
	(void) channel;
	std::list<User *>::iterator __user = channel->getUsers().begin();
	// std::list<User *>::iterator end = channel->getUsers().end();

	std::cout << (*__user )->getsockfd() << " " << message << std::endl ; 
	// for ( ; __user  != end ; ++__user )
		// send( (*__user )->getsockfd(), message.c_str(),  message.length(), 0);
}

// :Andres---pintor!HZ6hWkIW3@hCU.585.rEvd2U.virtual QUIT :Signed off
void Server::quit_command(int fd, char *buffer)
{
	
	User *tmp = this->_fd_users.at(fd);

	std::string msg_quit = "ERROR :Closing link: (" + tmp->getNick()
		+ "@" + inet_ntoa(this->_addr_server.sin_addr) + ") [Signed off]\n";
	
	std::string msg_quit_users = ": " +  tmp->getNick() + "! " + buffer;
	send(fd, msg_quit.c_str(),  msg_quit.length(), 0);

	std::list<Channel *>::iterator channel = this->_channel.begin();
	// std::list<Channel *>::iterator end = tmp->getChannels().end();
	
	/*for (; channel != end ; ++channel )
	{
		std::cout << " [[[[[[ HERE SEGFAULT ]]]]] \n";
		// delete *channel;
		// tmp->getChannels().erase( channel );
		// eliminar lista de canales de la clase servidor 
	}*/
		
		// std::cout << (*(*channel)->getUsers().begin())->getNick() << std::endl;
		 send_msg_chanell( *channel, msg_quit_users );
	close (fd);
}

void	Server::user_command( int fd, char *buffer )
{
	std::string *token = token_user(buffer);
	User * tmp = this->_fd_users.at(fd);
	if (!token)
		return send_error(ERR_NEEDMOREPARAMS, "USER :Not enough parameters", fd);
	if ( tmp->getRegistered() )
		return send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
	tmp->setUserName(token[1]);
	tmp->setmode(token[2][0], true);
	tmp->setRealName(token[4]);
	if ( tmp->getNick().size() > 0 && !tmp->getRegistered() )
	{
		tmp->setRegistered(true);
		this->_connected_users.push_back(tmp);
	}

	(*this->_channel.begin())->addUser(tmp);
	
	delete [] token;
}

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
		s.append(parse[1]);
		s.append(" :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	else if (ft_strlen(parse[1]) > 9)
	{
		s.append(parse[1]);
		s.append(" :Erroneous nickname");
		return (send_error(ERR_ERRONEUSNICKNAME, s, fd));
	}
	int j = 1;
	while (parse[1][j])
	{
		if (!ft_isalnum(parse[1][j]) && !ft_isspecial(parse[1][j]) && parse[1][j] != '-')
		{
			s.append(parse[1]);
			s.append(" :Erroneous nickname");
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
	// Si hemos llegado hasta aquí, el nock recibido es válido
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
	}
}

// Falta gestionar TOO MANY TARGETS. No sé si el error está bien.

void	Server::privmsg_command(std::string & command, int & fd)
{
	std::string 				delimiter = " ";
	size_t						pos = 0;
	std::string 				token;
	int							deliver_fd;
	std::list<User *>::iterator it;
	std::string					s;

	// Quitamos el comando
	pos = command.find(delimiter);
	command.erase(0, pos + delimiter.length());
	while (*(command.begin()) == ' ')
		command.erase(0, 1);	
	// Lo que tenemos a continuación es el target del mensaje
	if ((pos = command.find(delimiter)) == std::string::npos)
	{
		s.assign(":No recipient given (PRIVMSG)");
		return (send_error(ERR_NORECIPIENT, s, fd));
	}
	token = command.substr(0, pos);
	for (it = this->_connected_users.begin(); it != this->_connected_users.end(); ++it)
	{
		if ((*it)->getNick() == token)
		{
			deliver_fd = (*it)->getsockfd();
			break ;
		}
	}
	// Mandamos un error si no hemos encontrado el nick
	if (it == this->_connected_users.end())
	{
		s.assign(token);
		s.assign(" :No such nick/channel");
		return (send_error(ERR_NOSUCHNICK, s, fd));
	}
	// Si hemos encontrado el nick, mandamos el mensaje
	else
	{
		command.erase(0, pos + delimiter.length());
		while (command.begin() != command.end() && *(command.begin()) == ' ')
			command.erase(0, 1);
		if (command.begin() == command.end())
			return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
		if (!(*(command.begin()) == ':'))
		{
			s.assign(token);
			s.assign(" : Too many recipients.");
			return (send_error(ERR_TOOMANYTARGETS, s, fd));
		}
		else
			command.erase(0, 1);
		if (command.begin() == command.end())
			return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
		send(deliver_fd, command.c_str(), command.length(), 0);
	}
}

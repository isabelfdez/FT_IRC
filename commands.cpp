/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 18:43:25 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/21 19:56:14 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"
#include "utils.hpp"


std::string * token_user(char *buffer)
{
    std::string * tokens = new std::string[4];
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
    }
    if (i != 4)
        return NULL;
    return tokens;
}

void	Server::user_command( int fd, char *buffer )
{
    std::string *token = token_user(buffer);
	std::string mask;
    User * tmp = this->_fd_users.at(fd);
    if (!token)
        return send_error(ERR_NEEDMOREPARAMS, "USER :Not enough parameters", fd);
    if ( tmp->getRegistered() )
        return send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
    tmp->setUserName(token[0]);
    tmp->setmode(token[1][0], true);
    tmp->setRealName(token[3]);
    if ( tmp->getNick().size() > 0 && !tmp->getRegistered() )
    {
        tmp->setRegistered(true);
        this->_connected_users.push_back(tmp);
		complete_registration(this->_fd_users[fd]);
    }
}

// AQUI FALTA GESTIONAR LO DE LAS COMAS

void	Server::nick_command(char * str, int & fd)
{
	char 		**parse;
	char 		*substr;
	std::string	s;
	std::string mask;

	std::cout << str << "\n";
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
		mask.append(":");
		mask.append(parse[1]);
		mask.append("!");
		mask.append(this->_fd_users[fd]->getUserName());
		mask.append("@ft_irc.com");
		this->_fd_users[fd]->setMask(mask);
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
		this->_connected_users.push_back(this->_fd_users[fd]);
		complete_registration(this->_fd_users[fd]);
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
	std::string					message;

	//std::string					aux(command);

	if ((pos = command.find(delimiter)) == std::string::npos)
	{
		s.assign(":No recipient given (PRIVMSG)");
		return (send_error(ERR_NORECIPIENT, s, fd));
	}
	command.erase(0, pos + delimiter.length());
	while (command.begin() != command.end() && *(command.begin()) == ' ')
		command.erase(0, 1);
	if (command.begin() == command.end())
	{
		s.assign(":No recipient given (PRIVMSG)");
		return (send_error(ERR_NORECIPIENT, s, fd));
	}
	if ((pos = command.find(delimiter)) == std::string::npos)
		return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
	token = command.substr(0, pos);
	while (command.begin() != command.end() && *(command.begin()) == ' ')
		command.erase(0, 1);
	std::cout << "el target es !" << token << "!\n";
	for (it = this->_connected_users.begin(); it != this->_connected_users.end(); ++it)
	{
		if ((*it)->getNick() == token)
		{
			deliver_fd = (*it)->getsockfd();
			break ;
		}
	}
	// Si hemos encontrado el nick, mandamos el mensaje
	if (it != this->_connected_users.end())
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
		message.append("PRIVMSG " + token + " :" + command);
		return (send_message(message, deliver_fd, this->_fd_users[fd]));
	}
	// Ahora vamos al caso de mandar un mensaje a un todo un canal
	// Mandamos un error si no hemos encontrado el token ni en los nicks ni en los channels
	if (this->_name_channel.find(token) == this->_name_channel.end())
	{
		s.assign(token);
		s.assign(" :No such nick/channel");
		return (send_error(ERR_NOSUCHNICK, s, fd));

	}
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
		message.append("PRIVMSG " + token + " :" + command);
		return (send_message_channel(message, this->_fd_users[fd], this->_name_channel[token]));
	}
}

void	Server::join_channel(char * str, int & fd)
{

	int j = 1;
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
		s.append(str1);
		s.append(" :No such channel");
		return (send_error(ERR_NOSUCHCHANNEL, s, fd));
	}
	while (str1[j])
	{
		if (!ft_isalnum(str1[j]) && !ft_isspecial(str1[j]) && str1[j] != '-')
		{
			s.append(str1);
			s.append(" :No such channel");
			return (send_error(ERR_NOSUCHCHANNEL, s, fd));
		}
		j++;
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
		s.assign(this->_fd_users[fd]->getNick());
		s.append(" joined ");
		s.append(this->_name_channel[str1]->getName());
		s.append("\r\n");
		this->_name_channel[str1]->sendMsgChannel(s);
		this->_name_channel[str1]->addUser(this->_fd_users[fd]);
		this->_fd_users[fd]->addChannel(this->_name_channel[str1]);
		s.assign(" JOIN: ");
		s.append(str1);
		send_reply(RPL_NOTOPIC, s, fd);
		send_reply(RPL_USERS, this->_name_channel[str1]->userList(), fd);
	}
	else
	{
		// Create and join channel
		this->_name_channel[str1] = new Channel(str1, this->_fd_users[fd]);
		this->_name_channel[str1]->addUser(this->_fd_users[fd]);
		this->_fd_users[fd]->addChannel(this->_name_channel[str1]);
		s.assign(" JOIN: ");
		s.assign(str1);
		send_reply(RPL_NOTOPIC, s, fd);
		send_reply(RPL_USERS, this->_name_channel[str1]->userList(), fd);
	}
}

void	Server::join_command(char * str, int & fd)
{
	char 		**parse;
	std::string	s;

	char	*tmp;
	int		i = 0;

	tmp = strchr(str, '\r');
	*tmp = 0;
	str = str + 4;
	while (*str == ' ')
		str++;
	if (*str == ':')
		str++;
	parse = ft_split(str, ',');
	if (!parse[0])
	{
 		s.append("JOIN :Not enough parameters");
		free(parse);
		return (send_error(ERR_NEEDMOREPARAMS, s, fd));
	}
	while (parse[i] && i < 16)
	{
		Server::join_channel(parse[i], fd);
		i++;
	}
	free(parse);
}

void	Server::part_channel(char * str, int & fd)
{
	std::string s;
	std::string	str1(str);

	std::cout << "hola2\n";

	//str1.erase(str1.begin());
	std::cout << str1 << std::endl;
	if (str[0] != '#')
	{
		s.assign(str1);
		s.append(" :No such channel");
		return (send_error(ERR_NOSUCHCHANNEL, s, fd));
	}
	else if (this->_name_channel.count(str1))
	{
		std::cout << "hola3\n";

		this->_name_channel[str1]->deleteUser(this->_fd_users[fd]);
		this->_fd_users[fd]->deleteChannel(this->_name_channel[str1]);
		std::cout << "hola4\n";

	}
}

void	Server::part_command(char * str, int & fd)
{
	char		**parse;
	char		*tmp;
	std::string	s;
	int			i = 0;

	std::cout << "hola1\n";
	tmp = strchr(str, '\r');
	*tmp = 0;
	str = str + 4;
		while (*str == ' ')
		str++;
	if (*str == ':')
		str++;
	parse = ft_split(str, ',');
	if (!parse[0])
	{
 		s.append("PART :Not enough parameters");
		free(parse);
		return (send_error(ERR_NEEDMOREPARAMS, s, fd));
	}
	while (parse[i] && i < 16)
	{
		Server::part_channel(parse[i], fd);
		i++;
	}
}
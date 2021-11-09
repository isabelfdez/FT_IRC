/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:18:55 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/09 15:52:39 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::privmsg_command( std::vector<std::string> const & parse, User *usr )
{

	std::string nick_dest = parse[1];

	

	
	std::string 				delimiter = " ";
	size_t						pos = 0;
	std::string 				token;
	int							deliver_fd;
	std::list<User *>::iterator it;
	std::string					s;
	std::string					message;


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
	{
		if (!(*(command.begin()) == ':'))
			return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
		else
		{
			s.assign(":No recipient given (PRIVMSG)");
			return (send_error(ERR_NOTEXTTOSEND, s, fd));
		}
	}
	token = command.substr(0, pos);
	while (command.begin() != command.end() && *(command.begin()) == ' ')
		command.erase(0, 1);
	for (it = this->_connected_users.begin(); it != this->_connected_users.end(); ++it)
	{
		if (ft_toupper((*it)->getNick()) == ft_toupper(token))
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
			if (!ft_isspecial(*(command.begin())) && !ft_isalpha(*(command.begin())))
			{
				s.assign(" : Incorrect sintax.");
				return (send_error(ERR_SINTAX, s, fd));
			}
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
		s.append(" :No such nick/channel");
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
			if (!ft_isspecial(*(command.begin())) && !ft_isalpha(*(command.begin())))
			{
				s.assign(" : Incorrect sintax.");
				return (send_error(ERR_SINTAX, s, fd));
			}
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
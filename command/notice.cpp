#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::notice_command(std::string & command, int & fd)
{
	std::string 				delimiter = " ";
	size_t						pos = 0;
	std::string 				token;
	int							deliver_fd;
	std::list<User *>::iterator it;
	std::string					s;
	std::string					message;


	if ((pos = command.find(delimiter)) == std::string::npos)
        return ;
	command.erase(0, pos + delimiter.length());
	while (command.begin() != command.end() && *(command.begin()) == ' ')
		command.erase(0, 1);
	if (command.begin() == command.end())
	    return ;
	if ((pos = command.find(delimiter)) == std::string::npos)
        return ;
	token = command.substr(0, pos);
	while (command.begin() != command.end() && *(command.begin()) == ' ')
		command.erase(0, 1);
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
            return ;
		if (!(*(command.begin()) == ':'))
		    return ;
		else
			command.erase(0, 1);
		if (command.begin() == command.end())
			return ;
		message.append("NOTICE " + token + " :" + command);
		return (send_message(message, deliver_fd, this->_fd_users[fd]));
	}
	// Ahora vamos al caso de mandar un mensaje a un todo un canal
	// Mandamos un error si no hemos encontrado el token ni en los nicks ni en los channels
	if (this->_name_channel.find(token) == this->_name_channel.end())
	    return ;
	else
	{
		command.erase(0, pos + delimiter.length());
		while (command.begin() != command.end() && *(command.begin()) == ' ')
			command.erase(0, 1);
		if (command.begin() == command.end())
			return ;
		if (!(*(command.begin()) == ':'))
		    return ;
		else
			command.erase(0, 1);
		if (command.begin() == command.end())
			return ;
		message.append("NOTICE " + token + " :" + command);
		return (send_message_channel(message, this->_fd_users[fd], this->_name_channel[token]));
	}
}
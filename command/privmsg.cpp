
#include "../server/Server.hpp"
#include "../utils.hpp"

void    Server::privmsg_command(std::string & command, int & fd)
{
    std::string                 delimiter = " ";
    size_t                      pos = 0;
    std::string                 token;
    int                         deliver_fd;
    std::list<User *>::iterator it;
    std::string                 aux(command);



    if (((pos = command.find('\r')) || (pos = command.find('\r'))) && pos != std::string::npos )
        command[pos] = 0;
    // Quitamos el comando
    pos = command.find(delimiter);
    command.erase(0, pos + delimiter.length());
    while (*(command.begin()) == ' ')
        command.erase(0, 1);    
    // Lo que tenemos a continuación es el target del mensaje
    if ((pos = command.find(delimiter)) == std::string::npos)
        return (send_error(ERR_NORECIPIENT, ":No recipient given (PRIVMSG)", fd));
    token = command.substr(0, pos);
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
            return (send_error(ERR_TOOMANYTARGETS, token + " : Too many recipients.", fd));
        else
            command.erase(0, 1);
        if (command.begin() == command.end())
            return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
        return (send_message(aux, deliver_fd, this->_fd_users[fd]));
    }
    // Ahora vamos al caso de mandar un mensaje a un todo un canal
    // Mandamos un error si no hemos encontrado el token ni en los nicks ni en los channels
    if (this->_name_channel.find(token) == this->_name_channel.end())
        return (send_error(ERR_NOSUCHNICK, token + " :No such nick/channel", fd));
    else
    {
        command.erase(0, pos + delimiter.length());
        while (command.begin() != command.end() && *(command.begin()) == ' ')
            command.erase(0, 1);
        if (command.begin() == command.end())
            return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
        if (!(*(command.begin()) == ':'))
            return (send_error(ERR_TOOMANYTARGETS, token + " : Too many recipients.", fd));
        else
            command.erase(0, 1);
        if (command.begin() == command.end())
            return (send_error(ERR_NOTEXTTOSEND, ":No text to send", fd));
        return (send_message_channel(aux, this->_fd_users[fd], this->_name_channel[token]));
    }
}

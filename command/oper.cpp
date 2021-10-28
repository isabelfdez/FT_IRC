#include "../server/Server.hpp"

void	Server::oper_command(char * str, int & fd)
{
    std::vector<std::string> parse;

    parse = split(str, ' ');
    if (parse.size() < 3)
		return (send_error(ERR_NEEDMOREPARAMS, "OPER :Not enough parameters", fd));
    if (this->isOper(parse[1]))
		return ;
    else if (parse[1] != this->_fd_users[fd]->getNick())
		return (send_error("", "OPER :Thats not your nick", fd));
    else if (parse[2] == this->_password_oper)
    {
        this->_opers.push_back(this->_fd_users[fd]);
        return (send_reply(RPL_YOUREOPER, ":You are now an IRC operator", this->_fd_users[fd]));
    }
    else
		return (send_error(ERR_PASSWDMISMATCH, ":Password incorrect", fd));
}


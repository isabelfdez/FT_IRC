#include "../server/Server.hpp"

void    Server::exit_command(std::vector<std::string> const& parse, User *usr)
{
    if (parse.size() < 2)
        return (send_reply(ERR_NEEDMOREPARAMS, " EXIT :Not enough parameters", usr));
    if (!usr->getmode('o'))
        return (send_reply(ERR_CHANOPRIVSNEEDED, " EXIT :You are not channel operator", usr));
    else if (parse[1] != this->_password_oper)
        return  (send_reply(ERR_PASSWDMISMATCH, " EXIT :Password incorrect", usr));
    else
    {
        this->~Server();
        exit(0);
    }

}

#include "../server/Server.hpp"

void	Server::oper_command(std::vector<std::string> const & parse, User * usr)
{

  if (parse.size() < 3)
    return (send_error(ERR_NEEDMOREPARAMS, "OPER :Not enough parameters", usr));
  if (usr->getmode('o'))
    return ;
  else if (parse[1] != usr->getNick())
    return (send_error(ERR_USERSDONTMATCH, "OPER :Cannot change mode for other users", usr));
  else if (parse[2] == this->_password_oper)
  {
    this->deleteBan(usr);
    usr->setmode('o', true);
    return (send_reply(RPL_YOUREOPER, " :You are now an IRC operator", usr));
  }
  else
    return (send_error(ERR_PASSWDMISMATCH, " :Password incorrect", usr));
}


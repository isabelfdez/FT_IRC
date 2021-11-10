#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::invite_command(char * str, int & fd)
{
	std::vector<std::string>	parse;
	std::string					s;
	std::list<User *>::iterator it;
	int							deliver_fd = 0;
	User						*usr = this->_fd_users[fd];

    parse = split(str, ' ');
	Channel						*chann = this->_name_channel[parse[2]];
	if (parse.size() < 2)
        return (send_error(ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters", fd));
	else if (!this->isChannel(parse[2]))
        return (send_error(ERR_NOSUCHCHANNEL, "INVITE :No such channel", fd));
	for (it = this->_connected_users.begin(); it != this->_connected_users.end(); ++it)
	{
		if (ft_toupper((*it)->getNick()) == ft_toupper(parse[1]))
		{
			deliver_fd = (*it)->getsockfd();
			break ;
		}
	}
	if (deliver_fd == 0)
    	return (send_error(ERR_NOSUCHNICK, parse[1] + " :No such nick", fd));
	else if (chann->isUser(parse[1]))
    	return (send_error(ERR_USERONCHANNEL, parse[1] + " " + parse[2] + " :is already on channel", fd));
	else if (!chann->isInvite())
	{
		s = "You are invite to join " + parse[2];
		send_message(s, deliver_fd, usr);
		send_reply(RPL_INVITING, " :" + parse[2] + " " + parse[1], usr);
		return ;
	}
	else if (!usr->getmode('o') && !chann->isOp(usr))
        return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not a channel operator", fd));
	else
	{
		if (chann->isInvited(parse[1]))
			return;
		s = "You are invite to join " + parse[2];
		send_message(s, deliver_fd, usr);
		send_reply(RPL_INVITING, " :" + parse[2] + " " + parse[1], usr);
		chann->pushInvite(getUserWithNick(parse[1]));
	}
}

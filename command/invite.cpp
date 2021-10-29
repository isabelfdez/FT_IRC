#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::invite_command(char * str, int & fd)
{
	std::vector<std::string>	parse;
	std::string					s;
	std::list<User *>::iterator it;
	int							deliver_fd = 0;

    parse = split(str, ' ');
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
	else if (this->_name_channel[parse[2]]->isUser(parse[1]))
    	return (send_error(ERR_USERONCHANNEL, parse[1] + " " + parse[2] + " :is already on channel", fd));
	else if (!this->_name_channel[parse[2]]->isInvite())
	{
		s = "You are invite to join " + parse[2];
		send_message(s, deliver_fd, this->_fd_users[fd]);
		send_reply(RPL_INVITING, parse[2] + " " + parse[1], this->_fd_users[fd]);
		return ;
	}
	else if (!isOper(this->_fd_users[fd]->getNick()) && !this->_name_channel[parse[2]]->isOp(this->_fd_users[fd]))
        return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not a channel operator", fd));
	else
	{
		if (this->_name_channel[parse[2]]->isInvited(parse[1]))
			return;
		s = "You are invite to join " + parse[2];
		send_message(s, deliver_fd, this->_fd_users[fd]);
		send_reply(RPL_INVITING, parse[2] + " " + parse[1], this->_fd_users[fd]);
		this->_name_channel[parse[2]]->pushInvite(getUserWithNick(parse[1]));
	}
}

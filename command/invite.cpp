#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::invite_command(std::vector<std::string> const & parse, User *usr)
{
	std::string		s;
	int				flag = 0;

	if (parse.size() < 3)
        return (send_error(ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters", usr));
	else if (!this->isChannel(parse[2]))
        return (send_error(ERR_NOSUCHCHANNEL, "INVITE :No such channel", usr));
	Channel						*chann = this->_name_channel[parse[2]];

	list_user_it				start = this->_connected_users.begin();
	list_user_it				end = this->_connected_users.end();
	for (; start != end; ++start)
	{
		if (ft_toupper((*start)->getNick()) == ft_toupper(parse[1]))
		{
			flag = 1;
			break ;
		}
	}
	if (flag == 0)
    	return (send_error(ERR_NOSUCHNICK, parse[1] + " :No such nick", usr));
	User * usr_dest = this->getUserWithNick(parse[1]);
	if (!chann->isUser(usr->getNick()))
    	return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not allow to invite", usr));
	else if (chann->isUser(parse[1]))
    	return (send_error(ERR_USERONCHANNEL, parse[1] + " " + parse[2] + " :is already on channel", usr));
	else if (!chann->isInvite())
	{
		send_message("INVITE " + parse[2] + " " + parse[1], usr_dest, usr);
		send_message("INVITE " + parse[2] + " " + parse[1], usr, usr);
		send_reply(RPL_INVITING, " :" + parse[2] + " " + parse[1], usr);
		return ;
	}
	else if (!usr->getmode('o') && !chann->isOp(usr))
        return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not allow to invite", usr));
	else
	{
		if (chann->isInvited(parse[1]))
			return;
		send_message("INVITE " + parse[2] + " " + parse[1], usr_dest, usr);
		send_message("INVITE " + parse[2] + " " + parse[1], usr, usr);
		send_reply(RPL_INVITING, " INVITE :" + parse[1] + " " + parse[2], usr);
		chann->pushInvite(getUserWithNick(parse[1]));
	}
}

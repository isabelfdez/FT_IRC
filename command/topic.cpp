#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::topic_command(char * str, int & fd)
{
	std::vector<std::string>	parse;
	std::string					message;
	char						*tmp;
	int i;

	if ( ( tmp = strchr( str, '\r' ) ) || ( tmp = strchr( str, '\n' ) ) )
	 	*tmp = 0;
	if ((tmp = strchr(str, ':')))
		message = (++tmp);
	parse = split(str, ' ');
	i = parse.size();
	if (i < 2)
		return (send_error(ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters", fd));
	if (!this->_name_channel.count(parse[1]))
		return (send_error(ERR_NOSUCHCHANNEL, parse[1] + " :No such channel", fd));
	if (!this->isOper(this->_fd_users[fd]->getNick()) && !this->_name_channel[parse[1]]->isOp(this->_fd_users[fd]))
		return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not a channel operator", fd));
	else if (message.size() == 0)
	{
		this->_name_channel[parse[1]]->setTopic("");
		this->_name_channel[parse[1]]->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " no topic\n");
		return (send_reply(RPL_NOTOPIC, parse[1] + " :No topic is set", this->_fd_users[fd]));
	}
	else
	{
		this->_name_channel[parse[1]]->setTopic(message);
		this->_name_channel[parse[1]]->sendMsgChannel(":ft_irc.com Channel " + parse[1] + " topic is : " + message + "\n");
		return (send_reply(RPL_TOPIC, parse[1] + " :" + message, this->_fd_users[fd]));
	}
}

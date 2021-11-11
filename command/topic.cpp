#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::topic_command(std::vector<std::string> const & parse, User * usr)
{
	if (parse.size() < 2)
		return (send_error(ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters", usr));
	else if (!this->isChannel(parse[1]))
		return (send_error(ERR_NOSUCHCHANNEL, parse[1] + " :No such channel", usr));
	Channel * chann = this->_name_channel[parse[1]];
	if (parse.size() == 2)
		return (send_reply(RPL_TOPIC, " TOPIC : " + chann->getTopic(), usr));
	if (!usr->getmode('o') && !chann->isOp(usr))
		return (send_error(ERR_CHANOPRIVSNEEDED, parse[2] + " :You are not a channel operator", usr));
	else if (parse[2].size() == 0)
	{
		chann->setTopic("");
		chann->sendMsgChannel(usr->getMask() + "Channel " + parse[1] + " no topic\n", usr->getsockfd());
		return (send_reply(RPL_NOTOPIC, " :TOPIC :No topic is set", usr));
	}
	else
	{
		chann->setTopic(parse[2]);
		chann->sendMsgChannel(usr->getMask() + " Channel " + parse[1] + " topic is : " + parse[2] + "\n", usr->getsockfd());
		return (send_reply(RPL_TOPIC, " TOPIC :" + parse[2], usr));
	}
}

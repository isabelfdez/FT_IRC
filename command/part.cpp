
#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::part_channel(std::string str1, User * usr)
{
	if (this->_name_channel.count(str1))
	{
		Channel *chann = this->_name_channel[str1];
		if (chann->deleteUser(usr))
		{
			std::string messages = "has left the channel " + chann->getName();
			send_message_channel( messages , usr, chann);
		}

		usr->deleteChannel(chann);
		if (!chann->getUsers().size())
			deleteChannel(str1);
		send_reply("", " :channel " + str1 + " leaved", usr);
	}
	else
		return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", usr));
}

void	Server::part_command(std::vector<std::string> parse, User * usr)
{
	std::vector<std::string>	parse1;

	parse1 = split(parse[1], ',');
	vector_str_it start = parse1.begin();
	vector_str_it end = parse1.end();
	if (!parse1.size())
		return (send_error(ERR_NEEDMOREPARAMS, "PART :Not enough parameters", usr));
	for (; start != end; start++)
		Server::part_channel(*start, usr);
}
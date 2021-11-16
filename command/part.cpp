
#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::part_channel(std::string str1, std::string message, User * usr)
{
	if (this->_name_channel.count(str1))
	{
		Channel *chann = this->_name_channel[str1];
		if (chann->deleteUser(usr))
		{
			std::string messages = "PART " + chann->getName() + " :has left the channel";

			send_message_channel( messages , usr, chann);
		}
		usr->deleteChannel(chann);
		if (!chann->getUsers().size())
			deleteChannel(str1);
		if (message.size())
        	send_message("PART " + str1 + " :" + message, usr, usr);
		else
        	send_message("PART " + str1 + " :has left the channel", usr, usr);


	}
	else
		return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", usr));
}

void	Server::part_command(std::vector<std::string> parse, User * usr)
{
	std::vector<std::string>	parse1;
	std::string 				message = "";

	if (parse.size() > 2)
		message = parse[2];
	parse1 = split(parse[1], ',');
	vector_str_it start = parse1.begin();
	vector_str_it end = parse1.end();
	if (!parse1.size())
		return (send_error(ERR_NEEDMOREPARAMS, "PART :Not enough parameters", usr));
	for (; start != end; start++)
		Server::part_channel(*start, message, usr);
}
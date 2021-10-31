
#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::part_channel(std::string str1, int & fd)
{
	if (this->_name_channel.count(str1))
	{
		this->_name_channel[str1]->deleteUser(this->_fd_users[fd]);
		this->_fd_users[fd]->deleteChannel(this->_name_channel[str1]);
		if (!this->_name_channel[str1]->getUsers().size())
			deleteChannel(str1);
	}
	else
		return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
}

void	Server::part_command(char * str, int & fd)
{
	std::vector<std::string>	parse;

	str = str + 4;
		while (*str == ' ')
		str++;
	if (*str == ':')
		str++;
	parse = split(str, ',');
	if (!parse.size())
	{
		return (send_error(ERR_NEEDMOREPARAMS, "PART :Not enough parameters", fd));
	}
	for (std::vector<std::string>::iterator it = parse.begin(); it != parse.end(); it++)
		Server::part_channel(*it, fd);
}
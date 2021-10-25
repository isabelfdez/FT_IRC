
#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::part_channel(std::string str1, int & fd)
{
	int j = 1;

	std::cout << str1 << std::endl;

	if (str1[0] != '#')
		return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
	while (str1[j])
	{
		if (!ft_isalnum(str1[j]) && !ft_isspecial(str1[j]) && str1[j] != '-')
			return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
		j++;
	}
	if (str1.size() > 12)
		return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
	else if (this->_name_channel.count(str1))
	{
		this->_name_channel[str1]->deleteUser(this->_fd_users[fd]);
		this->_fd_users[fd]->deleteChannel(this->_name_channel[str1]);
		if (!this->_name_channel[str1]->getUsers().size())
			deleteChannel(str1);
	}
	else
		return (send_error(ERR_NOTONCHANNEL,  ":You're not on that channel", fd));
}

void	Server::part_command(char * str, int & fd)
{
	std::vector<std::string>	parse;

	std::cout << str << std::endl;
	str = str + 4;
		while (*str == ' ')
		str++;
	if (*str == ':')
		str++;
	parse = split(str, ',');
	std::cout << parse[0] << std::endl;
	if (!parse.size())
	{
		return (send_error(ERR_NEEDMOREPARAMS, "PART :Not enough parameters", fd));
	}
	for (std::vector<std::string>::iterator it = parse.begin(); it != parse.end(); it++)
		Server::part_channel(*it, fd);
}
#include "../server/Server.hpp"
#include "../utils.hpp"

void    Server::join_channel(std::string str1, int & fd)
{

    int j = 1;
    std::string s;

    if (str1 == "0")
    {
        str1 = "part " +  this->_fd_users[fd]->getChannelsString();
        return (part_command(&*str1.begin(), fd));
    }
    else if (str1[0] != '#')
        return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
    if (str1.size() > 12)
        return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
    while (str1[j])
    {
        if (!ft_isalnum(str1[j]) && !ft_isspecial(str1[j]) && str1[j] != '-')
            return (send_error(ERR_NOSUCHCHANNEL, str1 + " :No such channel", fd));
        j++;
    }
    if (this->_fd_users[fd]->getMaxChannels())
        return (send_error(ERR_TOOMANYCHANNELS, this->_fd_users[fd]->getNick() + " :You have joined too many channels", fd));
    if (this->_name_channel.count(str1) && this->_name_channel[str1]->getIsFull())
        return (send_error(ERR_CHANNELISFULL, str1 + " :Cannot join channel (+l)", fd));
    if (this->_name_channel.count(str1) && this->_name_channel[str1]->isBanned(this->_fd_users[fd]))
        return (send_error(ERR_BANNEDFROMCHAN, str1 + " :Cannot join channel (+b)", fd));
    else if (this->_name_channel[str1])
    {
        if (this->_name_channel[str1]->isInvite() && !this->_name_channel[str1]->isInvited(this->_fd_users[fd]->getNick()) && !this->_fd_users[fd]->getmode('o'))
            return (send_error(ERR_INVITEONLYCHAN, str1 + " :Cannot join channel (+i)", fd));
        // User join channel
        if (this->_name_channel[str1]->isUser(this->_fd_users[fd]->getNick()))
            return (send_error("", str1 + " :You are already on channel", fd));
        s = "JOIN " + this->_name_channel[str1]->getName();
        send_message_channel(s, this->_fd_users[fd], this->_name_channel[str1]);
        this->_name_channel[str1]->addUser(this->_fd_users[fd]);
        this->_fd_users[fd]->addChannel(this->_name_channel[str1]);
        this->names_command(&this->_name_channel[str1]->getName()[0], fd);
        if (this->_name_channel[str1]->getTopic().size() > 0)
            send_reply(RPL_TOPIC, " JOIN :" + str1 + " " + this->_name_channel[str1]->getTopic(), this->_fd_users[fd]);
    }
    else
    {
        // Create and join channel
        this->_name_channel[str1] = new Channel(str1, this->_fd_users[fd]);
        this->_name_channel[str1]->addUser(this->_fd_users[fd]);
        this->_fd_users[fd]->addChannel(this->_name_channel[str1]);
        std::string messages = "JOIN :" + str1;
        send_message(messages, fd, this->_fd_users[fd]);
        this->names_command(&this->_name_channel[str1]->getName()[0], fd);
    }
}

void	Server::join_command(char * str, int & fd)
{
	std::vector<std::string> parse;

	str = str + 4;
	while (*str == ' ')
		str++;
	if (*str == ':')
		str++;
    str = trim(str);
	parse = split(str, ',');
	if (!parse.size())
		return (send_error(ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters", fd));
	for (std::vector<std::string>::iterator it = parse.begin(); it != parse.end(); it++)
    {
		Server::join_channel(*it, fd);
        this->reStartSendMsg();
    }
}

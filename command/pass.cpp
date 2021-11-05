/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 15:20:00 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/05 17:02:38 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::pass_command(char * str, int & fd)
{
	if (this->_fd_users[fd]->getPassState())
		return (send_error(ERR_ALREADYREGISTRED, " :Unauthorized command (already registered)", fd));
	str += 4;
	while (*str && (*str == ' ' || *str == '\r' || *str == '\n'))
		str++;
	if (*str == '\0')
		return (send_error(ERR_NEEDMOREPARAMS, " PASS :Not enough parameters", fd));
	std::string aux = str;
	std::string aux2 = aux.substr(0, ft_max(aux.find('\n'), aux.find('\r')));
	if (aux2 != this->getPassword())
	{
		std::cout << "aux : |" << aux2 << "|\n";
		std::cout << "pass : |" << this->getPassword() << "|\n";
		return (send_reply("", ":Incorrect password ", this->_fd_users[fd]));
	}
	else
		this->_fd_users[fd]->setPassState(true);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:22:07 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/14 17:04:56 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	find_command(std::string command, std::list<std::string> commands)
{
	for (std::list<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
		if (*it == command) // Esto no se si esta bien
			return (true);
	return (false);
}

void	send_error(std::string error, std::string str, int fd)
{
	std::string message;
	message.assign(":ft_irc.com ");
	message.append(error);
	message.append(" * ");
	message.append(str);
	message.append("\n");
    send(fd, message.c_str(), message.length(), 0);
}
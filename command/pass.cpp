/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 15:20:00 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/18 18:48:14 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../utils.hpp"

void	Server::pass_command(std::vector<std::string> const & parse, User *usr)
{
	if ( usr->getPassState() )
		return (send_error(ERR_ALREADYREGISTRED, " :Unauthorized command", usr));
	if ( parse.size() < 2 )
		return (send_error(ERR_NEEDMOREPARAMS, " PASS :Not enough parameters", usr));
	if ( parse[1] != this->getPassword() )
	{
		std::cout << "aux : |" << parse[1] << "|\n";
		std::cout << "pass : |" << this->getPassword() << "|\n";
		return ( send_reply("", ":Incorrect password ", usr) );
	}
	else
		usr->setPassState(true);
}
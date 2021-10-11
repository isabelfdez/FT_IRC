/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 16:15:03 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/11 19:26:46 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int main ()
{
	try 
	{
		Server server;

		while (42)
		{
			 server.build_select_list();
			 server.setNumReadSock();
			 if ( server.getNumReadSock() < 0 )
			 	throw Server::GlobalServerExecption();
			if ( server.getNumReadSock() == 0)
			{
			  std::cout << ".";
			//   fflush(stdout);
			}
			else
				server.attendClients();
		}
		
	}
	catch ( ... ) { }
}
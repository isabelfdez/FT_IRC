/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 16:15:03 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/12 15:13:12 by krios-fu         ###   ########.fr       */
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
			std::cout << " * " << server.getNumReadSock() << std::endl; 
			//  if ( server.getNumReadSock() < 0 )
			 	// throw Server::GlobalServerExecption();
			if ( server.getNumReadSock() == 0)
			{
			  std::cout << ".";
			  fflush(stdout);
			}
			else
			{
					std::cout << " before "<< "\n";
					server.attendClients();
					std::cout << " affter "  << "\n";
			}
		}
		
	}
	catch ( ... ) { std::cout <<" herr \n"; }

	// close()
}
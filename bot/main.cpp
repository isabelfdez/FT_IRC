#include "Bot.hpp"
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 6667

Bot *bot;
void signal_kill ( int number )
{
	if ( number == SIGINT)
	{
		std::cout << "\n[[[ forced closure ]]] \n";
		bot->~Bot();
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		std::cout << "Need more params\n";
		exit (0);
	}
	struct hostent *hs;
	struct in_addr	a;
	std::string ip;
	if (isalpha(argv[1][0]))
	{
        hs = gethostbyname(argv[1]); 
		if (hs == NULL)
		{
			std::cout << "Invalid ip\n";
			exit (0);
		}
		bcopy(*hs->h_addr_list, (char *) &a, sizeof(a));
		ip = inet_ntoa(a);
	}
	else
		ip = argv[1];
	Bot lol("lol", ip, atoi(argv[2]));
	bot = &lol;
	// FD_SET(this->_sock, &lol.getWrites());
	// lol.build_select_list();
	// lol.setNumReadSock();

	// if ( lol.getNumReadSock() > 0 )
	// {
		std::cout << "adios3\n";

		// if(FD_ISSET(lol.getSocket(), &lol.getWrites()))
		// {
			// if (connect(lol.getSocket(), ( struct sockaddr * ) &lol.getAddress(), sizeof(lol.getAddress())) < 0)
			// {
			// //	perror("Connect");
			// //	exit(EXIT_FAILURE);
			// }
			std::string data = "User lol42 * * :lol42\r\nnick lol42\r\n";
			send(lol.getSocket(), data.c_str(), data.length(), 0);
		// }
	// }


	for (;;)
	{
		lol.build_select_list();
		lol.setNumReadSock();
		signal( SIGINT, signal_kill );
		if ( lol.getNumReadSock() < 0 )
		{
			exit(EXIT_FAILURE);
		}
		if ( lol.getNumReadSock() == 0 )
		{
			std::cout << ".";
		}
		else
			lol.attendServer();
	}
	return 0;
}
#include "Bot.hpp"
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 6667



int main(int argc, char const *argv[])
{
	Bot lol("lol", "127.0.0.1", 6667);
	lol.build_select_list();
	lol.setNumReadSock();
	if ( lol.getNumReadSock() > 0 )
	{
		if(FD_ISSET(lol.getSocket(), &lol.getWrites()))
		{
			std::string data = "User lol42 * * :lol42\r\nnick lol42\r\n";
			send(lol.getSocket(), data.c_str(), data.length(), 0);
		}
	}

	for (;;)
	{
		lol.build_select_list();
		lol.setNumReadSock();
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
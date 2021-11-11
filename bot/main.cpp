#include "Bot.hpp"

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
	lol.build_select_list();
	lol.setNumReadSock();
	if ( lol.getNumReadSock() > 0 )
	{
		int byte;
		char buffer[513];
		byte = recv(lol.getSocket(), buffer, 512,0);
		buffer[byte]= '\0';
		buffer[1] = 'O';
		std::string data = buffer;
		send(lol.getSocket(), data.c_str(), data.length(), 0);
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
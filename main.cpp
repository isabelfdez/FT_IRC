#include "./server/Server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include <signal.h>


Server *svr;

void signal_kill ( int number )
{
	if ( number == SIGINT)
	{
		std::cout << "[[[SIGNAL]]] \n";
		svr->~Server();
		// kill (getpid(), 9);
		exit(EXIT_FAILURE);
	}
}
int main ()
{
	try 
	{
		Server server;
		svr = &server;
		while (42)
		{
			signal( SIGINT, signal_kill );
			server.build_select_list();
			server.setNumReadSock();
			 if ( server.getNumReadSock() < 0 )
			 	throw Server::GlobalServerExecption();
			if ( server.getNumReadSock() == 0)
			{
				std::cout << ".";
				fflush(stdout);
			}
			else
				server.attendClients();
		}
		
	}
	catch ( ... ) { std::cout <<" herr \n"; }

}
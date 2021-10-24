#include "./server/Server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include <signal.h>

Server *svr;

void signal_kill ( int number )
{
	if ( number == SIGINT)
	{
		std::cout << "\n[[[ forced closure ]]] \n";
		svr->~Server();
		kill (getpid(), 9);
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
			server.sendPing( );
			server.build_select_list();
			server.setNumReadSock();
			 if ( server.getNumReadSock() < 0 )
			 	throw Server::GlobalServerExecption();
			if ( server.getNumReadSock() == 0)
			{
				std::cout << "\r";
				displayTimestamp();
				std::cout
					<< " : Connections : " << server.getNumConnections()
					<< " Users: " << server.getNumUser()
					<< " Channel: " << server.getNumChannel();
				fflush(stdout);
			}
			else
				server.attendClients();
		}
		
	}
	catch ( ... ) {  }

	return 0;
}
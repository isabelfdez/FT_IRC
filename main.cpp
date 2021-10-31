#include "./server/Server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include <csignal>

Server *svr;

void signal_kill ( int number )
{
	if ( number == SIGINT)
	{
		std::cout << "\n[[[ forced closure ]]] \n";
		svr->~Server();
		exit(EXIT_FAILURE);
	}
}
int main ()
{
	int i = 1;
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
					if ( i == 5)
						i = 1;
					// switch (i)
					// {
					// 	case 1:
					// 		std::cout << YELLOW" / "WHITE ;
					// 		break;
					// 	case 2: 
					// 		std::cout << YELLOW" - "WHITE ;
					// 		break;
					// 	case 3:
					// 	std::cout << YELLOW" \\ "WHITE;
					// 		break;
					// 	case 4:
					// 		std::cout << YELLOW" | "WHITE;
					// 		break;
					// 	default:
					// 		break;
					// }
				i++;
				fflush(stdout);
			}
			else
				server.attendClients();
		}
		
	}
	catch ( ... ) {  }

	return 0;
}
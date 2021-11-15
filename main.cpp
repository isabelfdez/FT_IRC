#include "./server/Server.hpp"
#include "user.hpp"
#include "channel.hpp"
#include <csignal>

Server					*svr;
// HAY QUE COMPROBAR QUE EL RANGO DEL PUERTO ES BUENO

void signal_kill ( int number )
{
	if ( number == SIGINT)
	{
		std::cout << "\n[[[ forced closure ]]] \n";
		svr->~Server();
		exit(EXIT_FAILURE);
	}
}
int main (int argc, char **argv)
{
	if (argc < 2 || argc > 3)
	{
		std::cout << "Incorrect number of arguments\n";
		std::cout << "A correct input must contain the following parameters\n";
		std::cout << "./ircserv <port> or ./ircserv <port> <password>\n";
		return (0);
	}
	std::string port;
	std::string password;
	if (argc == 3)
	{
		password = argv[2];
		if (password.size() > 20)
		{
			std::cout << "Password is too long\n";
			return (0);
		}
	}
	int i = 1;
	try 
	{
		int port = get_port(argv[1]);
		Server server(port);

		svr = &server;
		if (argc == 3)
			server.setPassword(password);
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
					 switch (i)
					 {
					 	case 1:
					 		std::cout << YELLOW" / "WHITE ;
					 		break;
					 	case 2:
					 		std::cout << YELLOW" - "WHITE ;
					 		break;
					 	case 3:
					 	std::cout << YELLOW" \\ "WHITE;
					 		break;
					 	case 4:
					 		std::cout << YELLOW" | "WHITE;
					 		break;
					 	default:
					 		break;
					}
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
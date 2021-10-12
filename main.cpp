#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

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
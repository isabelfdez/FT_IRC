#include "Server.hpp"

Server::Server()
{
	std::cout << "Creating Server..." << std::endl;
	FD_ZERO(&this->_reads);
	this->_listen_server_sock = 0;
	this->_highsock = 0;
	this->_listen_server_sock = socket( AF_INET, SOCK_STREAM, 0);
	if ( this->_listen_server_sock == -1 )
		throw Server::GlobalServerExecption();
	memset( this->_list_connected_user, 0 , sizeof( this->_list_connected_user) );
	memset( (char *) &this->_addr_server, 0 , sizeof(sockaddr_in) );
	this->_addr_server.sin_port = htons(PORT);
	this->_addr_server.sin_family = AF_INET;
	this->_addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	if ( bind( this->_listen_server_sock, (struct sockaddr *) &this->_addr_server, sizeof (this->_addr_server) )  == -1)
		throw Server::GlobalServerExecption();
	if ( listen (this->_listen_server_sock, 5) == -1)
		throw Server::GlobalServerExecption();
	this->_highsock = this->_listen_server_sock;

}

Server::~Server()
{
	
}


void Server::build_select_list( void )
{
	FD_ZERO(&this->_reads);
	FD_ZERO(&this->_writes);

	
	FD_SET( this->_listen_server_sock, &this->_reads );

	for (size_t i = 0; i <FD_SETSIZE; i++)
	{
		if( this->_list_connected_user[i] != 0)
		{
			FD_SET(this->_list_connected_user[i], &this->_reads);
			FD_SET(this->_list_connected_user[i], &this->_writes);
			if (this->_list_connected_user[i] > this->_highsock )
				this->_highsock = this->_list_connected_user[i];
		}
		
	}
}

void Server::join_new_connection()
{
	int connection;

	connection = accept(this->_listen_server_sock, NULL, NULL);
	if (connection < 0)
		throw Server::GlobalServerExecption();
	// setnonblocking

	for (size_t i = 0; i < 1 && (connection != -1); i++)
	{
		if(this->_list_connected_user[i] == 0)
			std::cout << "Conenection accepted: FD:" << connection << " pos: " << i << std::endl;
		this->_list_connected_user[i] = connection;
		connection = -1;
	}
	if ( connection != -1)
	{
		std::cout << "No roon left for new clinte" << std::endl;
		close ( connection );
	}
	
}

const char* Server::GlobalServerExecption::what() const throw ()
{
	return "❌ GlobalServerExecption\n";
}


void Server::setNumReadSock( void )
{
	this->_time_out.tv_sec = 1;
	this->_time_out.tv_usec = 0;
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_reads, &this->_writes, (fd_set *) 0 , &this->_time_out);
}

void Server::attendClients()
{
	if( FD_ISSET(this->_listen_server_sock , &this->_reads) )
		this->join_new_connection();
	for (size_t i = 0; i < FD_SETSIZE; i++)
	{
		if ( FD_ISSET( this->_list_connected_user[i], &this->_reads) )
			this->getCustomerRequest( this->_list_connected_user[i], i);
		if ( FD_ISSET( this->_list_connected_user[i], &this->_writes) )
		 	this->replyCustomerRequest( this->_list_connected_user[i], i);
	}
	
}

void Server::getCustomerRequest( int & fd_client, int i)
{
	char buffer[80];

	int byte=  read(fd_client, buffer, 80);
	std::cout << "byte : " << byte << std::endl;
	if ( !byte )
	{
		close( fd_client );
		this->_list_connected_user[i] = 0;
	}
	else
		std::cout << "Request: " << static_cast<std::string >( buffer ) << std::endl;

}

void Server::replyCustomerRequest( int & fd_client, int i)
{
	send(fd_client, "hola que tal", 13, 0);
	i++;

}

int		const &	Server::getNumReadSock( void ) const { return this->_num_read_sock; }
int		const &	Server::getListenSockServer( void ) const { return this->_listen_server_sock; }
fd_set	const &	Server::getSocks( void ) const { return this->_reads; }
int		const &	Server::getHigthSock ( void ) const { return this->_highsock; }

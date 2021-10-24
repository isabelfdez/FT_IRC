/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:29:16 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/24 17:02:39 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../utils.hpp"

static void setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

Server::Server(): _fd_users(), _name_channel()
{
	std::cout << "Creating Server..." << std::endl;
	FD_ZERO(&this->_reads);
	this->_listen_server_sock = 0;
	this->_highsock = 0;
	this->_listen_server_sock = socket( AF_INET, SOCK_STREAM, 0);
	if ( this->_listen_server_sock == -1 )
	{
		perror("Socket");
		throw Server::GlobalServerExecption();
	}
	setnonblocking( this->_listen_server_sock );
	memset( this->_list_connected_user, 0 , sizeof( this->_list_connected_user) );
	memset( (char *) &this->_addr_server, 0 , sizeof(this->_addr_server) );
	this->_addr_server.sin_port = htons(PORT);
	this->_addr_server.sin_family = AF_INET;
	this->_addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	if ( bind( this->_listen_server_sock, ( struct sockaddr * ) &this->_addr_server, sizeof( this->_addr_server ) )  == -1 )
	{	
		perror("Bind");
		throw Server::GlobalServerExecption();
	}
	if ( listen (this->_listen_server_sock, 5) == -1)
	{
		perror("Listen");
		throw Server::GlobalServerExecption();
	}
	this->_highsock = this->_listen_server_sock;
	this->_commands.push_back("USER");
	this->_commands.push_back("NICK");
	this->_commands.push_back("JOIN");
	this->_commands.push_back("PRIVMSG");
	this->_commands.push_back("NOTICE");
	this->_commands.push_back("PART");
	this->_commands.push_back("QUIT");
	this->_commands.push_back("user");
	this->_commands.push_back("nick");
	this->_commands.push_back("join");
	this->_commands.push_back("privmsg");
	this->_commands.push_back("notice");
	this->_commands.push_back("part");
	this->_commands.push_back("quit");

	//this->_channel.push_back( new Channel("42") );     No entiendo esta linea
}




Server::~Server()
{
	close( this->_listen_server_sock );
	close_all_fd();
	FD_ZERO( &this->_reads );
	memset( this->_list_connected_user, 0 , sizeof( this->_list_connected_user ) );
	memset( (char *) &this->_addr_server, 0 , sizeof( this->_addr_server ) );
	std::cout << "Destructor Server\n";
}


void Server::build_select_list( void )
{
	FD_ZERO(&this->_reads);

	FD_SET( this->_listen_server_sock, &this->_reads );

	for (size_t i = 0; i < FD_SETSIZE; i++)
	{
		if( this->_list_connected_user[i] != 0)
		{
			FD_SET(this->_list_connected_user[i], &this->_reads);
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
	setnonblocking( connection );
	for (size_t i = 0; i < FD_SETSIZE && (connection != -1); i++)
	{
		if(this->_list_connected_user[i] == 0)
		{
			std::cout << std::endl;
			displayTimestamp();
			std::cout << " : Connection accepted, IP: " << this->getIpUser() << " Socket: " << connection;
			this->_list_connected_user[i] = connection;
			this->_fd_users[connection] =  new User(connection);
			this->_fd_users[ this->_list_connected_user[i] ]->setLastTime( getTime() );
			connection = -1;
		}
	}
	if ( connection != -1)
	{
		std::cout << "No roon left for new client";
		close_fd ( connection );
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
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_reads, (fd_set *) 0, (fd_set *) 0 , &this->_time_out);
}

void Server::attendClients()
{
	if( FD_ISSET(this->_listen_server_sock , &this->_reads) )
		this->join_new_connection();
	for (size_t i = 0; i < FD_SETSIZE; i++)
	{
		if ( FD_ISSET( this->_list_connected_user[i], &this->_reads) )
		{
			std::cout << std::endl;
			displayTimestamp();
			std::cout << " : Attend client,       IP: " << this->getIpUser()  << " Socket: " << this->_list_connected_user[i] << " CMD : ";
			this->_fd_users[ this->_list_connected_user[i] ]->setLastTime( getTime() );
			this->getCustomerRequest( this->_list_connected_user[i], i );
		}
	}
	std::cout << std::endl;
}


 //  <user> <mode> <unused> <realname>  (int fd, std::string buff, char * str); error list add 





void Server::parse_command(int fd, std::string buff, char * str)
{
	while (*str == ' ')
	{
		buff.erase(buff.begin());
		str++;
	}
	std::string buff2 = buff.substr(0, buff.find('\r'));
	std::string command = buff2.substr(0, buff2.find(' '));
	std::cout << command; // no deleted krios-fu
	if (!find_command(command, this->_commands))
		return ; // Aquí tal vez haya que imprimir \r\n
	if (!this->_fd_users[fd]->getRegistered())
	{
		// Si el usuario no está registrado, solo se puede llamar a los comandos
		// USER o NICK, y no puede llamar a USER varias veces seguidas
		if (command != "USER" && command != "NICK" &&
			command != "user" && command != "nick")
			send_error(ERR_NOTREGISTERED, ":You have not registered", fd);
		else if ((command == "USER" || command == "user") && this->_fd_users[fd]->getUserName().size() > 0)
			send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
		else if (command == "USER" || command == "user")
			this->user_command(fd, str);
		else if (command == "NICK" || command == "nick")
			this->nick_command(str, fd);
	}
	else if ( this->_fd_users[fd]->getRegistered() )
	{
		if ((command == "USER" || command == "user"))
			send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", fd);
		else if (command == "NICK" || command == "nick")
			this->nick_command(str, fd);
		else if (command == "JOIN" || command == "join")
			join_command(str, fd);
		else if (command == "PRIVMSG" || command == "privmsg")
			this->privmsg_command(buff2, fd);
		//else if (command == "NOTICE" || command == "notice")
		//	notice_command();
		else if (command == "PART" || command == "part")
			part_command(str, fd);
		else if (command == "QUIT" || command == "quit")
		{
			this->quit_command(fd, str);

		}
	}
}

void Server::getCustomerRequest( int & fd_client, int i)
{
	char		buffer[512];
	memset(buffer, 0, sizeof(buffer));
	int byte = recv(fd_client, buffer, 512, 0);
	//char	*aux;
	//std::string	buff;
	//arreglar!
	//int byte = recv(fd_client, buffer, 9, 0);
	//buff += buffer;
//
	//i++;
	//if ( !byte )
	//{
	//	close( fd_client );
	//	this->_list_connected_user[i] = 0;
	//}
	//while (byte == 9)
	//{
//
	//	std::cout << "byte = " << byte << std::endl;
	//	byte = recv(fd_client, buffer, 9, 0);
	//	buff += buffer;
	//}
	//std::cout << "byte = " << byte << std::endl;
	//std::cout << "se sale\n";
	//std::cout << "buffer: " <<  buffer << std::endl;
	if ( !byte )
	{
		close( fd_client );
		this->_list_connected_user[i] = 0;
	}
	else
	{
		std::string buff2 (buffer);	
		this->parse_command(fd_client, buff2, buffer);
	}
}

int		const &	Server::getNumReadSock( void ) const { return this->_num_read_sock; }
int		const &	Server::getListenSockServer( void ) const { return this->_listen_server_sock; }
fd_set	const &	Server::getSocks( void ) const { return this->_reads; }
int		const &	Server::getHigthSock ( void ) const { return this->_highsock; }


size_t			Server::getNumChannel( void ) 	const{ return this->_name_channel.size(); }
size_t			Server::getNumConnections( void )		const{ return this->_fd_users.size(); }
size_t			Server::getNumUser( void )		const{ return this->_connected_users.size(); }





void Server::close_fd(int fd)
{
	for ( size_t i = 0; i <FD_SETSIZE; i++ )
		if( this->_list_connected_user[i] == fd)
			this->_list_connected_user[i] = 0;
	close (fd);
	std::cout << std::endl;
	displayTimestamp();
	std::cout << " : Connection close,    IP: " << this->getIpUser() << " Socket: " << fd;
}

void Server::close_all_fd()
{
	std::map<int, User *>::iterator start = this->_fd_users.begin();
	std::map<int, User *>::iterator end = this->_fd_users.end();

	for (; start != end; ++start )
	{
		std::cout << "Cerrando fd: " << start->first << std::endl;
		close( start->first );
		FD_CLR( start->first, &this->_reads );
	}

}


std::string Server::getIpUser( void ) const 
{
	struct in_addr clientIP;
	clientIP = this->_addr_server.sin_addr;
	char ipStr[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, &clientIP, ipStr, INET_ADDRSTRLEN);
}
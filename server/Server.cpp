/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 15:49:33 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/11 19:09:30 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	std::cout << "Creating Server..." << std::endl;
	this->_listen_server_sock = socket( AF_INET, SOCK_STREAM, 0);
	if ( this->_listen_server_sock == -1 )
		throw Server::GlobalServerExecption();
	memset( &this->_addr_server, 0 , sizeof(sockaddr_in) );
	this->_addr_server.sin_port = PORT;
	this->_addr_server.sin_family = AF_INET;
	this->_addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	if ( bind( this->_listen_server_sock, (struct sockaddr *) &this->_addr_server, sizeof (this->_addr_server) )  == -1)
		throw Server::GlobalServerExecption();
	if ( listen (this->_listen_server_sock, 5) == -1)
		throw Server::GlobalServerExecption();
	this->_highsock = this->_listen_server_sock;
	std::cout << "Server Succes" << std::endl;
}

Server::~Server()
{
	
}


void Server::build_select_list( void )
{
	FD_ZERO(&this->_socks);
	
	FD_SET( this->_listen_server_sock, &this->_socks );

	for (size_t i = 0; i < this->_list_connected_users.size(); i++)
	{
		if( this->_list_connected_users.at(i) != 0)
		{
			FD_SET(this->_list_connected_users.at(i), &this->_socks);
			if (this->_list_connected_users.at(i) > this->_highsock )
				this->_highsock = this->_list_connected_users.at(i);
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

	for (size_t i = 0; i < this->_list_connected_users.size() + 1 && (connection != -1); i++)
	{
		if(this->_list_connected_users.at(i) == 0)
			std::cout << "Conenection accepted: FD:" << connection << " pos: " << i << std::endl;
		this->_list_connected_users[i] = connection;
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
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_socks, (fd_set * ) 0, (fd_set *) 0 , NULL);
}

void Server::attendClients()
{
	if( FD_ISSET(this->_listen_server_sock , &this->_socks) )
		this->join_new_connection();
	
	for (size_t i = 0; i < this->_list_connected_users.size(); i++)
	{
		if ( FD_ISSET( this->_list_connected_users.at(i), &this->_socks) )
			std::cout << i <<" client ask for answer " << std::endl;
	}
	
}

void Server::getCustomerRequest()
{
	std::string _request;
	char *cur_char;

	// if ( sock_gets() )
}

int		const &	Server::getNumReadSock( void ) const { return this->_num_read_sock; }
int		const &	Server::getListenSockServer( void ) const { return this->_listen_server_sock; }
fd_set	const &	Server::getSocks( void ) const { return this->_socks; }
int		const &	Server::getHigthSock ( void ) const { return this->_highsock; }

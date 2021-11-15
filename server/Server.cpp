/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:29:16 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/15 15:34:45 by krios-fu         ###   ########.fr       */
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

Server::Server(int port): _fd_users(), _name_channel()
{
	int yes = 1;
	std::cout << "Creating Server..." << std::endl;
	FD_ZERO(&this->_reads);
	FD_ZERO(&this->_writes);
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
	this->_addr_server.sin_port = htons(port);
	this->_addr_server.sin_family = AF_INET;
	this->_addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(this->_listen_server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if ( bind( this->_listen_server_sock, ( struct sockaddr * ) &this->_addr_server, sizeof( this->_addr_server ) )  == -1 )
	{	close(this->_listen_server_sock);
		perror("Bind");
		throw Server::GlobalServerExecption();
	}
	if ( listen (this->_listen_server_sock, FD_SETSIZE) == -1)
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
	this->_commands.push_back("PONG");
	this->_commands.push_back("MODE");
	this->_commands.push_back("OPER");
	this->_commands.push_back("KICK");
	this->_commands.push_back("INVITE");
	this->_commands.push_back("TOPIC");
	this->_commands.push_back("LIST");
	this->_commands.push_back("NAMES");
	this->_commands.push_back("PASS");
	this->_commands.push_back("WHO");
	this->_commands.push_back("ISON");


	this->_password_oper = "abracadabra";
	this->_name_channel.clear();
	//this->_channel.push_back( new Channel("42") );     No entiendo esta linea
}




Server::~Server()
{
	std::cout << "Destructor Server\n";
	close( this->_listen_server_sock );
	close_all_fd();
	close( this->_listen_server_sock );
	FD_ZERO( &this->_reads );
	memset( this->_list_connected_user, 0 , sizeof( this->_list_connected_user ) );
	memset( (char *) &this->_addr_server, 0 , sizeof( this->_addr_server ) );
}

void Server::build_select_list( void )
{
	typedef std::deque<User *>::iterator it_user;
	
	it_user start = this->_send_message.begin();
	it_user end = this->_send_message.end();
	
	FD_ZERO( &this->_reads );
	FD_ZERO( &this->_writes );
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
	for (; start != end; ++start)
		FD_SET( (*start)->getsockfd(), &this->_writes);
}



void Server::join_new_connection()
{
	int connection;

	struct sockaddr_in addr_client ;
	
	memset( (char *) &addr_client, 0 , sizeof(addr_client) );
	socklen_t len_sock = sizeof( addr_client);

	connection = accept(this->_listen_server_sock, ( struct sockaddr * ) &addr_client, &len_sock);
	if ( connection < 0 )
		throw Server::GlobalServerExecption();
	
	setnonblocking( connection );
	for (size_t i = 0; i < FD_SETSIZE && (connection != -1); i++)
	{
		if(this->_list_connected_user[i] == 0)
		{
			this->_list_connected_user[i] = connection;
			this->_fd_users[connection] =  new User( connection, addr_client );
			if (!this->getPassword().size())
				this->_fd_users[connection]->setPassState(1);
			else
				this->_fd_users[connection]->setPassState(0);
			this->_fd_users[ this->_list_connected_user[i] ]->setLastTime( getTime() );
			std::cout << "\r";
			displayLog("Connection accepted", "", this->_fd_users[connection]);
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
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_reads, &this->_writes, (fd_set *) 0 , &this->_time_out);
}

void Server::setPassword(std::string pass) { this->_password = pass; }

void Server::attendClients()
{
	typedef std::deque<User *>::iterator it_user;
	
	it_user start = this->_send_message.begin();
	it_user end = this->_send_message.end();
	
	for (; start != end; ++start )
		if ( FD_ISSET( (*start)->getsockfd(), &this->_writes) )
			this->sendRequest( *start );

	if( FD_ISSET(this->_listen_server_sock , &this->_reads) )
		this->join_new_connection();
	for (size_t i = 0; i < FD_SETSIZE; i++)
	{
		if ( FD_ISSET( this->_list_connected_user[i], &this->_reads) )
		{
			 this->_fd_users[this->_list_connected_user[i]]->setLastTime( getTime() ); 
			std::cout << "\r";
			this->getCustomerRequest( this->_list_connected_user[i]);
		}

	}
}


void Server::parse_command(int fd, std::string buffer)
{
	std::string	command;
	std::string	buff2;

	User *user = this->_fd_users[fd];


	std::vector<std::string> parse = parser( buffer );


	command = ft_toupper(parse[0]);



	displayLog("Attend client", " CMD: " + command, user);
	
	if (!find_command(command, this->_commands))
		return send_error(ERR_UNKNOWNCOMMAND, command + " :Unknowm command", user);
	if (!user->getRegistered())
	{
		// Si el usuario no está registrado, solo se puede llamar a los comandos
		// PASS, USER o NICK, y no puede llamar a USER ni a PASS varias veces seguidas
		if ( command != "USER" && command != "NICK" && command != "PASS" )
			send_error(ERR_NOTREGISTERED, ":You have not registered", user);
		else if ( (command == "USER") && user->getUserName().size() > 0)
			send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", user);
		else if (command == "USER" )
			this->user_command(parse, user);
		else if (command == "NICK" )
		 	this->nick_command(parse, user);
		// else if (command == "PASS" )
		// 	this->pass_command(parse, user);
	}
	else if ( user->getRegistered() )
	{
	if ((command == "USER" || command == "user"))
		send_error(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)", user);
	else if ( command == "NICK" )
		this->nick_command(parse, user);
	else if ( command == "JOIN" )
		join_command(parse, user);
	else if ( command == "PRIVMSG" )
		this->privmsg_command(parse, user);
	else if ( command == "NOTICE" )
        this->notice_command(parse, user);
	else if ( command == "PART" )
		part_command(parse, user);
	else if ( command == "QUIT" )
		this->quit_command(parse, user);
		else if ( command == "PONG" )
		this->pong_command(parse, user);
	else if ( command == "MODE" )
		this->mode_command(parse, user);
	else if ( command == "OPER" )
		this->oper_command(parse, user);
	else if ( command == "KICK" )
		this->kick_command(parse, user);
	else if ( command == "INVITE" )
	 	this->invite_command(parse, user);
	else if ( command == "TOPIC" )
		this->topic_command(parse, user);
	else if ( command == "LIST" )
		this->list_command(parse, user);
	else if ( command == "NAMES" )
		this->names_command(parse, user);
	else if ( command == "WHO" )
		this->who_command(parse, user);
	else if ( command == "ISON" )
		this->ison_command(parse, user);
	}
}

void Server::getCustomerRequest( int fd_client )
{
	char		buffer[513];
	std::string	tmp;
	std::string tmp2;
	User		*usr;
	int 		byte;
	size_t		pos;
	
	usr = this->_fd_users[ fd_client ];

	tmp = usr->getBufferCmd();

	while ( (byte = recv(fd_client, buffer, 512, 0)) > 0)
	{
		buffer[byte] = '\0';
		tmp += buffer;
	}

	if ( tmp.length() == 0 )
		this->deleteUser( usr , "[Signed off]");

	while ( tmp.length() )
	{
		if (( pos = tmp.find('\n') ) != std::string::npos )
		{
			tmp2 = tmp.substr(0, pos + 1);
			tmp.erase(0, pos + 1);

			if (tmp2.length() > 510 )
				tmp2 = tmp2.substr(0, 510);
			if ( tmp2[0] != '\r' && tmp2[0] != '\n')
			{
				this->parse_command( fd_client, tmp2);
			}
			usr->setBufferCmd("");
		}
		else
		{
			std::string buffer_cmd = usr->getBufferCmd() + tmp;
			usr->setBufferCmd( buffer_cmd );
			tmp.clear();
		}
	}

	this->reStartSendMsg();
}

std::string Server::getPassword(void) const { return (this->_password); }

bool			Server::isUsr(std::string usr)
{
	for (std::list<User*>::iterator it = _connected_users.begin(); it != _connected_users.end(); it++)
	{
		if ((*it)->getNick() == usr)
			return true;
	}
	return false;
}


bool			Server::isChannel(std::string channel)
{
	if (this->_name_channel.count(channel))
		return true;
	return false;
}




int		const &	Server::getNumReadSock( void ) const { return this->_num_read_sock; }
int		const &	Server::getListenSockServer( void ) const { return this->_listen_server_sock; }
fd_set	const &	Server::getSocks( void ) const { return this->_reads; }
int		const &	Server::getHigthSock ( void ) const { return this->_highsock; }


size_t			Server::getNumChannel( void ) 	const{ return this->_name_channel.size(); }
size_t			Server::getNumConnections( void )		const{ return this->_fd_users.size(); }
size_t			Server::getNumUser( void )		const{ return this->_connected_users.size(); }

User			*Server::getUserWithNick(std::string nick)
{
	for (std::list<User*>::iterator it = this->_connected_users.begin(); it != this->_connected_users.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return (*it);
	}
	return nullptr;
}


void Server::close_fd(int fd)
{
	for ( size_t i = 0; i <FD_SETSIZE; i++ )
		if( this->_list_connected_user[i] == fd)
			this->_list_connected_user[i] = 0;
	close (fd);
	std::cout << "\r";
	displayLog("Connection closed", "", this->_fd_users[fd]);

}

void Server::close_all_fd()
{
	typedef std::map<int, User *>::iterator iterator_usr;

	iterator_usr start = this->_fd_users.begin();
	iterator_usr end = this->_fd_users.end();

	for (; start != end; ++start )
	{
		std::cout << "\r";
		displayLog("Connection closed", "", this->_fd_users[start->first ] );
		std::cout << std::endl;
		close( start->first );
		FD_CLR( start->first, &this->_reads );
	}

}

void			Server::deleteChannel( std::string channel )
{
	std::map<std::string, Channel*>::iterator it;

	it = this->_name_channel.find(channel);
	if (it != this->_name_channel.end())
	{
		delete this->_name_channel[channel];
		this->_name_channel.erase(it);
	}
}

void Server::deleteUser( User * usr, std::string const & _messages)
{
	typedef std::list<Channel *>::iterator iteratorChannel;

	iteratorChannel channel = usr->getChannels().begin();
	iteratorChannel end = usr->getChannels().end();
	
	for (; channel != end; ++channel )
	{
		// this->part_channel((*channel)->getName(), usr );
		if ((*channel)->deleteUser( usr ))
		{
			std::string messages = "PART " + (*channel)->getName() + " :"+_messages;
			send_message_channel( messages , usr, (*channel));
		}
		if (!(*channel)->getUsers().size())
			this->deleteChannel( (*channel)->getName() );
	}

	this->_connected_users.remove( usr );
	this->close_fd( usr->getsockfd() );
	this->_fd_users.erase( usr->getsockfd() );
	displayLog("Quit success", usr->getNick(), usr);
	this->_nicks.remove( usr->getNick() );
	this->deleteBan( usr );
	this->deleteInvite(usr);
	this->deleteDequeUser(usr);
	delete usr;
}

void	Server::deleteBan( User *  user)
{
	for (std::map<std::string, Channel *>::iterator	it = _name_channel.begin(); it != _name_channel.end(); it++)
	{
		(*(it->second)).banOff(user->getMask());
	}
}

void	Server::deleteInvite( User *user)
{
	for (std::map<std::string, Channel *>::iterator	it = _name_channel.begin(); it != _name_channel.end(); it++)
	{
		(*(it->second)).inviteOff(user);
	}
}

void		Server::reStartSendMsg()
{
	typedef std::map<int, User *>::iterator it_user;
	User *usr;

	it_user start = this->_fd_users.begin();
	it_user end = this->_fd_users.end();
	for (; start != end ; ++start )
	{
		usr = start->second;
		usr->setIsSendMsg( false );
	}
}



void Server::welcome( int const & fd )
{
	User *usr = this->_fd_users[ fd ];
	std::string part1 = "           / ____/   / / / /   /   | /_  __/          / // /  |__ \\ ";
	std::string part2 = "          / /       / /_/ /   / /| |  / /            / // /_  __/ / ";
	std::string part3 = "         / /___    / __  /   / ___ | / /            /__  __/ / __/      ";
	std::string part4 = "         \\____/   /_/ /_/   /_/  |_|/_/               /_/   /____/ ";
	std::string part5 = "                      / ____/ /_  __/     /  _/   / __ \\  / ____/ ";
	std::string part6 = "                     / /_      / /        / /    / /_/ / / /      ";
	std::string part7 = "                    / __/     / /       _/ /    / _, _/ / /___   ";
	std::string part8 = "                   /_/       /_/ ______/___/   /_/ |_|  \\____/";
	std::string part9 = "                                /_____/                           ";


	
	std::string part10 = "         Welcome: " + usr->getNick();
	
	send_reply(RPL_WELCOME, " :Welcome to the ft_irc Network " + usr->getNick() + "!" + usr->getUserName() + "@"+ this->_fd_users[fd]->getIp() , usr);
	send_reply(RPL_YOURHOST, " :Your host is ft_irc.com, running version v:0.42",  this->_fd_users[fd]);
	send_reply(RPL_CREATE, " :This server was creates 12:42:42 Oct 14 2021", this->_fd_users[fd]);
	send_reply(RPL_MYINFO, " :ft_irc v:0.42 io iobl", this->_fd_users[fd]);
	send_reply(RPL_MOTDSTART, " :ft_irc.com message of the day", this->_fd_users[fd]);
	send_reply(RPL_MOTD, " :" , usr);
	send_reply(RPL_MOTD, " :" + part1, usr);
	send_reply(RPL_MOTD, " :" + part2, usr);
	send_reply(RPL_MOTD, " :" + part3, usr);
	send_reply(RPL_MOTD, " :" + part4, usr);
	send_reply(RPL_MOTD, " :" , usr);
	send_reply(RPL_MOTD, " :" + part5, usr);
	send_reply(RPL_MOTD, " :" + part6, usr);
	send_reply(RPL_MOTD, " :" + part7, usr);
	send_reply(RPL_MOTD, " :" + part8, usr);
	send_reply(RPL_MOTD, " :" + part9, usr);
	send_reply(RPL_MOTD, " :" + part10, usr);
	send_reply(RPL_ENDOFMOTD, " :End of message of the day", this->_fd_users[fd]);

}

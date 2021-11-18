/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:33:37 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/18 18:56:19 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <arpa/inet.h>

Bot::Bot(std::string const & nick, std::string const & IP,int const & port )
	: _nick(nick)
{
	this->_sock = 0;
	if ((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	this->_highsock = this->_sock;
	memset(&this->_addr, 0, sizeof(this->_addr));

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	std::cout << IP << std::endl;
	
	if (inet_pton(AF_INET, IP.c_str(), &this->_addr.sin_addr) <= 0)
	{
		perror("INET_PTON");
		exit(EXIT_FAILURE);
	}
	if (connect(this->_sock, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) < 0)
	{
		perror("Connect");
		exit(EXIT_FAILURE);
	}

}

char const  *  Bot::getIp(  unsigned long ip ) 
{
	struct in_addr clientIP;
	this->_send.sin_addr.s_addr = ip;
	clientIP = this->_send.sin_addr;
	char ipStr[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, &clientIP, ipStr, INET_ADDRSTRLEN);
}


unsigned long  Bot::getIpLong() 
{
	char				hostname[64];
	struct	hostent*	host;

	gethostname(hostname, 64);
	host = gethostbyname(hostname);
	if (!host)
		return ntohl(inet_addr("127.0.1.1"));
	 return  ntohl(inet_addr(inet_ntoa(*((struct in_addr*)host->h_addr_list[0]))));
}




void	Bot::recv_file( std::vector<std::string> token )
{
	if ((this->_sock_c = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	memset(&this->_addr, 0, sizeof(this->_addr));

	this->_addr_c.sin_family = AF_INET;
	this->_addr_c.sin_port = htons( atoi(token[7].c_str()) );
	
	if (inet_pton(AF_INET, this->getIp(atol(token[6].c_str())), &this->_addr_c.sin_addr) < 0)
	{
		perror("INET_PTON");
		exit(EXIT_FAILURE);
	}
	if (connect(this->_sock_c, (struct sockaddr *) &this->_addr_c, sizeof(this->_addr_c)) < 0)
	{
		perror("Connect");
		exit(EXIT_FAILURE);
	}

	std::ofstream output;
	std::string line;
	int byte;
	char buffer[513];
	std::string tmp;
	int count_byte = 0;

	output.open(token[5] , std::ios::trunc);
	while ( (byte = read(this->_sock_c, buffer, 512)) > 0) 
	{
		count_byte += byte;
		buffer[byte] = 0;
		tmp.append(buffer);
	}

		output << tmp;
		std::cout << " byte " << byte << std::endl;

	output.close();
	
	close (this->_sock_c);
	
}


off_t getSizeFile(std::string nameFile)
{
	struct stat buffer;
	std::stringstream str;
	int fd;

	fd = open (nameFile.c_str(), O_RDONLY);
	if (!fd)
	{
		perror("File ");
	}

	if (fstat(fd, &buffer))
	{
		perror("Fstat");
	}
	close (fd);
	
	str << buffer.st_size;
	std::cout << nameFile << " " << str <<" **** \n"  ;
	
	return buffer.st_size;
}

template <typename T>
std::string toString( T data )
{
	std::stringstream conver ;

	conver << data;
	return conver.str();
}


void Bot::setNumReadSockC( void )
{

	FD_ZERO( &this->_reads_c );
	FD_ZERO( &this->_writes_c );
	FD_SET(this->_sock_c, &this->_reads_c);

	this->_time_out.tv_sec = 20;
	this->_time_out.tv_usec = 0;
	this->_num_read_sock_c = select( (this->_sock_c + 1 ), &this->_reads_c, &this->_writes_c, (fd_set *) 0 , &this->_time_out);
}

void	Bot::sendFile( std::string nick )
{
	int yes = 1;
	int port = 4242;
	memset((char *) &this->_addr, 0, sizeof(this->_addr_c));

	this->_addr_c.sin_addr.s_addr = INADDR_ANY;
	this->_addr_c.sin_family = AF_INET;
	this->_addr_c.sin_port = htons(port);

	if ((this->_sock_c = socket(AF_INET, SOCK_STREAM, 0))  < 0 )
	{
		perror("SOCKET " );
	}
	setsockopt(this->_sock_c, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	while (bind(this->_sock_c, (sockaddr*)&this->_addr_c, sizeof(this->_addr_c) ) < 0)
	{
		port++;
		if (port > 65535 || port < 0)
			return ;
		this->_addr_c.sin_port = htons(port);
		
	}
	unsigned long Ip = getIpLong();

	std::string msgFile;

	msgFile = "PRIVMSG " + nick + " :" + "\x01" + "DCC SEND " + "42.bmp " +  toString(Ip) + " " + toString(4242) + " " + toString(7680138) + "\r\n";

	send(this->_sock, msgFile.c_str(), msgFile.length(), 0);

	if (listen(this->_sock_c, FD_SETSIZE) < 0 )
	{
		perror ("LISTEN ");
		
	}
	struct   sockaddr_in client;
	memset((char *) &client, 0, sizeof(client));
	
	socklen_t len_client = sizeof(client);

	//this->setNumReadSock();

	if ( _num_read_sock_c > 0 )
	{
		int fd_client = accept(this->_sock_c, (struct sockaddr * ) &client, &len_client);
	
		int fd_file;
		int byte = 0;
		char buffer[513];
		char rec[4];
	
		fd_file = open("./42.bmp", O_RDONLY);
	
		while ((byte = read(fd_file, buffer, 512)) > 0 )
		{
			buffer[byte] = 0;
			send(fd_client, buffer, 512, 0);
			if ( recv(fd_client, rec, 4, 0) != 4)
				break ;
		}
		
		close(fd_client);
		close(fd_file);
			
		close(this->_sock_c);
	}
	
}


Bot::~Bot()
{
	typedef std::map<std::string, User *>::iterator iterator_usr;
	iterator_usr start = this->_users.begin();
	iterator_usr end = this->_users.end();

	for (; start != end; ++start )
	{
		delete start->second;
	}
	this->_users.clear();
	
}

void Bot::setNumReadSock( void )
{
	this->_time_out.tv_sec = 30;
	this->_time_out.tv_usec = 0;
	this->_num_read_sock = select( (this->_highsock + 1 ), &this->_reads, &this->_writes, (fd_set *) 0 , &this->_time_out);
}

void Bot::build_select_list( void )
{
	FD_ZERO( &this->_reads );
	FD_ZERO( &this->_writes );
	FD_SET(this->_sock, &this->_reads);
}


void Bot::read_serve()
{
	char		buffer[513];
	std::string	tmp;
	std::string tmp2;
	int 		byte;
	size_t		pos;
	


	byte = recv(this->getSocket(), buffer, 512, 0);
	
	buffer[byte] = '\0';
	tmp = buffer;
	
	std::cout << tmp;

	if (byte == 2)
		this->~Bot();
	while ( tmp.length() )
	{
		if (( pos = tmp.find('\n') ) != std::string::npos )
		{
			tmp2 = tmp.substr(0, pos + 1);
			tmp.erase(0, pos + 1);
			if (tmp2.length() > 510 )
				tmp2 = tmp2.substr(0, 510);
			if ( tmp2[0] != '\r' && tmp2[0] != '\n')
				this->parse( tmp2 );
		}
		else
			tmp.clear();
	}
}

void Bot::parse( std::string const & buffer )
{	
	std::vector<std::string> token = split( buffer, ' ');
	std::vector<std::string> token_game;

	if (!token.size())
		return ;
	if( token[0] == "PING" )
	{
		std::string message = "PONG ";
		message.append(token[1]);
		std::cout << message;
		send(this->_sock, message.c_str(), message.length(), 0 );
		return ;
	}

	token_game = split(token[0], '!');
	
	std::string nick = static_cast<std::string>(&token_game[0][1]);
	
	if ( token[1] == "PRIVMSG" && !this->_users.count(nick) )
	{
		this->_users[nick] = new User(nick);
		srand( time( NULL ) );

		int start = rand() % 2;
		if (  start )
			this->_users[nick]->setTableBot();
		this->draw( this->_users[nick] );
	}
	else
	{
		std::string message = token[ token.size() - 1 ];
		if (token[1] == "PRIVMSG" )
			this->ticTacToe(message, nick);
	}
}

void Bot::draw(User *usr)
{
	typedef std::map<std::string, std::string>::iterator it_map;
	std::string messages;

	std::map<std::string, std::string> map = usr->getTabla();
	int i = 0;
	it_map start = map.begin();
	it_map end = map.end();
	this->send_message("\tYou are: ❌", usr);
	messages.append("\t");
	for (int j = 1; start != end ; ++start, j++)
	{
		std::cout << start->second;
		if ( !(j % 3) )
			std::cout << std::endl;
		messages.append(start->second);
		i++;
		if (i == 3)
		{
			this->send_message(messages, usr);
			messages.clear();
			messages.append("\t");
			i = 0;
		}
	}
}

void Bot::ticTacToe(std::string const & message, std::string const & nick)
{
	User *usr = this->_users[nick];
	
	if ( usr )
	{
		if ( !usr->setTabla(message) )
			this->send_message("Invalid coordinate (x,y) (MIN x,y: 1)  (MAX x,y: 3) or coordinate is already used.", usr);
		else
			usr->setTableBot();
		this->draw(usr);
		if ( usr->getWin() )
		{
			this->send_message("You Win 🏆 ", usr);
			this->sendFile(nick);
		}
		if ( usr->getLose() )
			this->send_message("You Lose 😭 ", usr);
		if (usr->getTie())
			this->send_message("We are in a Draw 😱", usr);
	}
}

void Bot::attendServer()
{
	if (FD_ISSET(this->_sock, &this->_reads) )
			this->read_serve();
	if (FD_ISSET(this->_sock, &this->_writes) )
	{
		typedef std::deque<User *>::iterator it_user;
	
		it_user start = this->_send_message.begin();
		it_user end = this->_send_message.end();
	
		for (; start != end; ++start )
			this->sendRequest( *start );
	}

}

void	Bot::send_message(std::string _message, User * usr)
{
	std::string message;

	FD_SET(this->_sock, &this->_writes);

	message.append("PRIVMSG ");
	message.append(usr->getNick());
	message.append(" :");
	message.append( _message );
	message.append("\r\n");
	usr->setAnswer( message );
	if ( !this->isAnswerUser( usr ) )
		this->_send_message.push_back( usr );
}

bool	Bot::isAnswerUser( User *usr )
{
	typedef std::deque<User *>::iterator it_deque;

	it_deque start = this->_send_message.begin();
	it_deque end = this->_send_message.end();

	for (; start != end ; ++start)
	{
		if ( (*start)->getNick()  == usr->getNick() )
			return true;
	}

	return false;
}

void	Bot::sendRequest(User *usr)
{
	int			diff;
	size_t		len;
	std::string	messages;

	diff = 0;
	len = 0;
	while ( ( messages = usr->getAnswer() ).size() != 0)
	{
		if ( messages.length() > 512 )
		{
			messages = messages.substr(0, 512);
			messages[510] = '\r';
			messages[511] = '\n';
		}
		len = send(this->_sock, messages.c_str(), messages.length(), 0);
		diff = messages.length() - len;
		if ( diff > 0 )
		{
			usr->setAnswer(messages.substr(diff, messages.length()) );
			break ;
		}
	}
	if ( diff == 0 )
		this->deleteDequeUser( usr );
}


void Bot::deleteDequeUser ( User * usr )
{
	typedef std::deque<User *>::iterator it_user;

	it_user	start = this->_send_message.begin();
	it_user	end = this->_send_message.end();

	for (; start != end; ++start )
	{
		if ((*start)->getNick() == usr->getNick() )
		{
			this->_send_message.erase( start );
			break ;
		}
	}
	if ( usr->getWin() || usr->getLose() || usr->getTie() )
	{
		this->_users.erase(usr->getNick());
		delete usr;
	}
}

int const & Bot::getSocket() const 
{
	return this->_sock;
}
int const &  Bot::getNumReadSockC( void ) const
{
	return this->_num_read_sock_c;
}
int const &  Bot::getNumReadSock( void ) const
{
	return this->_num_read_sock;
}

fd_set	const & Bot::getWrites() const 
{
	return this->_writes;
}

fd_set	const & Bot::getReads() const 
{
	return this->_reads;
}

struct sockaddr_in	const &Bot::getAddress()
{
	return this->_addr;
}
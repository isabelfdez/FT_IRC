/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:33:37 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/13 19:05:56 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

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
	
	if (inet_pton(AF_INET, IP.c_str(), &this->_addr.sin_addr) <= 0)
	{
		perror("INET_PTON");
		exit(EXIT_FAILURE);
	}
	if (connect(this->_sock, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) < 0 )
	{
		perror("Connect");
		exit(EXIT_FAILURE);
	}
}
Bot::~Bot()
{
	
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
	FD_SET(this->_sock, &this->_writes);
}


void Bot::read_serve()
{
	char		buffer[513];
	std::string	tmp;
	std::string tmp2;
	User		*usr;
	int 		byte;
	size_t		pos;
	


	byte = recv(this->getSocket(), buffer, 512, 0);
	
		buffer[byte] = '\0';
		tmp += buffer;
	

	// tmp = usr->getBufferCmd();
	// if ( tmp.length() == 0 )
		// this->deleteUser( usr , "[Signed off]");
	std::cout << tmp;
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
				this->parse( tmp2 );
			}
			// usr->setBufferCmd("");
		}
		else
		{
			// std::string buffer_cmd = usr->getBufferCmd() + tmp;
			// usr->setBufferCmd( buffer_cmd );
			tmp.clear();
		}
	}
}

void Bot::parse( std::string const & buffer )
{
	typedef std::map<std::string, User *>::iterator it_user;
	
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
	
	// it_user start = this->_users.begin();
	// it_user end = this->_users.end();
	std::string nick = static_cast<std::string>(&token_game[0][1]);
	if ( !this->_users.count(nick) )
		this->_users[nick] = new User(nick);
	else
	{
		std::string message = token[token.size() - 1 ];
		if (token[1] == "PRIVMSG" )
			this->ticTacToe(message, nick);
	}
	
}

void Bot::ticTacToe(std::string const & message, std::string const & nick)
{
	User *usr = this->_users[nick];
	std::string messages;
	typedef std::map<std::string, std::string>::iterator it_map;
	
	if ( usr )
	{
		int i = 0;
		if ( !usr->setTabla(message) )
		{
			this->send_message("Invalid coordinate (x,y) (MIN x,y: 1)  (MAX x,y: 3) or coordinate is already used.", usr);
			// return ;
		}
		else
			usr->setTableBot();
		std::map<std::string, std::string> map = usr->getTabla();
		it_map start = map.begin();
		it_map end = map.end();
		
		messages.append("\t");
		for (; start != end ; ++start)
		{
			std::cout << start->second;
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
		if ( usr->getWin() )
			this->send_message("You Win 🏆 ", usr);
		if ( usr->getLose() )
			this->send_message("You Lose 😭 ", usr);
	}
}

void Bot::attendServer()
{
	if (FD_ISSET(this->_sock, &this->_reads) )
	{	
		// std::cout << "attendServer\n";
			this->read_serve();
		// this->parse();
	}
	// if (FD_ISSET(this->_sock, &this->_writes) )
	// {
		typedef std::deque<User *>::iterator it_user;
	
		it_user start = this->_send_message.begin();
		it_user end = this->_send_message.end();
	
		for (; start != end; ++start )
		{
				this->sendRequest( *start );
			
			std::cout << "\nHERE \n";
		}
	// }

}

void	Bot::send_message(std::string _message, User * usr)
{
    std::string message;


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
	if ( usr->getWin() || usr->getLose() )
	{
		this->_users.erase(usr->getNick());
		delete usr;
	}
}

int const & Bot::getSocket() const 
{
	return this->_sock;
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



#include "Server.hpp"

void    Server::send_message(std::string _message, User * dest, User * usr)
{
    std::string message;


    message.append(usr->getMask());
    message.append(" ");
    message.append( _message );
    message.append("\r\n");

	dest->setAnswer( message );

	if ( !this->isAnswerUser( dest ) )
		this->_send_message.push_back( dest );
}

void    Server::send_message_channel(std::string message, User * usr, Channel * chnl)
{
    std::string mask;
	typedef std::list<User*>::iterator it_user;


	it_user start = chnl->getUsers().begin();
	it_user end = chnl->getUsers().end();
    mask.append(usr->getMask());
    mask.append(" ");
    mask.append(message);
    mask.append("\r\n");


	for ( ; start != end ; ++start)
	{
		// if ( (*start)->getsockfd() != usr->getsockfd()  )
		// {
			(*start)->setAnswer( mask );
			if ( !this->isAnswerUser( (*start) ) )
				this->_send_message.push_back( (*start) );
		// }
	}
}

void    Server::send_message_channel_block(std::string message, User * usr, Channel * chnl)
{
    std::string mask;
	typedef std::list<User*>::iterator it_user;

	it_user start = chnl->getUsers().begin();
	it_user end = chnl->getUsers().end();

    mask.append(usr->getMask());
    mask.append(" ");
    mask.append(message);
    mask.append("\r\n");


	for ( ; start != end ; ++start)
	{
		if ( (*start)->getsockfd() != usr->getsockfd() && !((*start))->getIsSendMsg() )
		{
			(*start)->setAnswer( mask );
			if ( !this->isAnswerUser( (*start) )  )
				this->_send_message.push_back( (*start) );
			(*start)->setIsSendMsg( true );
		}
	}
}

void	Server::send_reply(std::string reply, std::string str, User * usr)
{
	std::string message;

	message.assign(":ft_irc.com ");
	message.append(reply);
	message.append(" " + usr->getNick());
	message.append(str);
	message.append("\r\n");

	usr->setAnswer( message);

	if ( !this->isAnswerUser( usr ) )
		this->_send_message.push_back( usr );
	// send(usr->getsockfd(), message.c_str(), message.length(), 0);

}

void	Server::send_error(std::string reply, std::string str, User *usr)
{
	std::string message;

	// User		*dest  = this->_fd_users[ fd ];
	message.assign(":ft_irc.com ");
	message.append(reply);
	message.append(" " + usr->getNick());
	message.append(" " + str);
	message.append("\r\n");

	usr->setAnswer( message);

	if ( !this->isAnswerUser( usr ) )
		this->_send_message.push_back( usr );
	//  send(fd, message.c_str(), message.length(), 0);
}


bool			Server::isAnswerUser( User *usr )
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


void	Server::sendRequest(User *usr)
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
		len = send(usr->getsockfd(), messages.c_str(), messages.length(), 0);
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

void Server::deleteDequeUser ( User * usr )
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
}

#include "server.hpp"

// Constructor && destructor

Server::Server(int port)
{
	FD_ZERO(&_socks);
	this->_sock = 0;
	this->_highSock = 0;
	for (int i = 0; i < MAXFD; i++)
		this->_connectList[i] = 0;
	memset((char *) &_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = port;
}

Server::~Server()
{
	std::list<User*>::iterator it = _users.begin();
	while (it != _users.end())
	{
		close((*it)->getsockfd());
		it++;
		_users.pop_front();
	}
}

// Getters

fd_set &			Server::getSocks() { return (_socks); }

int &				Server::getSock() { return(_sock); }

int					Server::getHighSock() const { return(_highSock); }

struct sockaddr_in &Server::getStruct() { return(_server_address); }

int &				Server::getList(int index) { return(_connectList[index]); }

// Setters

void				Server::setSock(int a) { _sock = a; }

void				Server::setHighSock(int a) { _highSock = a; }

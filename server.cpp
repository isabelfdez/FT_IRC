#include "server.hpp"

Server::Server(int port)
{

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


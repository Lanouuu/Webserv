#include "server.h"
#include "Client.hpp"

void launchServers(serv_vector & servers, int & epoll_fd)
{
    for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
        (*it).launchServer(epoll_fd);
    return ;
}
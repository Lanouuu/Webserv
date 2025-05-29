#include "server.h"
#include "Client.hpp"

void launchServers(serv_vector & servers)
{
    for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
        (*it).launchServer();
    return ;
}
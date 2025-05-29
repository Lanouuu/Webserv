#ifndef SERVER_H
# define SERVER_H

# include <sys/socket.h>
# include <netdb.h>
# include <sys/epoll.h>
# include "parsing.hpp"
# include "Client.hpp"

void launchServers(serv_vector & servers);

#endif
#ifndef SERVER_H
# define SERVER_H

# define MAX_EVENTS 1024

# include <sys/socket.h>
# include <netdb.h>
# include "Client.hpp"
# include "Server.hpp"
# include "Parser.hpp"

void    parsingConfFile(std::string file, serv_vector & servers);
void    launchServers(serv_vector & servers, int & epoll_fd);

#endif
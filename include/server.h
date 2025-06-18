#ifndef SERVER_H
# define SERVER_H

# define MAX_EVENTS 1024

# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include "Client.hpp"
# include "Parser.hpp"
# include "Socket.hpp"

void    parsingConfFile(std::string file, socket_map & sockets);
void    launchServers(serv_vector & servers, int & epoll_fd);

#endif
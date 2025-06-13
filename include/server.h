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

void    parsingConfFile(std::string file, socket_map & sockets, int & epoll_fd);
void    createSockets(socket_map & sockets, serv_vector & servers, int & epoll_fd);
void    printSockets(socket_map & sockets);

#endif
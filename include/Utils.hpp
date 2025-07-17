#ifndef UTILS_HPP
# define UTILS_HPP

#include "Client.hpp"
#include "Parser.hpp"


void    addClient(client_map & clients, Client const & client);
bool    isServerSocket(int const & serverSocket, serv_vector const & servers);
void    setClient(Client & client, int const & socket_fd, int const & epoll_fd);
int     read_request(client_map & clients, int const & socket_fd, int const & epoll_fd);
#endif
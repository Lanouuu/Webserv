#ifndef UTILS_HPP
# define UTILS_HPP

#include "Client.hpp"
#include "Parser.hpp"


void    addClient(client_map & clients, Client const & client, int const & clientSocket);
bool    isServerSocket(int const & serverSocket, serv_vector const & servers);

#endif
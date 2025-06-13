#include "server.h"

void    parsingConfFile(std::string file, socket_map & sockets, int & epoll_fd)
{
    Parser      parser(file);
    serv_vector servers;

  //  parser.printTokens();
    parser.parseConf(servers);
    createSockets(sockets, servers, epoll_fd);
    printSockets(sockets);
    return ;
}
#include "server.h"

static void fillServer(serv_vector & servers)
{
    for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
    {
        (*it).fillStruct();
        (*it).fillSocket();
    }
    return ;
}

void    parsingConfFile(std::string file, serv_vector & servers)
{
    Parser   parser(file);

  //  parser.printTokens();
    parser.parseConf(servers);
    fillServer(servers);
    printServers(servers);
    return ;
}
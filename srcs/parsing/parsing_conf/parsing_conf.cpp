#include "server.h"

static void    printLocations(location_map locations)
{
    int i = 1;
    for (location_map::iterator it = locations.begin(); it != locations.end(); it++)
    {
        std::cout
            << BLUE << "Location [" << i << "] " << (*it).first
            << std::endl << std::endl
            << "\t" << "Base Uri = " << (*it).second.getBaseUri()
            << std::endl
            << "\t" << "URL = " << (*it).second.getUrl()
            << std::endl;
            (*it).second.printIndexes();
        std::cout
            << END
            << std::endl;
        i++;
    }
}

static void    printServers(serv_vector & servers)
{
    int i = 1;

    for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
    {
        std::cout 
            << GREEN << "Server [" << i << "] "
            << std::endl << std::endl
            << "\t"<< std::flush;
            (*it).printServNames();
        std::cout 
            << "\t" << "Root = " << (*it).getRoot()
            << std::endl
            << "\t" << "Port = " << (*it).getPort()
            << std::endl
            << "\t" << "IP = " << (*it).getIP()
            << END
            << std::endl
            << std::endl;
        printLocations((*it).getLocaMap());
        i++;
    }
}

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
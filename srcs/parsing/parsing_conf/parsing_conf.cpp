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
            << std::endl
            << "\t" << "Autoindex = "
            << std::flush;
            if ((*it).second.getAutoIndex())
                std::cout << "[ON]" << std::endl;
            else
                std::cout << "[OFF]" << std::endl;
            (*it).second.printIndexes();
            (*it).second.printMethod();
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
            << "---------------------------------------------------------"
            << std::endl
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
            << std::endl;
            (*it).printErrorPage();
            std::cout
            << END
            << std::endl
            << std::endl;
        printLocations((*it).getLocaMap());
        i++;
    }
}

// static void fillServer(serv_vector & servers)
// {
//     for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
//     {
//         (*it).fillStruct();
//         (*it).fillSocket();
//     }
//     return ;
// }

static void createSockets(socket_map & sockets, serv_vector & servers)
{
    for (; !servers.empty();)
    {
        
    }
    return ;
}

void    parsingConfFile(std::string file, socket_map & sockets)
{
    Parser      parser(file);
    serv_vector servers;

  //  parser.printTokens();
    parser.parseConf(servers);
    createSockets(sockets, servers);
    //fillServer(servers);
    printServers(servers);
    return ;
}
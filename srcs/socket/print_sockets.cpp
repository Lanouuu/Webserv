#include "server.h"

static void    printLocations(location_map locations)
{
    int i = 1;
    for (location_map::iterator it = locations.begin(); it != locations.end(); it++)
    {
        std::cout
            << BLUE << "--- Location [" << i << "] " << (*it).first
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

static void    printServers(serv_vector servers)
{
    int i = 1;

    for (serv_vector::iterator it = servers.begin(); it != servers.end(); it++)
    {
        std::cout 
            << std::endl
            << GREEN << "---------- Server [" << i << "] "
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
            std::cout << "\t" << std::flush;
            (*it).printIndexes();
            std::cout
            << END
            << std::endl
            << std::endl;
        printLocations((*it).getLocaMap());
        i++;
    }
}

void    printSockets(socket_map & sockets)
{
    int i = 1;

    for (socket_map::iterator it = sockets.begin(); it != sockets.end(); it++)
    {
        std::cout
            << std::endl
            << YELLOW
            << "---------------------------------"
            << " SOCKET [" << i << "]"
            << std::endl << std::endl
            << "\t" << "Socket FD = " << (*it).second.getFD()
            << std::endl
            << "\t" << "IP = " << (*it).second.getIP()
            << std::endl
            << "\t" << "Port = " << (*it).second.getPort()
            << END
            << std::endl
            << std::endl;
            printServers((*it).second.getServers());
        i++;
    }
    return ;
}
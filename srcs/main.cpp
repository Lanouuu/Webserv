
#include "server.h"


int main(int ac, char **av, char **env)
{
    (void)env;

    try
    {
        serv_vector servers;
        client_map  clients;    

        if (ac > 2)
            throw std::invalid_argument("Error: too many arguments");
        if (ac == 2)
            parsingConfFile(av[1], servers);
        launchServers(servers);
        while (1)
        {
            // gerer accept, epoll ?
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl << std::endl;
        return (1);
    }
    
    return (0);
}
#include "server.h"

int main(int ac, char **av, char **env)
{
    try
    {
        if (ac > 2)
            throw std::invalid_argument( RED "Error: " END "too many arguments");
        if (ac < 2)
            throw std::invalid_argument( RED "Error: " END "expected \'.conf\' file");
        
        serv_vector servers;
        client_map  clients;
        int         epoll_fd;
        int         n_event;

        if (epoll_fd = epoll_create1(0) == -1);
            throw std::runtime_error( RED "Error: epoll_create: " END + std::string(strerror(errno)));
        parsingConfFile(av[1], servers);
        launchServers(servers, epoll_fd);
        struct epoll_event events[MAX_EVENTS]; // tableau d'event
        while (1)
        {
            if(n_event = epoll_wait(epoll_fd, events, MAX_EVENTS, -1) == -1)
                throw std::runtime_error( RED "Error: epoll_wait: " END + std::string(strerror(errno)));
            /*
                boucle for (parcourir le tableau epoll_event events) 
                {
                    int fd = events[i].data.fd;
                    si (events[i].events & EPOLLIN) {
                        Client temp;
                        temp._clientFd = accept(fd, struct client_addr, client_addr_len);
                        clients.insert(temp); inserer le nouveau client dans la map de client
                    }
                }
            */
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl << std::endl;
        return (1);
    }
    
    return (0);
}
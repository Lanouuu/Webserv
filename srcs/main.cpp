#include "server.h"
#include "Utils.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
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
        // size_t      index = 0;
        if ((epoll_fd = epoll_create1(0)) == -1)
            throw std::runtime_error( RED "Error: epoll_create: " END + std::string(strerror(errno)));
        parsingConfFile(av[1], servers);
        launchServers(servers, epoll_fd);
        struct epoll_event events[MAX_EVENTS]; // tableau d'event
        while (1)
        {
            if((n_event = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) == -1)
                throw std::runtime_error( RED "Error: epoll_wait: " END + std::string(strerror(errno)));
            
            for (int i = 0; i < n_event; i++) 
            {
                int socket_fd = events[i].data.fd; // -> correspond a la socket du server;
                if (events[i].events & EPOLLIN) {
                    Client temp;
                    if (isServerSocket(socket_fd, servers))
                    {
                        std::cout << "Server socket =  " << socket_fd << std::endl;
                        setClient(temp, socket_fd, epoll_fd);
                        addClient(clients, temp);
                    }
                    else
                    {
                        if (read_request(clients, socket_fd, epoll_fd) == 0)
                            continue;
                        // for(;index < servers.size(); index++)
                        // {
                        //     if (servers[index].getSocket() == socket_fd)
                        //         break ;
                        // }
                        if (clients[socket_fd].RequestIsComplete())
                        {
                            clients[socket_fd].getRequest().parse_request(clients[socket_fd], servers[0]);
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, &temp.getClientEpollStruct());
                            clients.erase(socket_fd);
                        }
                    }
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl << std::endl;
        return (1);
    }
    
    return (0);
}
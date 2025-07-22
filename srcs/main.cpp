#include "server.h"
#include "Utils.hpp"
#include "signal.h"

int main(int ac, char **av)
{
    int         epoll_fd = -1;
    client_map  clients;
    try
    {
        if (ac > 2)
            throw std::invalid_argument( RED "Error: " END "too many arguments");
        if (ac < 2)
            throw std::invalid_argument( RED "Error: " END "expected \'.conf\' file");
        
        serv_vector servers;
        
        int         n_event;
        // size_t      index = 0;
        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        
        parsingConfFile(av[1], servers);
        if ((epoll_fd = epoll_create1(0)) == -1)
            throw std::runtime_error( RED "Error: epoll_create: " END + std::string(strerror(errno)));
        launchServers(servers, epoll_fd);
        struct epoll_event events[MAX_EVENTS]; // tableau d'event
        if(sigaction(SIGINT, &sa, NULL) == -1)
            throw std::runtime_error( RED "Error: sigaction: " END + std::string(strerror(errno)));
        while (!stop)
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
                            close(socket_fd);
                            clients.erase(socket_fd);
                        }
                    }
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        if (epoll_fd != -1)
            close(epoll_fd);
        for (client_map::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if ((*it).first != -1)
                close((*it).first);
        }
        if (stop == 1)
            return (0);
        std::cerr << e.what() << std::endl << std::endl;
        return (1);
    }
    return (0);
}
#include "server.h"

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
                int fd = events[i].data.fd; // -> correspond a la socket du server;
                if (events[i].events & EPOLLIN) {
                    Client temp;
                    struct sockaddr_in clientInfo;
                    socklen_t size = sizeof(clientInfo);
                    temp.setFd(accept(fd, (struct sockaddr *)&clientInfo, &size)); // -> utiliser le setter .setFd();
                    // clients.insert(temp); //inserer le nouveau  client dans la map de client
                    char buf[255] = {0};
                    memset(&buf, 0, 255);
                    recv(temp.getClientFd(), buf, 255, 0);
                    temp.getRequest().add_request(buf, sizeof(buf));
                    // Parsing requete
                    if (temp.getRequest().parse_request() != 0)
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
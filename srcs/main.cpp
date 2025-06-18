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
        size_t      index = 0;
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
                    struct sockaddr_in clientInfo;
                    socklen_t size = sizeof(clientInfo);
                    temp.setFd(accept(socket_fd, (struct sockaddr *)&clientInfo, &size)); // -> utiliser le setter .setFd();
                    for(int i = 0; i < std::numeric_limits<int>::max(); i++)
                    {
                        std::stringstream ss;
                        std::string key;
                        ss << i;
                        ss >> key;
                        client_map::const_iterator it = clients.find(key);
                        if (it != clients.end())
                            continue;
                        clients.insert(std::pair<std::string, Client>(key, temp));
                        clients[key].setUid(key);
                        break ;
                    }
                    if (!clients.empty())
                    {
                        for(client_map::iterator it = clients.begin(); it != clients.end(); it++)
                            std::cout << "client uid = " << it->second.getUid() << std::endl;
                    }
                    char buf[255] = {0};
                    int readed = 255;
                    while (readed == 255)
                    {
                        memset(&buf, 0, 255);
                        readed = recv(temp.getClientFd(), buf, 255, 0);
                        std::cout << "readed = " << readed << std::endl;
                        temp.getRequest().add_request(buf, sizeof(buf));
                        std::cout << buf << std::endl;
                    }
                    for(;index < servers.size(); index++)
                    {
                        if (servers[index].getSocket() == socket_fd)
                            break ;
                    }
                    temp.getRequest().parse_request(temp, servers[index]);
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
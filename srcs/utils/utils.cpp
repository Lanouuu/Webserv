#include "Utils.hpp"

void    addClient(client_map & clients, Client const & client)
{
    // for(int i = 0; i < std::numeric_limits<int>::max(); i++)
    // {
    //     std::stringstream ss;
    //     std::string key;
    //     ss << i;
    //     ss >> key;
        client_map::const_iterator it = clients.find(client.getClientFd());
        if (it != clients.end())
            return ;
        clients.insert(std::pair<int, Client>(client.getClientFd(), client));
    //     clients[clientSocket].setUid(key);
    //     break ;
    // }
}

void    setClient(Client & client, int const & socket_fd, int const & epoll_fd)
{
        struct sockaddr_in clientInfo;
        socklen_t size = sizeof(clientInfo);
        int clientSocket = accept(socket_fd, (struct sockaddr *)&clientInfo, &size);
        client.setFd(clientSocket);
        client.getClientEpollStruct().events = EPOLLIN;
        client.getClientEpollStruct().data.fd = clientSocket;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &client.getClientEpollStruct());
}

bool    isServerSocket(int const & serverSocket, serv_vector const & servers)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (serverSocket == servers[i].getSocket())
            return true;
    }
    return false;
}

int    read_request(client_map & clients, int const & socket_fd, int const & epoll_fd)
{
    char buf[4098] = {0};
    int readed = 0;
    memset(&buf, 0, 4098);
    readed = recv(socket_fd, buf, 4098, 0);
    if (readed <= 0)
    {
        std::cout << "ICI" << std::endl;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
        close(socket_fd);
        clients.erase(socket_fd);
        return readed;
    }
    // std::cout << "readed = " << readed << std::endl;
    clients[socket_fd].getRequest().add_request(buf, readed);
    // std::cout << buf << std::flush;
    return readed;
}

volatile sig_atomic_t stop = 0;

void signal_handler(int s)
{
    std::cout << RED <<  "Caught signal " << s << END << std::endl;
    stop = 1;
}
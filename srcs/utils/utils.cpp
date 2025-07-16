#include "Utils.hpp"
#include "Error_code.hpp"
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
    clients[socket_fd].getRequest().add_request(buf, sizeof(buf));
    std::cout << buf << std::flush;
    return readed;
}


//create response with the html file related (file successfully created for ex)
std::string create_response_html(int succes_code, std::string mode)
{
    std::string (*funct[])(void) = {&error_code_200, &error_code_201, &error_code_400, &error_code_403, &error_code_404, &error_code_415, &error_code_500};
    
    switch (succes_code)
    {
        case 200:
            if (mode == "delete")
                return (*funct[0])();
        case 201:
            if (mode == "crok")
                return (*funct[1])();
        case 400:
            if (mode == "badreq")
                return (*funct[2])();
        case 403:
            if (mode == "forbidden")
                return (*funct[3])();
        case 404:
            if (mode == "nofound")
                return (*funct[4])();
        case 415:
            if (mode == "umt")
                return (*funct[5])();
        case 500:
            if (mode == "ise")
                return (*funct[6])();
        default:
            return 0;
    }
}
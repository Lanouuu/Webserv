#include "server.h"

static std::string socketErr(std::string serv_ip, uint16_t serv_Port)
{
    std::ostringstream buf;
    buf << RED << "Error: " << END
        << serv_ip << ":"<< serv_Port
        << " : addr already use";
    return (buf.str());
}

static int  checkServNames(Socket & socket, Server & serv_temp)
{
    serv_vector temp = socket.getServers();
    for (serv_vector::iterator it = temp.begin(); it != temp.end(); it++)
    {
        if ((*it).getNames() == serv_temp.getNames())
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static void initSocket(socket_map & sockets, Server & serv_temp, int & epoll_fd)
{
    Socket  socket_temp;
    socket_temp.fillSocket(serv_temp, epoll_fd);
    sockets.insert(std::make_pair(socket_temp.getFD(), socket_temp));
    return ;
}

void    createSockets(socket_map & sockets, serv_vector & servers, int & epoll_fd)
{
    for (; !servers.empty();)
    {
        Server  serv_temp(servers.front());
        if (sockets.empty())
            initSocket(sockets, serv_temp, epoll_fd);
        else
        {
            int flag = 0;
            for (socket_map::iterator it = sockets.begin(); it != sockets.end(); it++)
            {
                if (serv_temp.getPort() == (*it).second.getPort())
                {
                    if (serv_temp.getIP() != (*it).second.getIP())
                        throw std::invalid_argument(socketErr(serv_temp.getIP(), serv_temp.getPort()));
                    if (checkServNames((*it).second, serv_temp) == EXIT_FAILURE)
                        throw std::invalid_argument(RED "Error: " END " servers has same IP/Port and server names");
                    (*it).second.addServer(serv_temp);
                    flag = 1;
                    break ;
                }
            }
            if (flag == 0)
                initSocket(sockets, serv_temp, epoll_fd);
        }
        servers.erase(servers.begin());
    }
    return ;
}
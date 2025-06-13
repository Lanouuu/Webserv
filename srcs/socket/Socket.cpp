#include "Socket.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Socket::Socket(void) : _socketFD(-1)
{
    return ;
}

Socket::Socket(const Socket & src)
{
    *this = src;
    return ;
}

Socket::~Socket(void)
{
    return ;
}

/****************************************************************************/
/*                               Operators                                  */
/****************************************************************************/

Socket &    Socket::operator=(const Socket & rhs)
{
    if (this != &rhs)
    {
        this->_socketFD = rhs._socketFD;
        this->_socketIP = rhs._socketIP;
        this->_socketPort = rhs._socketPort;
        this->_socketServers = rhs._socketServers;
    }
    return (*this);
}

/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

int Socket::getFD(void) const
{
    return (this->_socketFD);
}

uint16_t    Socket::getPort(void) const
{
    return (this->_socketPort);
}

std::string Socket::getIP(void) const
{
    return (this->_socketIP);
}

serv_vector Socket::getServers(void) const
{
    return (this->_socketServers);
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Socket::fillStruct(void)
{
    _socketSa.sin_family = AF_INET;
    _socketSa.sin_port = htons(_socketPort);
    if (inet_pton(AF_INET, _socketIP.c_str(), &_socketSa.sin_addr) != 1)
        throw std::runtime_error(RED "Error: inet_pton: " END + std::string(strerror(errno)));
    return ;
}

void    Socket::fillSocket(Server & serv_temp, int epoll_fd)
{
    _socketPort = serv_temp.getPort();
    _socketIP = serv_temp.getIP();
    _socketServers.push_back(serv_temp);
    this->fillStruct();
    _socketFD = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (_socketFD == -1)
        throw std::runtime_error(RED "Error: socket: " END + std::string(strerror(errno)));
    if (bind(_socketFD, (struct sockaddr *)&_socketSa, sizeof(_socketSa)) == -1)
        throw std::runtime_error(RED "Error: bind: " END + std::string(strerror(errno)));
    if (listen(_socketFD, 4096) == -1)
        throw std::runtime_error(RED "Error: listen: " END + std::string(strerror(errno)));
    struct epoll_event  event;
    event.events = EPOLLIN;
    event.data.fd = _socketFD;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socketFD, &event) == -1)
        throw std::runtime_error("Error: epoll_ctl: " + std::string(strerror(errno)));
    return ;
}

void    Socket::addServer(Server & serv_temp)
{
    _socketServers.push_back(serv_temp);
    return ;
}
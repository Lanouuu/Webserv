#include "Server.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Server::Server(void) : _serverRoot("./"), _serverSocket(-1)
{
    return ;
}

Server::~Server(void)
{
    return ;
}

/****************************************************************************/
/*                               Operators                                  */
/****************************************************************************/


/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

void    Server::setHost(const std::string & host)
{
    this->_serverHost = host;
    return ;
}

void    Server::setIndex(const std::string & index)
{
    this->_serverIndex = index;
    return ;
}

void    Server::setPort(const uint16_t & port)
{
    this->_serverPort = port;
    return ;
}

void    Server::setIP(const std::string & ip)
{
    this->_serverIP = ip;
    return ;
}

std::string Server::getRoot(void) const
{
    return (this->_serverRoot);
}

uint16_t    Server::getPort(void) const
{
    return (this->_serverPort);
}
        
std::string Server::getIP(void) const
{
    return (this->_serverIP);
}

location_map    Server::getLocaMap(void) const
{
    return (this->_serverLocations);
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Server::printServNames(void) const
{
    std::cout
        << "Server_name = " << std::flush;
    for (std::vector<std::string>::const_iterator it = _serverName.begin(); it != _serverName.end(); it++)
        std::cout << (*it) << " " << std::flush;
    std::cout << std::endl;
    return ;
}

void    Server::addName(const std::string & name)
{
    this->_serverName.push_back(name);
    return ;
}

void    Server::addLocation(const std::string & name, const Location & location)
{
    this->_serverLocations.insert(std::make_pair(name, location));
    return ;
}

void    Server::fillStruct(void)
{
    _serverSa.sin_family = AF_INET;
    _serverSa.sin_port = htons(_serverPort);
    _serverSa.sin_addr.s_addr = INADDR_ANY;
    return ;
}

void    Server::fillSocket(void)
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error(RED "Error: socket: " END + std::string(strerror(errno)));
    return ;
}

void    Server::launchServer(int & epoll_fd)
{
    if (bind(_serverSocket, (struct sockaddr *)&_serverSa, sizeof(_serverSa)) == -1)
        throw std::runtime_error(RED "Error: bind: " END + std::string(strerror(errno)));
    if (listen(_serverSocket, 4096) == -1)
        throw std::runtime_error(RED "Error: listen: " END + std::string(strerror(errno)));
    struct epoll_event  event = {.events = EPOLLIN, .data.fd = _serverSocket};
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _serverSocket, &event) == -1)
        throw std::runtime_error("Error: epoll_ctl: " + std::string(strerror(errno)));
    return ;
}
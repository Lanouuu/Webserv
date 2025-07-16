#include "Server.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Server::Server(void) : _serverRoot("./"), _serverSocket(-1), _serverBodySize(1048576)
{
    _serverIndexes.push_back("index.html");
    return ;
}

Server::~Server(void)
{
    if (_serverSocket != -1)
        close(_serverSocket);
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

void    Server::setBodySize(const size_t & size)
{
    this->_serverBodySize = size;
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

std::vector<std::string> Server::getNames(void) const
{
    return (this->_serverName);
}

std::vector<std::string>    Server::getIndexes(void) const
{
    return (this->_serverIndexes);
}

int const & Server::getSocket(void) const
{
    return (this->_serverSocket);
}  

size_t &    Server::getBodySize(void)
{
    return (this->_serverBodySize);
}

cgi_map Server::getCgi(void) const
{
    return (this->_serverCgi);
}

const errpage_map Server::getErrPages(void) const
{
    return (this->_serverErrorPages);
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

void    Server::printErrorPage(void) const
{
    int i = 1;
    if (!_serverErrorPages.empty())
    {
        for (errpage_map::const_iterator it = _serverErrorPages.begin(); it != _serverErrorPages.end(); it++)
        {
            std::cout
                << "\tError_pages [" << i << "] " << (*it).first << std::endl;
            for (std::vector<int>::const_iterator vit = (*it).second.begin(); vit != (*it).second.end(); vit++)
                std::cout << "\t" << (*vit) << std::endl; 
            i++;
        }
    }
    return ;
}

void    Server::printIndexes(void) const
{
    std::cout
        << "Indexes = " << std::flush;
    for (std::vector<std::string>::const_iterator it = _serverIndexes.begin(); it != _serverIndexes.end(); it++)
        std::cout << (*it) << " " << std::flush;
    std::cout << std::endl;
    return ;
}

void    Server::printCgi(void) const
{
    int i = 1;
    if (!_serverCgi.empty())
    {
        for (cgi_map::const_iterator it = _serverCgi.begin(); it != _serverCgi.end(); it++)
        {
            std::cout
                << "\tCgi [" << i << "] " << (*it).first << " = " << (*it).second << std::endl;
            i++;
        }
    }
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

void    Server::addErrorPages(const std::pair<std::string, std::vector<int> > & pages)
{
    _serverErrorPages.insert(pages);
    return ;
}

void Server::addCgi(const std::pair<std::string, std::string> cgi_pair)
{
    _serverCgi.insert(cgi_pair);
    return ;
}

void    Server::fillStruct(void)
{
    _serverSa.sin_family = AF_INET;
    _serverSa.sin_port = htons(_serverPort);
    if (inet_pton(AF_INET, _serverIP.c_str(), &_serverSa.sin_addr) != 1)
        throw std::runtime_error(RED "Error: inet_pton: " END + std::string(strerror(errno)));
    return ;
}

void    Server::fillSocket(void)
{
    _serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (_serverSocket == -1)
        throw std::runtime_error(RED "Error: socket: " END + std::string(strerror(errno)));
    int opt = 1;
    setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return ;
}

void    Server::launchServer(int & epoll_fd)
{
    if (bind(_serverSocket, (struct sockaddr *)&_serverSa, sizeof(_serverSa)) == -1)
        throw std::runtime_error(RED "Error: bind: " END + std::string(strerror(errno)));
    if (listen(_serverSocket, 4096) == -1)
        throw std::runtime_error(RED "Error: listen: " END + std::string(strerror(errno)));
    struct epoll_event  event;
    event.events = EPOLLIN;
    event.data.fd = _serverSocket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _serverSocket, &event) == -1)
        throw std::runtime_error("Error: epoll_ctl: " + std::string(strerror(errno)));
    return ;
}

void    Server::addIndex(const std::string & index)
{
    _serverIndexes.push_back(index);
    return ;
}

void    Server::clearIndex(void)
{
    _serverIndexes.clear();
    return ;
}

void    Server::deleteErrPage(const std::string & err_page)
{
    if (!_serverErrorPages.empty())
        _serverErrorPages.erase(err_page);
    return ;
}
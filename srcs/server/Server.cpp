#include "Server.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Server::Server(void) : _serverRoot("./")
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

std::vector<std::string> Server::getNames(void) const
{
    return (this->_serverName);
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
        std::cout << std::endl;
    }
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

void    Server::addErrorPages(const std::pair<std::string, std::vector<int> > & pages)
{
    _serverErrorPages.insert(pages);
    return ;
}
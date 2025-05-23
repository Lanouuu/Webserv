#include "Server.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Server::Server(void)
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

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

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
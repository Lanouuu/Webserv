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

void    Server::setName(const std::string & name)
{
    this->_serverName = name;
    return ;
}

void    Server::setRoot(const std::string & root)
{
    this->_serverRoot = root;
    return ;
}

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

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Server::addLocation(const std::string & name, const Location & location)
{
    this->_serverLocations.insert(std::make_pair(name, location));
    return ;
}
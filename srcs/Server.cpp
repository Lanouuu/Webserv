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
}

void    Server::setRoot(const std::string & root)
{
    this->_serverRoot = root;
}

void    Server::setHost(const std::string & host)
{
    this->_serverHost = host;
}

void    Server::setIndex(const std::string & index)
{
    this->_serverIndex = index;
}

void    Server::setPort(const uint16_t & port)
{
    this->_serverPort = port;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Server::addLocation(const std::string & name, const Location & location)
{
    this->_serverLocations.insert(std::make_pair(name, location));
}
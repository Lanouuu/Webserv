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

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

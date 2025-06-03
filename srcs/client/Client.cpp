#include "Client.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Client::Client(void) : _clientFd(-1)
{
    return ;
}

Client::Client(const Client & src)
{
    *this = src;
    return ;
}

Client::~Client(void)
{
    return ;
}

/****************************************************************************/
/*                               Operators                                  */
/****************************************************************************/

Client &    Client::operator=(const Client & rhs)
{
    this->_clientFd = rhs._clientFd;
    return (*this);
}

/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

void    Client::setFd(const int & fd)
{
    this->_clientFd = fd;
    return ;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/


int  &      Client::getClientFd(void)
{
    return _clientFd;
}


Request &   Client::getRequest(void)
{
    return _request;
}
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

int  &      Client::getClientFd(void)
{
    return _clientFd;
}


Request &   Client::getRequest(void)
{
    return _request;
}

std::string const & Client::getUid(void)
{
    return _uid;
}

void    Client::setFd(const int & fd)
{
    this->_clientFd = fd;
    return ;
}

void        Client::setUid(std::string const & uid)
{
    this->_uid = uid;
    return ;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

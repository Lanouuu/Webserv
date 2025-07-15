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

int  const &      Client::getClientFd(void) const
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

struct epoll_event & Client::getClientEpollStruct()
{
    return _client_event;
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

bool Client::RequestIsComplete() const 
{
    const char word[] = {'\r', '\n', '\r', '\n'};
    std::vector<char>::const_iterator it = std::search(_request.getRequest().begin(), _request.getRequest().end(), word, word + 4);
    if (it != _request.getRequest().end())
        return true;
    return false;
}
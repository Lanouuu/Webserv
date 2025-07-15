#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Request.hpp"

class Request;

class   Client
{
    public:

        Client(void);
        Client(const Client & src);
        ~Client(void);

        Client &    operator=(const Client & rhs);

        void        setFd(const int & fd);
        void        setUid(std::string const & uid);

        int const &         getClientFd(void) const;
        std::string const & getUid(void);
        Request &   getRequest(void);
        struct epoll_event & getClientEpollStruct();
        bool RequestIsComplete() const;

    private:

        int         _clientFd;
        std::string _uid;
        struct epoll_event _client_event;
        Request     _request;


};

typedef std::map<int, Client>   client_map;

#endif
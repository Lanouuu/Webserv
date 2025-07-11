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

        int &       getClientFd(void);
        std::string const & getUid(void);
        Request &   getRequest(void);

    private:

        int         _clientFd;
        std::string _uid;
        Request     _request;


};

typedef std::map<std::string, Client>   client_map;

#endif
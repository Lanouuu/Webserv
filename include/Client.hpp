#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Request.hpp"

class   Client
{
    public:

        Client(void);
        Client(const Client & src);
        ~Client(void);

        Client &    operator=(const Client & rhs);

        void        setFd(const int & fd);

        int &       getClientFd(void);
        Request &   getRequest(void);

    private:

        int _clientFd;
        Request _request;


};

typedef std::map<std::string, Client>   client_map;

#endif
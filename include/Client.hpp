#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <map>

class   Client
{
    public:

        Client(void);
        Client(const Client & src);
        ~Client(void);

        Client &    operator=(const Client & rhs);

        void        setFd(const int & fd);

    private:

        int _clientFd;

};

typedef std::map<std::string, Client>   client_map;

#endif
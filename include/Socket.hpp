#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Server.hpp"

class   Socket
{
    public:

        Socket(void);
        Socket(const Socket & src);
        ~Socket(void);

        Socket &    operator=(const Socket & rhs);

    private:

        int         _socketFD;
        uint16_t    _socketPort;
        std::string _socketIP;
        serv_vector _socketServers;


};

typedef std::map<int, Socket>   socket_map;

# endif
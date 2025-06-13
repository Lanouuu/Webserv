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

        int         getFD(void) const;
        uint16_t    getPort(void) const;
        std::string getIP(void) const;
        serv_vector getServers(void) const;

        void        fillSocket(Server & serv_temp, int epoll_fd);
        void        addServer(Server & serv_temp);

    private:

        int         _socketFD;
        sockaddr_in _socketSa;
        uint16_t    _socketPort;
        std::string _socketIP;
        serv_vector _socketServers;

        void        fillStruct(void);


};

typedef std::map<int, Socket>   socket_map;

# endif
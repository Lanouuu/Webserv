#ifndef SERVER_HPP
# define SERVER_HPP

# include <inttypes.h>
# include <map>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <string.h>
# include "Location.hpp"

typedef std::map<std::string, Location> location_map;

class   Server
{
    public:

        Server(void);
        ~Server(void);

        void            setHost(const std::string & host);
        void            setIndex(const std::string & index);
        void            setPort(const uint16_t & port);
        void            setIP(const std::string & ip);

        std::string     getRoot(void) const;
        uint16_t        getPort(void) const;
        std::string     getIP(void) const;
        location_map    getLocaMap(void) const;    

        void            printServNames(void) const;

        void            addName(const std::string & name);
        void            addLocation(const std::string & name, const Location & location);

        void            fillStruct(void);
        void            fillSocket(void);

        void            launchServer(int & epoll_fd);

    private:
    
        std::vector<std::string>    _serverName;
        std::string                 _serverRoot;
        std::string                 _serverHost;
        std::string                 _serverIndex;
        std::string                 _serverIP;
        uint16_t                    _serverPort;
        location_map                _serverLocations;
        sockaddr_in                 _serverSa;
        int                         _serverSocket;
};

#endif
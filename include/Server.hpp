#ifndef SERVER_HPP
# define SERVER_HPP

# include <inttypes.h>
# include <map>
# include "Location.hpp"

typedef std::map<std::string, Location> location_map;

class   Server
{
    public:

        Server(void);
        ~Server(void);

        void        addName(const std::string & name);

        void            setHost(const std::string & host);
        void            setIndex(const std::string & index);
        void            setPort(const uint16_t & port);
        void            setIP(const std::string & ip);

        std::string     getRoot(void) const;
        uint16_t        getPort(void) const;
        std::string     getIP(void) const;
        location_map    getLocaMap(void) const;    

        void        printServNames(void) const;

        void        addLocation(const std::string & name, const Location & location);

    private:
    
        std::vector<std::string>    _serverName;
        std::string                 _serverRoot;
        std::string                 _serverHost;
        std::string                 _serverIndex;
        std::string                 _serverIP;
        uint16_t                    _serverPort;
        location_map                _serverLocations;
};

#endif
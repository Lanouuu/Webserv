#ifndef SERVER_HPP
# define SERVER_HPP

# include <inttypes.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <string.h>
# include "Location.hpp"

typedef std::map<std::string, Location> location_map;
typedef std::map<std::string, std::vector<int> > errpage_map;

class   Server
{
    public:

        Server(void);
        ~Server(void);

        void                        setHost(const std::string & host);
        void                        setPort(const uint16_t & port);
        void                        setIP(const std::string & ip);
        void                        setBodySize(const size_t & size);

        std::string                 getRoot(void) const;
        uint16_t                    getPort(void) const;
        std::string                 getIP(void) const;
        location_map                getLocaMap(void) const;
        std::vector<std::string>    getNames(void) const;
        std::vector<std::string>    getIndexes(void) const;
        int const &                 getSocket(void) const;
        size_t &                    getBodySize(void);
        cgi_map                     getCgi(void) const;
        const errpage_map           getErrPages(void) const;               

        void                        printServNames(void) const;
        void                        printErrorPage(void) const;
        void                        printIndexes(void) const;
        void                        printCgi(void) const;

        void                        addName(const std::string & name);
        void                        addLocation(const std::string & name, const Location & location);
        void                        addErrorPages(const std::pair<std::string, std::vector<int> > & pages);
        void                        addIndex(const std::string & index);
        void                        addCgi(const std::pair<std::string, std::string> cgi_pair);
        void                        clearIndex(void);
        void                        deleteErrPage(const std::string & err_page);

        void                        fillStruct(void);
        void                        fillSocket(void);
        void                        launchServer(int & epoll_fd);

    private:
    
        std::vector<std::string>    _serverName;
        std::string                 _serverRoot;
        std::string                 _serverHost;
        std::vector<std::string>    _serverIndexes;
        errpage_map                 _serverErrorPages;
        std::string                 _serverIP;
        uint16_t                    _serverPort;
        location_map                _serverLocations;
        sockaddr_in                 _serverSa;
        int                         _serverSocket;
        size_t                      _serverBodySize;
        cgi_map                     _serverCgi;
};
  
#endif
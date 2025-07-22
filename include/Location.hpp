#ifndef LOCATION_HPP
# define LOCATION_HPP

# define GREEN 	"\033[0;32m"
# define RED	"\033[0;31m"
# define BLUE	"\033[0;36m"
# define END	"\033[0m"

# include <iostream>
# include <map>
# include <map>
# include <vector>
# include <exception>
# include <errno.h>
# include <algorithm>
# include <limits>
# include <utility>

typedef std::map<std::string, std::string> cgi_map;

typedef std::map<std::string, std::string> cgi_map;

class   Location
{
    public:

        Location(void);
        Location(const Location & src);
        ~Location(void);

        Location &                          operator=(const Location & rhs);

        void                                setBaseUri(const std::string & uri);
        void                                setUrl(const std::string & url);
        void                                setAlias(const std::string & alias);
        void                                setUpload(const std::string & path);
        void                                setAutoIndex(const bool & autoindex);
        void                                setIsDirectory(const bool & isdirectory);
        void                                setIndexes(const std::vector<std::string> & indexes);
        void                                setBodySize(const size_t & size);
        void                                setCgi(const cgi_map cgi);

        std::string                         getBaseUri(void) const;
        std::string                         getUrl(void) const;
        std::string                         getAlias(void) const;
        std::vector<std::string> const &    getIndexes(void) const;
        bool                                getAutoIndex(void) const;
        bool                                getIsDirectory(void) const;
        size_t                              getBodySize(void) const;
        cgi_map                             getCgi(void) const;

        void                                addMethod(const std::string & method);
        void                                addIndex(const std::string & index);
        void                                addCgi(const std::pair<std::string, std::string> cgi_pair);

        void                                uploadIndex(const std::string & uri);
        void                                clearIndex(void);
        void                                clearMethods(void);
        void                                clearCgi(void);

        void                                printIndexes(void) const;
        void                                printMethod(void) const;
        void                                printCgi(void) const;

    private:
    
        std::string                         _locationBaseUri;
        std::string                         _locationRoot;
        std::string                         _locationAlias;
        std::string                         _locationUrl;
        std::vector<std::string>            _locationIndexes;
        std::string                         _locationUpload;
        std::vector<std::string>            _locationMethod;
        bool                                _locationAutoIndex;
        bool                                _isDirectory;
        size_t                              _locationBodySize;
        cgi_map                             _locationCgi;
};

#endif
#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <exception>

class   Location
{
    public:

        Location(void);
        Location(const Location & src);
        ~Location(void);

        Location &  operator=(const Location & rhs);

        void        setBaseUri(const std::string & uri);
        void        setUrl(const std::string & url);
        void        setAlias(const std::string & alias);
        void        setIndex(const std::string & index);
        void        setUpload(const std::string & path);

        std::string getBaseUri(void) const;
        std::string getUrl(void) const;
        std::string getIndex(void) const;

        void    addMethod(const std::string & method);

    private:
    
        std::string                 _locationBaseUri;
        std::string                 _locationRoot;
        std::string                 _locationAlias;
        std::string                 _locationUrl;
        std::string                 _locationIndex;
        std::string                 _locationUpload;
        std::vector<std::string>    _locationMethod;

};

#endif
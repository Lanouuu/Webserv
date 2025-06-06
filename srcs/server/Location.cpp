#include "Location.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Location::Location(void) : _locationRoot("./")
{
    this->_locationIndex.push_back("index.html");
    return ;
}

Location::Location(const Location & src)
{
    *this = src;
    return ;
}

Location::~Location(void)
{
    return ;
}

/****************************************************************************/
/*                               Operators                                  */
/****************************************************************************/

Location &  Location::operator=(const Location & rhs)
{
    if (this != &rhs)
    {
        this->_locationBaseUri = rhs._locationBaseUri;
        this->_locationRoot = rhs._locationRoot;
        this->_locationUrl = rhs._locationUrl;
        this->_locationAlias = rhs._locationAlias;
        this->_locationIndex = rhs._locationIndex;
        this->_locationUpload = rhs._locationUpload;
        this->_locationMethod = rhs._locationMethod;
    }
    return (*this);
}

/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

void    Location::setBaseUri(const std::string & uri)
{
    this->_locationBaseUri = uri;
    return ;
}

void    Location::setUrl(const std::string & url)
{
    this->_locationUrl = url;
    return ;
}

void    Location::setAlias(const std::string & alias)
{
    this->_locationAlias = alias;
    return ;
}

void    Location::setUpload(const std::string & path)
{
    this->_locationUpload = path;
    return ;
}

std::string Location::getUrl(void) const
{
    return (this->_locationUrl);
}

std::string Location::getBaseUri(void) const
{
    return (this->_locationBaseUri);
}

std::string Location::getAlias(void) const
{
    return (this->_locationAlias);
}


/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Location::printIndexes(void) const
{
    std::cout
        << "\t" << "Indexes = " << std::flush;
    for (std::vector<std::string>::const_iterator it = _locationIndex.begin(); it != _locationIndex.end(); it++)
        std::cout << (*it) << " " << std::flush;
    std::cout << std::endl;
    return ;
}

void    Location::addMethod(const std::string & method)
{
    this->_locationMethod.push_back(method);
    return ;
}

void    Location::addIndex(const std::string & index)
{
    this->_locationIndex.push_back(index);
    return ;
}

void    Location::uploadIndex(const std::string & url)
{
    for (std::vector<std::string>::iterator it = _locationIndex.begin(); it != _locationIndex.end(); it++)
    {
        std::string buf = url + (*it);
        (*it) = buf;
    }
    return ;
}
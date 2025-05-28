#include "Location.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Location::Location(void) : _locationRoot("./"), _locationIndex("index.html")
{
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

void    Location::setIndex(const std::string & index)
{
    this->_locationIndex = index;
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

std::string Location::getIndex(void) const
{
    return (this->_locationIndex);
}

std::string Location::getBaseUri(void) const
{
    return (this->_locationBaseUri);
}


/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Location::addMethod(const std::string & method)
{
    this->_locationMethod.push_back(method);
    return ;
}
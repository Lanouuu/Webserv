#include "Location.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Location::Location(void) : _locationRoot("./"), _locationAutoIndex(false), _isDirectory(false)
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
        this->_locationIndexes = rhs._locationIndexes;
        this->_locationUpload = rhs._locationUpload;
        this->_locationMethod = rhs._locationMethod;
        this->_locationAutoIndex = rhs._locationAutoIndex;
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

void    Location::setAutoIndex(const bool & autoindex)
{
    this->_locationAutoIndex = autoindex;
    return ;
}

void    Location::setIsDirectory(const bool & isdirectory)
{
    this->_isDirectory = isdirectory;
    return ;
}

void    Location::setIndexes(const std::vector<std::string> & indexes)
{
    this->_locationIndexes = indexes;
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

bool    Location::getAutoIndex(void) const
{
    return (this->_locationAutoIndex);
}

bool    Location::getIsDirectory(void) const
{
    return (this->_isDirectory);
}

std::vector<std::string> const &    Location::getIndexes(void) const
{
    return (this->_locationIndexes);
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Location::printIndexes(void) const
{
    std::cout
        << "\t" << "Indexes = " << std::flush;
    for (std::vector<std::string>::const_iterator it = _locationIndexes.begin(); it != _locationIndexes.end(); it++)
        std::cout << (*it) << " " << std::flush;
    std::cout << std::endl;
    return ;
}

void    Location::printMethod(void) const
{
    if (!_locationMethod.empty())
    {
        std::cout
            << "\t" << "Methods = " << std::flush;
        for (std::vector<std::string>::const_iterator it = _locationMethod.begin(); it != _locationMethod.end(); it++)
            std::cout << (*it) << " " << std::flush;
        std::cout << std::endl;
    }
    return ;
}

void    Location::addMethod(const std::string & method)
{
    this->_locationMethod.push_back(method);
    return ;
}

void    Location::addIndex(const std::string & index)
{
    this->_locationIndexes.push_back(index);
    return ;
}

void    Location::uploadIndex(const std::string & url)
{
    for (std::vector<std::string>::iterator it = _locationIndexes.begin(); it != _locationIndexes.end(); it++)
    {
        std::string buf = url + (*it);
        (*it) = buf;
    }
    return ;
}

void    Location::clearIndex(void)
{
    _locationIndexes.clear();
    return ;
}

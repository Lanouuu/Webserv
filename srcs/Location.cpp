#include "Location.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Location::Location(void)
{
    return ;
}

Location::Location(const Location & src)
{
    *this = src;
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
        this->_locationRoot = rhs._locationRoot;
        this->_locationIndex = rhs._locationIndex;
        this->_locationUpload = rhs._locationUpload;
        this->_locationMethod = rhs._locationMethod;
    }
    return (*this);
}

/****************************************************************************/
/*                           Getters / Server::Setters                      */
/****************************************************************************/

void    Location::setRoot(const std::string & root)
{
    this->_locationRoot = root;
}

void    Location::setIndex(const std::string & index)
{
    this->_locationIndex = index;
}

void    Location::setUpload(const std::string & path)
{
    this->_locationUpload = path;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

void    Location::addMethod(const std::string & method)
{
    this->_locationMethod.push_back(method);
}
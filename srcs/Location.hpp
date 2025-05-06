#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>

class   Location
{
    public:

        Location();
        Location(const Location & src);
        ~Location();

        Location &  operator=(const Location & rhs);

        void    setRoot(const std::string & root);
        void    setIndex(const std::string & index);
        void    setUpload(const std::string & path);

        void    addMethod(const std::string & method);

    private:
        std::string                 _locationRoot;
        std::string                 _locationIndex;
        std::string                 _locationUpload;
        std::vector<std::string>    _locationMethod;

};

#endif
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

        void    setAlias(const std::string & alias);
        void    setIndex(const std::string & index);
        void    setUpload(const std::string & path);

        void    addMethod(const std::string & method);

    private:
    
        std::string                 _locationAlias;
        std::string                 _locationIndex;
        std::string                 _locationUpload;
        std::vector<std::string>    _locationMethod;

};

#endif
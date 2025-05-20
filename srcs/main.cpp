
#include "parsing.hpp"
#include <exception>

int main(int ac, char **av, char **env)
{
    (void)env;

    try
    {
        server_map   servers;

        if (ac > 2)
            throw std::invalid_argument("Error: too many arguments");
        if (ac == 2)
            parsingConfFile(av[1], servers);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl << std::endl;
        return (1);
    }
    
    return (0);
}
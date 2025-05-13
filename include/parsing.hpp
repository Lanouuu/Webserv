#ifndef PARSING_HPP
# define PARSING_HPP

# include "Server.hpp"

typedef std::map<std::string, Server>   server_map;

void    parsingConfFile(std::string file, server_map & servers);

#endif
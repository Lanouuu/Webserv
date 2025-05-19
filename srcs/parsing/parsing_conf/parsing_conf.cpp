#include "parsing.hpp"

void    parsingConfFile(std::string file, server_map & servers)
{
    Parser   parser(file);

    parser.printTokens();
    parser.confParser(servers);
    return ;
}
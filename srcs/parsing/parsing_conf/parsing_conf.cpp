#include "parsing.hpp"

void    parsingConfFile(std::string file, serv_vector & servers)
{
    Parser   parser(file);

    parser.printTokens();
    parser.parseConf(servers);
    return ;
}
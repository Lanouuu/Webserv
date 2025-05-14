#include "parsing.hpp"

void    parsingConfFile(std::string file, server_map & servers)
{
    Lexer   lexer(file);
    (void)servers;

    lexer.printTokens();
    return ;
}
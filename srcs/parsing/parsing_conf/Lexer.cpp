#include "Lexer.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Lexer::Lexer(const std::string file)
{
    return ;
}

Lexer::~Lexer(void)
{
    return ;
}

/****************************************************************************/
/*                               Operators                                  */
/****************************************************************************/

/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

bool    Lexer::checkEmptyLine(const std::string & line)
{
    if (line.find_first_not_of(" \t") == std::string::npos || line.empty())
        return (true);
    return (false);
}

void    Lexer::tokenizer(const std::string & line, int n_line)
{
}

/*
tokenizer 

identifier (string) (bracket)
identifier (string) (semicolon)

*/

void    Lexer::loadFile(const std::string & file)
{
    std::ifstream               conf_file(file.c_str());
    std::vector<std::string>    buf;

    if(!conf_file.is_open())
        throw std::invalid_argument("Error: " + file + ": failed to open");
    for (std::string line; std::getline(conf_file, line);)
        buf.push_back(line);
    int n_line = 1;
    for (std::vector<std::string>::const_iterator it = buf.begin(); it != buf.end(); ++it)
    {
        if (!checkEmptyLine(*it))
            tokenizer(*it, n_line);
        n_line++;
    }
}
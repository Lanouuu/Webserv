#include "Lexer.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Lexer::Lexer(const std::string file)
{
    loadFile(file);
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

void    Lexer::printTokens(void) const
{
    for (std::vector<t_token>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++)
    {
        std::cout
            << "Type: " << (*it).type << ", "
            << "Value: \"" << (*it).value << "\", "
            << "Line: " << (*it).line << ", "
            << "Pos: " << (*it).pos
            << std::endl;
    }
    return ;
}

bool    Lexer::checkEmptyLine(const std::string & line)
{
    if (line.find_first_not_of(" \t") == std::string::npos || line.empty())
        return (true);
    return (false);
}

void    Lexer::addDelimToken(int type, const std::string value, size_t n_line, size_t pos)
{
    t_token token;

    if (value == "{" && !this->_tokens.empty())
    {
        for (std::vector<t_token>::reverse_iterator it = this->_tokens.rbegin(); it != this->_tokens.rend(); it++)
        {
            if ((*it).type == Identifier)
            {
                (*it).type = Block;
                break ;
            }
        }
    }
    token.type = type;
    token.value = value;
    token.line = n_line;
    token.pos = pos;
    this->_tokens.push_back(token);
    return ;
}

void    Lexer::addStrToken(const std::string value, size_t n_line, size_t pos)
{
    t_token token;
    t_token last_token;
    
    if (this->_tokens.empty())
        token.type = Identifier;
    if (!this->_tokens.empty())
    {
        last_token = this->_tokens.back();
        if (last_token.type == OpenBrace || last_token.type == CloseBrace || last_token.type == Semicolon)
            token.type = Identifier;
        else
            token.type = String;
    }
    token.value = value;
    token.line = n_line;
    token.pos = pos;
    this->_tokens.push_back(token);
    return ;
}

void    Lexer::tokenizer(const std::string & line, size_t n_line)
{
    std::string delimiter = " \t;{}#";
    for (std::string::size_type pos = 0; pos < line.size(); pos++)
    {
        if (line[pos] == '#')
            break ;
        if (line[pos] == ' ' || line[pos] == '\t')
            pos++;
        if (line[pos] == '{')
            addDelimToken(OpenBrace, &(line[pos]), n_line, pos);
        else if (line[pos] == '}')
            addDelimToken(CloseBrace, &(line[pos]), n_line, pos);
        else if (line[pos] == ';')
            addDelimToken(Semicolon, &(line[pos]), n_line, pos);
        else
        {
            std::string buf;
            size_t temp_pos = pos;
            for (; delimiter.find(line[pos]) == std::string::npos; pos++)
                buf.push_back(line[pos]);
            if (!buf.empty())
                addStrToken(buf, n_line, temp_pos);
        }
        if (line[pos] == '#')
        pos--;
    }
    return ;
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
    size_t n_line = 1;
    for (std::vector<std::string>::const_iterator it = buf.begin(); it != buf.end(); ++it)
    {
        if (!checkEmptyLine(*it))
            tokenizer(*it, n_line);
        n_line++;
    }
    return ;
}
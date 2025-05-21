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
    for (std::vector<t_token>::const_iterator it = _tokens.begin(); it != _tokens.end(); it++)
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

void    Lexer::addDelimToken(int type, const char value, size_t n_line, size_t pos)
{
    t_token     token;
    std::string buf;

    buf.push_back(value);
    if (value == '{' && !_tokens.empty())
    {
        for (std::vector<t_token>::reverse_iterator it = _tokens.rbegin(); it != _tokens.rend(); it++)
        {
            if ((*it).type == Identifier)
            {
                (*it).type = Block;
                break ;
            }
        }
    }
    token.type = type;
    token.value = buf;
    token.line = n_line;
    token.pos = pos;
    _tokens.push_back(token);
    return ;
}

void    Lexer::addStrToken(const std::string value, size_t n_line, size_t pos)
{
    t_token token;
    t_token last_token;
    
    if (_tokens.empty())
        token.type = Identifier;
    if (!_tokens.empty())
    {
        last_token = _tokens.back();
        if (last_token.type == OpenBrace || last_token.type == CloseBrace || last_token.type == Semicolon)
            token.type = Identifier;
        else
            token.type = String;
    }
    token.value = value;
    token.line = n_line;
    token.pos = pos;
    _tokens.push_back(token);
    return ;
}

void    Lexer::tokenizer(const std::string & line, size_t n_line)
{
    std::string delimiter = " \t{};#";
    for (std::string::size_type pos = 0; line[pos]; pos++)
    {
        while (line[pos] == ' ' || line[pos] == '\t')
            pos++;
        if (pos >= line.size())
            break ;
        if (line[pos] == '{')
            addDelimToken(OpenBrace, line[pos], n_line, pos);
        else if (line[pos] == '}')
            addDelimToken(CloseBrace, line[pos], n_line, pos);
        else if (line[pos] == ';')
            addDelimToken(Semicolon, line[pos], n_line, pos);
        else if (line[pos] == '#')
            break ;
        else
        {
            std::string buf;
            size_t temp_pos = pos;
            for (; delimiter.find(line[pos]) == std::string::npos; pos++)
                buf.push_back(line[pos]);
            pos--;
            if (!buf.empty())
                addStrToken(buf, n_line, temp_pos);
        }
    }
    return ;
}

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
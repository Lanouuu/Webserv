#include "Lexer.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Lexer::Lexer(const std::string file) : _fileName(file)
{
    getServIdent();
    getLocaIdent();
    loadFile(file);
    return ;
}

Lexer::~Lexer(void)
{
    return ;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/


/*********Error*********/


std::string Lexer::tokenErr(std::string error, t_token & token)
{
    std::ostringstream buf;

    buf << std::endl
        << RED << "Error: " << END
        << _fileName << ":" << token.line << ":" << token.pos + 1
        << RED << " error: " << END << error 
        << std::endl
        << token.line << " | " << token.value;
    return (buf.str());
}


/*********Lexer init*********/


void    Lexer::getServIdent(void)
{
    _servID.insert("server_name");
    _servID.insert("listen");
    _servID.insert("error_page");
    return ;
}

void    Lexer::getLocaIdent(void)
{
    _locaID.insert("index");
    _locaID.insert("set_method");
    _locaID.insert("return");
    _locaID.insert("alias");
    _locaID.insert("cgi");
    _locaID.insert("autoindex");
    _locaID.insert("index");
    return ;
}


/*********Tokenizer*********/


void    Lexer::addDelimToken(int type, const char value, size_t n_line, size_t pos)
{
    t_token     token;
    std::string buf;

    buf.push_back(value);
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
    
    token.value = value;
    token.line = n_line;
    token.pos = pos;
    if (token.value == "server" || token.value == "location")
        token.type = Block;
    else if (std::find(_servID.begin(), _servID.end(), value) != _servID.end()
        || std::find(_locaID.begin(), _locaID.end(), value) != _locaID.end())
        token.type = Identifier;
    else
        token.type = String;
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
            {
                if (pos >= line.size())
                    break;
                buf.push_back(line[pos]);
            }
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
    if (buf.empty())
        throw std::invalid_argument("Error: " + file + ": empty file");
    size_t n_line = 1;
    for (std::vector<std::string>::const_iterator it = buf.begin(); it != buf.end(); ++it)
    {
        if (!checkEmptyLine(*it))
            tokenizer(*it, n_line);
        n_line++;
    }
    return ;
}


/*********Lexer utils*********/


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
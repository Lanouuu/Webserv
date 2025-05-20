#include "Parser.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Parser::Parser(const std::string file) : Lexer(file), _fileName(file), _current(_tokens.begin())
{
    getValidBlocks();
    getServIdent();
    getLocaIdent();
    return ;
}

Parser::~Parser(void)
{
    return ;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/


/*********Parser init*********/


void    Parser::getValidBlocks(void)
{
    this->_validBlocks.insert("server");
    this->_validBlocks.insert("location");
    return ;
}

void    Parser::getServIdent(void)
{
    this->_servIdentifiers.insert("server_name");
    this->_servIdentifiers.insert("listen");
    this->_servIdentifiers.insert("root");
    this->_servIdentifiers.insert("error_page");
    return ;
}

void    Parser::getLocaIdent(void)
{
    this->_locaIdentifiers.insert("index");
    this->_locaIdentifiers.insert("set_method");
    this->_locaIdentifiers.insert("return");
    this->_locaIdentifiers.insert("root");
    this->_locaIdentifiers.insert("cgi");
    this->_locaIdentifiers.insert("autoindex");
    this->_locaIdentifiers.insert("index");
    return ;
}


/*********Error*********/


std::string Parser::tokenError(std::string error, t_token & token)
{
    std::ostringstream buf;

    buf << std::endl
        << RED << "Error: " << END
        << this->_fileName << ":" << token.line << ":" << token.pos
        << RED << " error: " << END << error 
        << std::endl
        << token.line << " | " << token.value;
    return (buf.str());
}


/*********Parsing*********/


void    Parser::parseConf(server_map & servers)
{
    Server  serv_temp;

    while (_current != _tokens.end())
    {
        if (match("server") && expect(Block))
            parseServer(serv_temp);
        else
            throw std::invalid_argument(tokenError("expected server block", *this->_current));
    }
    servers.insert(std::make_pair(serv_temp.getName(), serv_temp));
    return ;
}

void    Parser::parseServer(Server & serv_temp)
{
    (void)serv_temp;
    return ;
}


/*********Parsing utils*********/


t_token *Parser::peek(int offset)
{
    std::vector<t_token>::iterator it = _current + offset;
    if (it >= _tokens.end())
        return NULL;
    return &(*it);
}

bool    Parser::match(std::string key)
{
    if (this->_current->value == key)
        return true;
    return false;
}

bool    Parser::expect(int type)
{
    if (this->_current->type == type)
    {
        ++this->_current;
        return true;
    }
    return false;
}

void    Parser::advance(void)
{
    ++this->_current;
}
#include "Parser.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Parser::Parser(const std::string file) : Lexer(file), _fileName(file)
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
/*                               Operators                                  */
/****************************************************************************/

/****************************************************************************/
/*                           Getters / Setters                              */
/****************************************************************************/

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/

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

std::string Parser::tokenError(std::string error, t_token & token)
{
    std::ostringstream buf;

    buf << std::endl
        << RED << "Error: " << END
        << this->_fileName << ":" << token.line << ":" << token.pos
        << RED << " error: " << END << error 
        << std::endl
        << token.line << " | " << token.value
        << std::endl;
    return (buf.str());
}

void    Parser::checkTokens(void)
{
    if (this->_tokens.begin()->value != "server")
        throw std::invalid_argument(tokenError("expected server block", *(this->_tokens.begin())));
}

void    Parser::confParser(server_map & servers)
{
    (void)servers;
    checkTokens();
    return ;
}

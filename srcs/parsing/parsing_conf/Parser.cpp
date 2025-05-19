#include "Parser.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Parser::Parser(const std::string file) : Lexer(file)
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

void    tokenError(std::string & value, size_t line, size_t pos)
{
    //std::string buf = "Error: " + value + itoa(line)
}

void    Parser::checkTokens(void) const
{
    if (this->_tokens.begin()->value != "server")
        throw   std::invalid_argument("Error: " + this->_tokens.begin()->value + ".conf:" +)
}

void    Parser::confParser(server_map & servers)
{
    (void)servers;
    checkTokens();
    return ;
}

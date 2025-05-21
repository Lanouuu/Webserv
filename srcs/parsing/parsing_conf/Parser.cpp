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
    _validBlocks.insert("server");
    _validBlocks.insert("location");
    return ;
}

void    Parser::getServIdent(void)
{
    _servID.insert("server_name");
    _servID.insert("listen");
    _servID.insert("root");
    _servID.insert("error_page");
    return ;
}

void    Parser::getLocaIdent(void)
{
    _locaID.insert("index");
    _locaID.insert("set_method");
    _locaID.insert("return");
    _locaID.insert("root");
    _locaID.insert("cgi");
    _locaID.insert("autoindex");
    _locaID.insert("index");
    return ;
}


/*********Error*********/


std::string Parser::tokenErr(std::string error, t_token & token)
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


/*********Parsing*********/


void    Parser::parseConf(server_map & servers)
{
    Server  serv_temp;

    this->printTokens();

    while (_current != _tokens.end())
    {
        if (match("server"))
            parseServer(serv_temp);
        else
            throw std::invalid_argument(tokenErr("expected server block", *_current));
        //_current++;
    }
    servers.insert(std::make_pair(serv_temp.getName(), serv_temp));
    return ;
}

void    Parser::parseServer(Server & serv_temp)
{
    advance();
    checkEOF();
    if (!expect(OpenBrace))
        throw std::invalid_argument(tokenErr("expected \"{\" after server identifier", *_current));
    while (_current->type != CloseBrace && _current != _tokens.end())
    {
        if (_current->type == Identifier)
            parseServDirective(serv_temp);
        else if(_current->type == Block && match("location"))
            parseLocation(serv_temp);
    }
    return ;
}

void    Parser::parseLocation(Server & serv_temp)
{
    (void)serv_temp;
    return ;
}

void    Parser::parseServDirective(Server & serv_temp)
{
    if (std::find(_servID.begin(), _servID.end(), _current->value) == _servID.end())
        throw std::invalid_argument(tokenErr("invalid server directive", *_current));
    std::string value = _current->value;
    if (match("server_name") || match("listen") || match("root"))
    {
        advanceAndCheck();
        if (_current->type == String)
        {
            if (value == "server_name")
                serv_temp.setName(_current->value);
           // else if (value == "listen")
               // serv_temp.setPort(_current->value);
            else if (value == "root")
                serv_temp.setRoot(_current->value);
        }
        else
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        advanceAndCheck();
        // sur cette regle verifier qu'il n'y ait qu'une seule value apres l'identifier
    }
    if (!expect(Semicolon))
        throw std::invalid_argument(tokenErr("expected \";\" at the end of directive", *peek(-1)));
    return ;
}

void    Parser::parseLocaDirective(Location & loca_temp)
{
    (void)loca_temp;
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
    if (_current->value == key)
        return true;
    return false;
}

bool    Parser::expect(int type)
{
    if (_current != _tokens.end() && _current->type == type)
    {
        ++_current;
        return true;
    }
    return false;
}

void    Parser::advance(void)
{
    if (_current != _tokens.end())
        ++_current;
    return ;
}

void    Parser::checkEOF(void)
{
    if (_current == _tokens.end())
        throw std::invalid_argument(tokenErr("invalid EOF", *_current));
    return ;
}

void    Parser::advanceAndCheck(void)
{
    if (_current != _tokens.end())
        ++_current;
    checkEOF();
    return ;
}
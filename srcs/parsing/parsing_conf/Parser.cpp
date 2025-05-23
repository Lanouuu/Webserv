#include "Parser.hpp"

/****************************************************************************/
/*                      Constructors / Destructors                          */
/****************************************************************************/

Parser::Parser(const std::string file) : Lexer(file), _current(_tokens.begin())
{
    return ;
}

Parser::~Parser(void)
{
    return ;
}

/****************************************************************************/
/*                           Members Functions                              */
/****************************************************************************/


/*********Parsing*********/


void    Parser::parseConf(serv_vector & servers)
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
    servers.push_back(serv_temp);
    return ;
}

void    Parser::parseServer(Server & serv_temp)
{
    advanceAndCheck();
    if (!expect(OpenBrace))
        throw std::invalid_argument(tokenErr("expected \"{\" after server identifier", *peek(-1)));
    while (_current->type != CloseBrace && _current != _tokens.end())
    {
        if (_current->type == OpenBrace || _current->type == Semicolon)
            throw std::invalid_argument(tokenErr("invalid delimiter \"" + _current->value + "\"", *_current));
        else if (_current->type == String)
            throw std::invalid_argument(tokenErr("invalid server directive", *_current));
        else if (_current->type == Identifier)
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
    std::string value = _current->value;
    if (match("listen"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        if (value == "listen")
            parsePort(serv_temp, _current->value);
        advanceAndCheck();
        if (_current->type == String)
            throw std::invalid_argument(tokenErr("too many value for \"" + value + "\" directive", *_current));
    }
    else if (match("server_name"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        while (_current->type == String)
        {
            if (value == "server_name")
                serv_temp.addName(_current->value);
            advanceAndCheck();
        }
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


/*********Parsing Ip/port*********/


void    Parser::parsePort(Server & serv_temp, std::string & value) 
{
    long        port = -1;
    std::string str_ip;

    if (value.find_first_not_of("0123456789") == std::string::npos)
    {
        port = strtol(value.c_str(), NULL, 10);
        str_ip = "0.0.0.0";
    }
    else
    {
        std::string str_port;
        if (std::count(value.begin(), value.end(), ':') != 1)
            throw std::invalid_argument(tokenErr("invalid port", *_current));
        str_port = value.substr(value.find(':') + 1, value.size());
        if (str_port.empty())
            throw std::invalid_argument(tokenErr("invalid port", *_current));
        if (str_port.find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument(tokenErr("invalid port", *_current));
        port = strtol(str_port.c_str(), NULL, 10);
        str_ip = parseIP(value.substr(0, value.find(':')));
    }
    if (port == ERANGE || port < 0 || port > 65535)
        throw std::invalid_argument(tokenErr("invalid port [0-65535]", *_current));
    serv_temp.setPort(static_cast<uint16_t>(port));
    serv_temp.setIP(str_ip);
}

std::string Parser::parseIP(std::string value)
{
    if (value == "*")
        return ("0.0.0.0");
    if (value.find_first_not_of("0123456789.") != std::string::npos 
        || std::count(value.begin(), value.end(), '.') != 3
        || value.empty())
        throw std::invalid_argument(tokenErr("invalid IP address", *_current));
    return (value);
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
        throw std::invalid_argument(tokenErr("invalid EOF", *peek(-1)));
    return ;
}

void    Parser::advanceAndCheck(void) 
{
    if (_current != _tokens.end())
        ++_current;
    checkEOF();
    return ;
}
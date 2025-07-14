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


/*********Parsing Server*********/


void    Parser::parseConf(serv_vector & servers)
{
    int     nb_serv = 0;
    
    while (_current != _tokens.end())
    {
        Server  serv_temp;
        if (!match("server") && nb_serv == 0)
            throw std::invalid_argument(tokenErr("expected server block", *_current));
        else if (match("server"))
            parseServer(serv_temp);
        else
            throw std::invalid_argument(tokenErr("expected server block or EOF", *_current));
        nb_serv++;
        _current++;
        servers.push_back(serv_temp);
    }
    return ;
}

void    Parser::parseServer(Server & serv_temp)
{
    int nb_location = 0;
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
        {
            if (nb_location > 0)
            throw std::invalid_argument(tokenErr("server directive is forbidden after location block", *_current));
            parseServDirective(serv_temp);
        }
        else if(_current->type == Block && match("location"))
        {
            parseLocation(serv_temp);
            nb_location++;
        }
    }
    checkEOF();
    return ;
}

void    Parser::parseServDirective(Server & serv_temp)
{
    std::string value = _current->value;
    if (match("listen") || match("max_body_size"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        if (value == "listen")
            parsePort(serv_temp, _current->value);
        else if (value == "max_body_size")
            serv_temp.setBodySize(parseBodySize(_current->value));
        advanceAndCheck();
        if (_current->type == String)
            throw std::invalid_argument(tokenErr("too many value for \"" + value + "\" directive", *_current));
    }
    else if (match("server_name") || match("error_page") || match("index") || match("cgi"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        if (value == "index")
            serv_temp.clearIndex();
        while (_current->type == String)
        {
            if (value == "server_name")
            {
                serv_temp.addName(_current->value);
                advanceAndCheck();
            }
            else if (value == "error_page")
                parseErrorPage(serv_temp);
            else if (value == "index")
            {
                serv_temp.addIndex(_current->value);
                advanceAndCheck();
            }
            else if (value == "cgi")
                parseCgi(serv_temp);
        }
    }
    if (!expect(Semicolon))
        throw std::invalid_argument(tokenErr("expected \";\" at the end of directive", *peek(-1)));
    return ;
}


/*********Parsing Location*********/


void    Parser::parseLocation(Server & serv_temp)
{
    advanceAndCheck();
    if (_current->type != String)
        throw std::invalid_argument(tokenErr("expected path after location", *peek(-1)));
    if (_current->value[0] != '/')
        throw std::invalid_argument(tokenErr("invalid URI", *_current));
    if (_current->value[_current->value.length() - 1] == '/')
        parseLocDir(serv_temp);
    else
    {
        std::vector<t_token>::iterator temp = _current;
        parseLocFile(serv_temp);
        _current = temp;
        parseLocDir(serv_temp);
    }
    if (!expect(CloseBrace))
        throw std::invalid_argument(tokenErr("expect \'}\' atthe end of location block", *_current));
    return ;
}

void    Parser::parseLocFile(Server & serv_temp)
{
    Location loca_temp;

    loca_temp.setBaseUri(_current->value);
    loca_temp.setUrl("." + _current->value);
    loca_temp.setIndexes(serv_temp.getIndexes());
    loca_temp.setBodySize(serv_temp.getBodySize());
    loca_temp.setCgi(serv_temp.getCgi());
    advanceAndCheck();
    if (!expect(OpenBrace))
        throw std::invalid_argument(tokenErr("expected \"{\" after location identifier", *peek(-2)));
    while (_current->type != CloseBrace && _current != _tokens.end())
    {
        if (_current->type == OpenBrace || _current->type == Semicolon)
            throw std::invalid_argument(tokenErr("invalid delimiter \"" + _current->value + "\"", *_current));
        else if (_current->type == String || _current->type == Block)
            throw std::invalid_argument(tokenErr("invalid server directive", *_current));
        else if (_current->type == Identifier)
            parseLocaDirective(loca_temp);
    }
    serv_temp.addLocation(loca_temp.getBaseUri(), loca_temp);
    checkEOF();
}

void    Parser::parseLocDir(Server & serv_temp)
{
    Location loca_temp;

    if (_current->value[_current->value.length() - 1] != '/')
        _current->value.push_back('/');
    loca_temp.setBaseUri(_current->value);
    loca_temp.setUrl("." + _current->value);
    loca_temp.setIsDirectory(true);
    loca_temp.setIndexes(serv_temp.getIndexes());
    loca_temp.setBodySize(serv_temp.getBodySize());
    loca_temp.setCgi(serv_temp.getCgi());
    advanceAndCheck();
    if (!expect(OpenBrace))
        throw std::invalid_argument(tokenErr("expected \"{\" after location identifier", *peek(-2)));
    while (_current->type != CloseBrace && _current != _tokens.end())
    {
        if (_current->type == OpenBrace || _current->type == Semicolon)
            throw std::invalid_argument(tokenErr("invalid delimiter \"" + _current->value + "\"", *_current));
        else if (_current->type == String || _current->type == Block)
            throw std::invalid_argument(tokenErr("invalid location directive", *_current));
        else if (_current->type == Identifier)
            parseLocaDirective(loca_temp);
    }
    serv_temp.addLocation(loca_temp.getBaseUri(), loca_temp);
    checkEOF();
}

void    Parser::parseLocaDirective(Location & loca_temp)
{
    std::string value = _current->value;
    int flag_cgi = 0;
    if (match("alias") || match("autoindex") || match("max_body_size"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        if (value == "alias")
            parseAlias(loca_temp);
        else if (value == "autoindex")
            parseAutoIndex(loca_temp);
        else if (value == "max_body_size")
            loca_temp.setBodySize(parseBodySize(_current->value));
        advanceAndCheck();
        if (_current->type == String)
            throw std::invalid_argument(tokenErr("too many value for \"" + value + "\" directive", *_current));
    }
    else if (match("index") || match("set_method") || match("cgi"))
    {
        advanceAndCheck();
        if (_current->type != String)
            throw std::invalid_argument(tokenErr("expected string value after directive", *_current));
        if (value == "index")
            loca_temp.clearIndex();
        if (value == "set_method")
            loca_temp.clearMethods();
        if (value == "cgi" && flag_cgi == 0)
        {
            loca_temp.clearCgi();
            flag_cgi++;
        }
        while (_current->type == String)
        {
            if (value == "index")
                parseLocaIndex(loca_temp);
            else if (value == "set_method")
                parseSetMethod(loca_temp);
            else if (value == "cgi")
                parseCgi(loca_temp);
        }
   }
    if (!expect(Semicolon))
        throw std::invalid_argument(tokenErr("expected \";\" at the end of directive", *peek(-1)));
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
    size_t  pos = 0;
    long    test;
    int     i = 0;
    std::string buf;
    std::string temp = value;
    while (i < 4) 
    {
        pos = temp.find('.');
        buf = temp.substr(0, pos);
        if (buf.empty())
            throw std::invalid_argument(tokenErr("invalid IP address", *_current));
        test = strtol(buf.c_str(), NULL, 10);
        if ((errno == ERANGE) | (test < 0) | (test > 255))
            throw std::invalid_argument(tokenErr("invalid IP address", *_current));
        temp.erase(0, pos + 1);
        i++;
    }
    return (value);
}


/*********Parsing Error Pages*********/


static void  checkErrPage(Server & serv_temp, long & err_temp)
{
    errpage_map temp = serv_temp.getErrPages();

    for (errpage_map::const_iterator it = temp.begin(); it != temp.end(); it++)
    {
        std::vector<int> vtemp = (*it).second;
        for (std::vector<int>::const_iterator vit = vtemp.begin(); vit != vtemp.end(); vit++)
        {
            if (*vit == err_temp)
            {
                serv_temp.deleteErrPage((*it).first);
                break ;
            }
        }
    }
}

void    Parser::parseErrorPage(Server & serv_temp)
{
    std::pair<std::string, std::vector<int> > pair_temp;
    std::vector<int>    error_types;
    while (_current->type == String)
    {
        if (peek(1)->type == Semicolon || peek(1)->type == Identifier || peek(1)->type == Block)
        {
            if (_current->value[0] != '/' || _current->value[_current->value.length() - 1] == '/')
                throw std::invalid_argument(tokenErr("expected a file for \"error_page\" directive", *_current));
            pair_temp.first = _current->value;
        }
        else
        {
            if (std::find(_servError.begin(), _servError.end(), _current->value) == _servError.end())
                throw std::invalid_argument(tokenErr("invalid error type for \"error_page\" directive", *_current));
            long    err_temp;
            err_temp = strtol(_current->value.c_str(), NULL, 10);
            checkErrPage(serv_temp, err_temp);
            error_types.push_back(err_temp);
        }
        advanceAndCheck();
    }
    if (pair_temp.first.empty())
        throw std::invalid_argument(tokenErr("expected a file for \"error_page\" directive", *peek(-1)));
    if (error_types.empty())
        throw std::invalid_argument(tokenErr("expected any error type for \"error_page\" directive", *peek(-1)));
    pair_temp.second = error_types;
    serv_temp.addErrorPages(pair_temp);
    return ;
}


/*********Parsing Body Size*********/


size_t    Parser::parseBodySize(std::string & str_size)
{
    size_t  result = 0;

    if (str_size.find_first_not_of("0123456789KkMmGg") != std::string::npos)
        throw std::invalid_argument(tokenErr("invalid value for \"max_body_size\" directive", *_current));
    int n_alpha = 0;
    std::string suffix;
    for (size_t i = 0; i < str_size.length(); i++)
    {
        if (isalpha(str_size[i]))
        {
            n_alpha++;
            if (n_alpha > 1)
                throw std::invalid_argument(tokenErr("invalid value for \"max_body_size\" directive", *_current));
            if (i != str_size.length() - 1)
                throw std::invalid_argument(tokenErr("invalid value for \"max_body_size\" directive", *_current));
            suffix = str_size[i];
        }
    }
    if (suffix.empty())
        result = strtol(str_size.c_str(), NULL, 10);
    else
    {
        size_t base = 0;
        size_t multiplier = 1;
        std::string str_num = str_size.substr(0, str_size.size() - 1);
        base = strtol(str_num.c_str(), NULL, 10);
        if (base > MAX_BODY_SIZE || errno == ERANGE)
            throw std::overflow_error(tokenErr("max body size too large", *_current));
        if (suffix == "K" || suffix == "k")
            multiplier = 1024;
        else if (suffix == "M" || suffix == "m")
            multiplier = 1024 * 1024;
        else if (suffix == "G" || suffix == "g")
            multiplier = 1024 * 1024 * 1024;
        if (base > MAX_BODY_SIZE / multiplier)
            throw std::overflow_error(tokenErr("max body size too large", *_current));
        result = base * multiplier;
    }
    if (result > MAX_BODY_SIZE)
            throw std::overflow_error(tokenErr("max body size too large", *_current));
    return (result);
}


/*********Parsing Alias*********/


void    Parser::parseAlias(Location & loca_temp)
{
    if (_current->value[0] != '/')
        throw std::invalid_argument(tokenErr("invalid uri for alias", *_current));
    std::string alias = _current->value;
    if (alias[alias.length() - 1] != '/')
        alias.push_back('/');
    loca_temp.setAlias(alias);
    if (loca_temp.getIsDirectory())
        loca_temp.setUrl("." + alias);
    else
        loca_temp.setUrl("." + alias + loca_temp.getBaseUri().substr(1));
    return ;
}


/*********Parsing AutoIndex*********/


void    Parser::parseAutoIndex(Location&loca_temp)
{
    if (_current->value == "on")
        loca_temp.setAutoIndex(true);
    else if (_current->value == "off")
        loca_temp.setAutoIndex(false);
    else
        throw std::invalid_argument(tokenErr("invalid value for \"autoindex\" directive", *_current));
    return ;
}


/*********Parsing Set Method*********/


void    Parser::parseSetMethod(Location & loca_temp)
{
    if (_current->value != "GET" && _current->value != "POST" && _current->value != "DELETE")
        throw std::invalid_argument(tokenErr("invalid method for \"set_method\" directive", *_current));
    else
        loca_temp.addMethod(_current->value);
    advanceAndCheck();
    return ;
}


/*********Parsing Location Index*********/


void    Parser::parseLocaIndex(Location & loca_temp)
{
    if (loca_temp.getIsDirectory())
        loca_temp.addIndex(_current->value);
    advanceAndCheck();
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

bool    Parser::isExecutableFile(const std::string & path) 
{
    return (access(path.c_str(), X_OK) == 0);
}
#include "request.hpp"

Request::Request() {

}

std::string Request::get_methode() {
    return _methode;
}

std::string Request::get_url() {
    return _url;
}

std::string Request::get_http_version() {
    return _http_version;
}

std::string Request::get_accept() {
    return _accept;
}

std::string Request::get_accept_language() {
    return _accept_language;
}

std::string Request::get_accept_encoding() {
    return _accept_encoding;
}

std::string Request::get_connection() {
    return _connection;
}

std::string Request::get_host() {
    return _host;
}

std::string Request::get_body() {
    return _body;
}

std::string Request::get_content_type() {
    return _content_type;
}

std::string Request::get_content_length() {
    return _content_length;
}

//check si on a bien \r\n a la fin des lignes de la requete get, return error 1
int Request::check_request_format_get(std::string const &req) {
    std::string end;
    end = req.substr(req.size() - 4, 4);
    if(end != "\r\n\r\n")
        return 1;
    for(std::string::const_iterator it = req.end() - 4; it != req.begin(); it--)
    {
        if(*it == '\n' && *(it - 1) != '\r' )
            return 1;
        if(*it == '\r' && *it == req[0])
            return 1;
        if(*it == '\r' && *(it - 1) && isalnum(*(it - 1)) == 0)
            return 1;
    }
    std::cout << "format get ok" << std::endl;
    return 0;
}

//check si on a bien \r\n a la fin des lignes de la requete post, retunr error 1
int Request::check_request_format_post(std::string const &req) {
    std::string end;
    int end_found = 0;
    end = req.substr(req.size() - 2, 2);
    if(end != "\r\n")
        return 1;
    for(std::string::const_iterator it = req.end() - 2; it != req.begin(); it--)
    {
        if(*it == '\n' && *(it - 1) != '\r' )
            return 1;
        if(*it == '\r' && *it == req[0])
            return 1;

        if(*it == '\r' && *(it - 1) == '\n' && *(it - 2) == '\r' && *(it + 1) == '\n')
        {
            ++end_found;
            if(end_found != 1)
                return 1;
        }
        else if(*it == '\r' && *(it - 1) && isalnum(*(it - 1)) == 0 && *(it - 1) != '\n')
            return 1;
    }
    if(end_found != 1)
        return 1;
    std::cout << "format post ok" << std::endl;
    return 0;
}

int Request::parse_body_form() {
    std::map<std::string, std::string> data;
    std::string key, value;
    int a = 0;
    for(std::string::const_iterator it = _body.begin(); it != _body.end(); it++)
    {
        if(*it == '=')
        {
            a = 1;
            it++;
        }
        if(*it == '&')
        {
            data[key] = value;
            key.clear();
            value.clear();
            it++;
            a = 0;
        }
        if(a == 0)
            key += *it;
        else
            value += *it;
    }
    if (!key.empty())
        data[key] = value;
    std::map<std::string, std::string>::iterator it;
    for (it = data.begin(); it != data.end(); ++it)
        std::cout << it->first << " " << it->second << "\n";
    return 0;
}

int Request::parse_request(std::string const &req) {
    std::string line;
    std::string word;
    std::istringstream buf(req.c_str());
    if(req.compare(0, 3, "GET") == 0 && check_request_format_get(req) == 1)
        return 1;
    if(req.compare(0, 4, "POST") == 0 && check_request_format_post(req) == 1)
        return 1;
    while (std::getline(buf, line) )
    {
        std::string methods[] = {"GET", "POST", "DELETE", "Accept", "Accept-Language", "Accept-Encoding", "Connection", "Host", "Content-Type", "Content-Length"};
        int (Request::*funct[])(std::string const &) = {&Request::set_methode, &Request::set_methode, &Request::set_methode, &Request::set_accept, &Request::set_accept_language, &Request::set_accept_encoding, &Request::set_connection, &Request::set_host, &Request::set_content_type, &Request::set_content_length};
        for (std::string::const_iterator it = line.begin(); it != line.end(); it++)
        {
            if (*it == ' ' || *it == ':')
            {
                for(unsigned int i = 0; i < sizeof(methods) / sizeof(methods[0]); i++)
                {
                    if (word == methods[i])
                    {
                        if ((this->*funct[i])(line) == 1)
                            return 1;
                        break ;
                    }
                    else if(i == sizeof(methods) / sizeof(methods[0]) && word != methods[sizeof(methods) / sizeof(methods[0])])
                        return 1;
                }
                word.clear();
                break;
            }
            word += *it;
        }
        if (word == "\r")
        {
            std::cout << "ici" << std::endl;
            word.clear();
            set_body(req);
        }
    }

    if(_methode == "POST")
    {
        if(_host.empty() == true || _content_length.empty() == true || _content_type.empty() == true || _body.empty() == true)
            return 1;
        for(std::string::const_iterator it = _content_length.begin(); it != _content_length.end() - 1; it++)
        {
            if(!isdigit(*it))
                return 1;
        }
        if(_content_type == "application/x-www-form-urlencoded\r")
        {
            if(parse_body_form() == 1)
                return 1;
        }
        if(_content_type == "text/plain\r")
        {

        }
        else
            return 1;
        // creation du fichier + parsing body
    }
    std::string response;
    response = create_response();
    std::cout << "response : " << response << std::endl;
    // send(client_fd, response.c_str(), response.length(), 0);
    // close(client_fd);
    return 0;
}


int Request::set_methode(std::string const & line)
{
    std::string read;
    for(std::string::const_iterator it = line.begin(); it != line.end(); it++)
    {
        if (*it == ' ' && _methode.empty() )
        {
            if (read == "GET" || read == "POST" || read == "DELETE")
                _methode = read;
            else
            {
                std::cout << "return 1" << std::endl;
                return 1;
            }
            read.clear();
            it++;
        }
    
        else if(*it == ' ' && _url.empty() )
        {
            _url = read;
            read.clear();
            it++;
        }
    
        else if (*it == '\r')
        {
            // std::cout << "read : " << read << std::endl;
            if(read == "HTTP/1.1") 
                _http_version = read;
            else
                return 1;
            read.clear();
        }
        read += *it;
    }
    return 0;
}

int Request::set_accept(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 8; it != line.end(); it++)
        _accept += *it;
    return 0;
}

int Request::set_accept_language(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 17; it != line.end(); it++)
        _accept_language += *it;
    return 0;
}

int Request::set_accept_encoding(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 17; it != line.end(); it++)
        _accept_encoding += *it;
    return 0;
}

int Request::set_connection(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 12; it != line.end(); it++)
        _connection += *it;
    return 0;
}

int Request::set_host(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 6; it != line.end(); it++)
        _host += *it;
    return 0;
}

int Request::set_content_type(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 14; it != line.end(); it++)
        _content_type += *it;
    return 0;
}

int Request::set_content_length(std::string const & line)
{
    for (std::string::const_iterator it = line.begin() + 16; it != line.end(); it++)
        _content_length += *it;
    return 0;
}

int Request::set_body(std::string const & line)
{
    size_t pos = line.find("\r\n\r\n", 0);
    for (std::string::const_iterator it = line.begin() + pos + 4; it != line.end(); it++)
        _body += *it;
    return 0;
}

std::string get_file_type(const std::string& path) {
    size_t dot = path.rfind('.');
    if (dot == std::string::npos) return "application/octet-stream";
    std::string ext = path.substr(dot + 1);
    if (ext == "html") return "text/html";
    if (ext == "css") return "text/css";
    if (ext == "js") return "application/javascript";
    if (ext == "png") return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "ico") return "image/x-icon";
    return "application/octet-stream";
}

std::string Request::create_response() {
    std::string root_path = "./";
    std::ifstream file(_url.c_str(), std::ios::binary);
    if (!file) {
        return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string body = ss.str();

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << get_file_type(_url) << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}
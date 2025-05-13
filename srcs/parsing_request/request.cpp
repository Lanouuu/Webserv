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

std::string Request::get_body() {
    return _body;
}

int Request::parse_request(std::string const &req) {
    std::string line;
    std::string word;
    std::istringstream buf(req.c_str());
    while (std::getline(buf, line) )
    {
        std::string methods[] = {"GET", "POST", "DELETE", "Accept", "Accept-Language", "Accept-Encoding", "Connection"};
        int (Request::*funct[])(std::string const &) = {&Request::set_methode, &Request::set_methode, &Request::set_methode, &Request::set_accept, &Request::set_accept_language, &Request::set_accept_encoding, &Request::set_connection};
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
                    else if(i == 6 && word != methods[6])
                        return 1;
                }
                word.clear();
                break;
            }
            word += *it;
        }
    }

    if(_methode == "POST")
    {
        
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
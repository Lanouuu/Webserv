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
    {
        return 1;
    }
    std::cout << "format post ok" << std::endl;
    return 0;
}

int Request::check_request_format_post_multi(std::string const &req) {
    std::string end;
    int end_found = 0;
    end = req.substr(req.size() - 2, 2);
    if(end != "\r\n")
        return 1;
    for(std::string::const_iterator it = req.begin(); it != req.end(); it++)
    {
        if(*it == '\r' && *(it + 1) != '\n' )
            return 1;
        if(isalnum(*it) == 0 && *it == req[0])
            return 1;
        if(*it == '\r' && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
        {
            ++end_found;
           for (std::string::const_iterator it2 = it + 4; it2 != req.end(); it2++)
           {
                if ((*it2 == '\r' && *(it2 + 1) != '\n') || (*it2 == '\n' && *(it2 - 1) != '\r'))
                    return 1;
           }
            break;
        }
        else if(*it == '\r' && *(it + 1) && isalnum(*(it + 1)) == 0 && *(it + 1) != '\n' && end_found == 0)
            return 1;
    }
    std::cout << "format post ok" << std::endl;
    return 0;
}

//recuperer le body dans une map (file name = blabla)
int Request::parse_body_form() {
    std::map<std::string, std::string> data;
    std::string key, value;
    int a = 0;
    for(std::string::const_iterator it = _body.begin(); it != _body.end(); it++)
    {
        if(*it == '=')
        {
            a = 1;
            continue;
        }
        if(*it == '&')
        {
            data[key] = value;
            key.clear();
            value.clear();
            a = 0;
            continue;
        }
        if(a == 0)
            key += *it;
        else
            value += *it;
    }
    if (!key.empty())
        data[key] = value;
    _body_data = data;
    return 0;
}

void Request::decode_content() {
    std::map<std::string, std::string> code;
    code["+"] = " ";
    code["%20"] = " ";
    code["%21"] = "!";
    code["%22"] = "\"";
    code["%23"] = "#";
    code["%24"] = "$";
    code["%25"] = "%";
    code["%26"] = "&";
    code["%27"] = "'";
    code["%28"] = "(";
    code["%29"] = ")";
    code["%2A"] = "*";
    code["%2B"] = "+";
    code["%2C"] = ",";
    code["%2D"] = "-";
    code["%2E"] = ".";
    code["%2F"] = "/";
    code["%3A"] = ":";
    code["%3B"] = ";";
    code["%3C"] = "<";
    code["%3D"] = "=";
    code["%3E"] = ">";
    code["%3F"] = "?";
    code["%40"] = "@";
    code["%5B"] = "[";
    code["%5C"] = "\\";
    code["%5D"] = "]";
    code["%5E"] = "^";
    code["%5F"] = "_";
    code["%60"] = "`";
    code["%7B"] = "{";
    code["%7C"] = "`";
    code["%7D"] = "}";
    code["%7E"] = "~";

    std::string content = _body_data["Content"];
    for (std::string::iterator it = content.begin(); it != content.end(); it++)
    {
        if (*it == '%')
        {
            std::string hexa_code = content.substr(it - content.begin(), 3);
            std::map<std::string, std::string>::iterator  it_map = code.find(hexa_code);
            if(it_map != _body_data.end())
            {
                content.erase(it - content.begin(), 2);
                content.replace(it - content.begin(), 1, it_map->second);
            }
            else
            {
                std::cout << "Hexacode not found in map" << std::endl;
            }
        }
    }
    _body_data["Content"] = content;
    std::cout << "body content = " << _body_data.find("Content")->second.c_str() << std::endl; 
}

int Request::parse_request(std::string const &req) {
    std::string line;
    std::string word;
    std::istringstream buf(req.c_str());
    if(req.compare(0, 3, "GET") == 0 && check_request_format_get(req) == 1)
        return 1;
    if(req.compare(0, 4, "POST") == 0 && check_request_format_post(req) == 1 && check_request_format_post_multi(req) == 1)
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
            word.clear();
            set_body(req);
            break;
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
            {
                std::cout << "error in parse_body_form" << std::endl;
                return 1;
            }
            std::cout << "nom du fichier a creer : " << _body_data.find("File+name")->second << std::endl;
            std::ofstream new_file;
            std::string filename = _body_data.find("File+name")->second.c_str();
            std::ifstream test_open_file(filename.c_str());
            if(test_open_file.is_open())
            {
                std::cout << "File name already exist, add suffix" << std::endl;
                filename += "_";
                for(int i = 0; i < std::numeric_limits<int>::max(); i++)
                {
                    std::stringstream ss;
                    ss << i;
                    filename += ss.str();
                    if(open(filename.c_str(), O_CREAT | O_EXCL, 0644) == -1)
                    {
                        size_t pos = filename.find_last_of('_');
                        filename.erase(pos + 1, (filename.end() - filename.begin()) - pos);
                    }
                    else
                        break;
                }
            }
            new_file.open(filename.c_str());
            if(!new_file.is_open())
            {
                std::cout << "error creating new file" << std::endl;
                return 1;
            }
            std::cout << "File = " << filename << std::endl;
            decode_content();
            new_file << _body_data.find("Content")->second.c_str();
            new_file.close();
        }
        else if(_content_type == "text/plain\r")
        {
            std::string filename = "upload.txt";
            std::ofstream new_file;
            std::ifstream test_open_file(filename.c_str());
            if(test_open_file.is_open())
            {
                for(int i = 0; i < std::numeric_limits<int>::max(); i++)
                {
                    std::ostringstream oss;
                    oss << "upload_" << i << ".txt";
                    filename = oss.str();
    
                    if (open(filename.c_str(), O_CREAT | O_EXCL | O_RDWR, 0644) == -1)
                        continue;
                    else
                    {
                        break;
                        //std::cerr << "Erreur d'ouverture du fichier\n";
                    }
                }
            }
            new_file.open(filename.c_str(), std::ostream::in | std::ostream::out);
            if(!new_file.is_open())
            {
                std::cout << "error creating new file" << std::endl;
                return 1;
            }
            std::cout << "upload file = " << filename.c_str() << std::endl;
            new_file <<_body;
            new_file.close();
        }
        else if(_content_type.find("multipart/form-data", 0) != std::numeric_limits<size_t>::max())
        {
            std::string content_disposition, name, filename, content_type, line, keyword, content;
            size_t begin, end;
            if (check_request_format_post_multi(req) == 1)
                return 1;
            std::string boundary = "--";
            size_t pos = _content_type.find("boundary=", 0);
            if (pos == std::numeric_limits<size_t>::max())
                return 1;
            boundary += _content_type.substr(pos + 9, _content_type.end() - _content_type.begin() - pos + 9) + '\n';
            std::cout << "boundary = " << boundary << std::endl;
            begin = _body.find(boundary, 0);
            if (begin == std::numeric_limits<size_t>::max())
                return 1;
            end = _body.find(boundary, begin + boundary.size());
            if (end == std::numeric_limits<size_t>::max())
            {
                for(std::string::iterator it = boundary.end() - 1; it != boundary.end(); it++)
                {
                    if (boundary.size() >= 2)
                        boundary.erase(boundary.size() - 2);
                    boundary += "--\r\n";
                    std::cout << "end boudary = " << boundary << std::endl;
                    break;
                }
                end =  _body.find(boundary, begin + boundary.size());
            }
            std::cout << "End = " << end << std::endl;
            line = _body.substr(begin + boundary.size() - 2, end - boundary.size() - 2);
            //upload
            if (line.find("Content-Disposition: form-data; name=\"content\"; filename=", 0) != std::string::npos)
            {
                pos = line.find('\r', 0);
                filename = line.substr(57, pos - 57);
                filename.erase(0, 1);
                filename.erase(filename.size() - 1, 1);
                std::cout << "Filename = " << filename << std::endl;
                std::cout << "END" << std::endl;
            }
            if ((pos = line.find("Content-Type:", 0)) != std::string::npos)
            {
                std::cout << "pos = " << pos << std::endl;
                for(std::string::iterator it = line.begin() + pos + 14; *it != '\r'; it++)
                {
                    content_type += *it;
                }
                std::cout << "Content-type = " << content_type << std::endl;
            }
            pos = line.find("\r\n\r\n", 0);
            content = line.substr(pos + 4, end);
            std::cout << "pos = " << pos << std::endl;
            std::cout << "content = " << content << std::endl;
            std::ofstream new_file;
            std::ifstream test_open_file(filename.c_str());
            if(test_open_file.is_open())
            {
                std::cout << "File name already exist, add suffix" << std::endl;
                filename += "_";
                for(int i = 0; i < std::numeric_limits<int>::max(); i++)
                {
                    std::stringstream ss;
                    ss << i;
                    filename += ss.str();
                    if(open(filename.c_str(), O_CREAT | O_EXCL, 0644) == -1)
                    {
                        size_t pos = filename.find_last_of('_');
                        filename.erase(pos + 1, (filename.end() - filename.begin()) - pos);
                    }
                    else
                        break;
                }
            }
            new_file.open(filename.c_str());
            if(!new_file.is_open())
            {
                std::cout << "error creating new file" << std::endl;
                return 1;
            }
            std::cout << "File = " << filename << std::endl;
            new_file << content;
            new_file.close();
            std::cout << "line = " << line << std::endl;
        }
        else
        {
            std::cout << "ici" << std::endl;
            return 1;
        }
    }
    else if (_methode == "DELETE")
    {
        _url.erase(0, 1);
        std::ifstream file(_url.c_str());

        if(file.is_open())
        {
            std::remove(_url.c_str());

            if (std::ifstream(_url.c_str()))
            {
                std::cout << "Error, file could no be deleted" << std::endl;
            }
        }
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
    {
        _content_type += *it;
    }
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
    //a modifier car \0
    std::cout << "ICI" << std::endl;
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
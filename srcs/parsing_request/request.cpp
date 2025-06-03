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

void Request::get_body() {
    for (size_t i = 0; i < _body.size(); i++)
        std::cout << _body[i];
    std::cout << std::endl;
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
int Request::check_request_format_post() {
    int end_found = 0;
    if(*(_request.end() - 2) != '\n' || *(_request.end() - 3) != '\r')
    {
        return 1;
    }
    for(std::vector<char>::const_iterator it = _request.end() - 2; it != _request.begin(); it--)
    {
        if(*it == '\n' && *(it - 1) != '\r' )
        {
            return 1;
        }
        if(*it == '\r' && *it == _request[0])
        {
            return 1;
        }
        if(*it == '\r' && *(it - 1) == '\n' && *(it - 2) == '\r' && *(it + 1) == '\n')
        {
            ++end_found;
            if(end_found != 1)
            {
                return 1;
            }
        }
        else if(*it == '\r' && *(it - 1) && isalnum(*(it - 1)) == 0 && *(it - 1) != '\n' && end_found == 1)
        {
            return 1;
        }
    }
    if(end_found != 1)
    {
        return 1;
    }
    std::cout << "format post xform ok" << std::endl;
    return 0;
}

int Request::check_request_format_post_multi() {
    int end_found = 0;
    if(*(_request.end() - 2) != '\n' || *(_request.end() - 3) != '\r')
    {
        return 1;
    }
    for(std::vector<char>::const_iterator it = _request.begin(); it != _request.end(); it++)
    {
        if(*it == '\r' && *(it + 1) != '\n' )
        {
            return 1;
        }
        if(isalnum(*it) == 0 && *it == _request[0])
        {
            return 1;
        }
        if(*it == '\r' && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
        {
            ++end_found;
           for (std::vector<char>::const_iterator it2 = it + 4; it2 != _request.end(); it2++)
           {
                if ((*it2 == '\r' && *(it2 + 1) != '\n') || (*it2 == '\n' && *(it2 - 1) != '\r'))
                {
                    return 1;
                }
           }
            break;
        }
        else if(*it == '\r' && *(it + 1) && isalnum(*(it + 1)) == 0 && *(it + 1) != '\n' && end_found == 0)
        {
            return 1;
        }
    }
    std::cout << "format post multi ok" << std::endl;
    return 0;
}

//recuperer le body dans une map (file name = blabla)
int Request::parse_body_form() {
    std::map<std::string, std::string> data;
    std::string key, value;
    int a = 0;
    for(std::vector<char>::const_iterator it = _body.begin(); it != _body.end(); it++)
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
    int succes_code = 0;
    std::istringstream buf(req.c_str());
    if(req.compare(0, 3, "GET") == 0 && check_request_format_get(req) == 1)
        return 400;
    if(req.compare(0, 4, "POST") == 0 && check_request_format_post() == 1 && check_request_format_post_multi() == 1)
        return 400;
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
                            return 400;
                        break ;
                    }
                    else if(i == sizeof(methods) / sizeof(methods[0]) && word != methods[sizeof(methods) / sizeof(methods[0])])
                        return 400;
                }
                word.clear();
                break;
            }
            word += *it;
        }
        if (word == "\r")
        {
            word.clear();
            set_body();
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
                return 400;
        }
        if(_content_type == "application/x-www-form-urlencoded\r")
        {
            if(parse_body_form() == 1 && check_request_format_post() == 1)
            {
                std::cout << "error in parse_body_form" << std::endl;
                return 400;
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
                        if (errno == EEXIST)
                        {
                            size_t pos = filename.find_last_of('_');
                            filename.erase(pos + 1, (filename.end() - filename.begin()) - pos);
                        }
                        return 500;
                    }
                    else
                        break;
                }
            }
            new_file.open(filename.c_str());
            if(!new_file.is_open())
            {
                std::cout << "error creating new file" << std::endl;
                return 500;
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
                    {
                        if (errno == EEXIST)
                            continue;
                        return 500;
                    }
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
                return 500;
            }
            std::cout << "upload file = " << filename.c_str() << std::endl;
            new_file.write(&_body[0], _body.size());
            new_file.close();
        }
        else if(_content_type.find("multipart/form-data", 0) != std::numeric_limits<size_t>::max())
        {
            std::string content_disposition, name, filename, content_type, content;
            std::vector<char> line;
            if (check_request_format_post_multi() == 1)
                return 400;
            std::string boundary = "--";
            size_t pos = _content_type.find("boundary=", 0);
            if (pos == std::numeric_limits<size_t>::max())
            {
                return 400;
            }
            boundary += _content_type.substr(pos + 9, _content_type.end() - _content_type.begin() - pos + 9) + '\n';
            // std::cout << "boundary = " << boundary << std::endl;
            // std::cout << "boundary size = " << boundary.size() << std::endl;
            std::vector<char>::iterator begin = std::search(_body.begin(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            if (begin == _body.end())
                return 400;
            std::vector<char>::iterator end = std::search(begin + boundary.size(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            if (end == _body.end())
            {
                // std::cout << "OK" << std::endl;
                for(std::string::iterator it = boundary.end() - 1; it != boundary.end(); it++)
                {
                    if (boundary.size() >= 2)
                        boundary.erase(boundary.size() - 2);
                    boundary += "--\r\n";
                    // std::cout << "end boudary = " << boundary << std::endl;
                    break;
                }
                end =  std::search(_body.begin(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            }
            // std::cout << "End = " << *end << std::endl;
            size_t boundary_pos = end - _body.begin();
            begin += boundary.size();
            for (;begin != end; begin++)
            {
                line.push_back(*begin);
            }
            for (size_t i = 0; i < line.size(); i++)
                std::cout << line[i];
            std::cout << std::endl;
            //upload
            const char keyword[] = {"Content-Disposition: form-data; name=\"filename\"; filename="};
            begin = std::search(line.begin(), line.end(), keyword, keyword + sizeof(keyword) - 1);
            // std::cout << "begin = " << begin - line.begin() << std::endl;
            if (begin != line.end())
            {
                const char pos[] = {'\r'};
                end = std::search(line.begin(), line.end(), pos, pos + sizeof(pos));
                //std::cout << "end = " << end - line.begin() << std::endl;
                if (end != line.end())
                {
                    for (std::vector<char>::const_iterator it = line.begin() + 58; it != end; it++)
                        filename += *it;
                    filename.erase(0, 1);
                    filename.erase(filename.size() - 1, 1);
                    std::cout << "Filename = " << filename << std::endl;
                   // std::cout << "END" << std::endl;
                }
            }
            else
            {
                std::cout << "Pas ok 0" << std::endl;
                return 400;
            }


            begin = std::search(_body.begin() + boundary_pos, _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            // std::cout << "boudary = " << boundary << std::endl;
            // std::cout << "begin = " << begin - _body.begin() << std::endl;
            end = std::search(begin + boundary.size(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
           // std::cout << "end = " << end - _body.begin() << std::endl;
            if (end != _body.end())
            {
               // std::cout << "OK" << std::endl;
                for(std::string::iterator it = boundary.end() - 1; it != boundary.end(); it++)
                {
                    if (boundary.size() >= 2)
                        boundary.erase(boundary.size() - 2);
                    boundary += "--\r\n";
                   // std::cout << "end boudary = " << boundary << std::endl;
                    break;
                }
                end =  std::search(end + boundary.size(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());     
            }
            line.erase(line.begin(), line.end() - 1);
            for (;begin != end; begin++)
            {
                line.push_back(*begin);
            }
            const char keyword2[] = {"Content-Type:"};
            begin = std::search(line.begin(), line.end(), keyword2, keyword2 + sizeof(keyword2) - 1);
            if (begin != line.end())
            {
               // std::cout << "pos = " << pos << std::endl;
                for(std::vector<char>::const_iterator it = begin + 14; *it != '\r'; it++)
                {
                    content_type += *it;
                }
                std::cout << "Content-type = " << content_type << std::endl;
            }
            else
            {
                std::cout << "Pas ok" << std::endl;
                return 400;
            }
            const char keyword3[] = {'\r', '\n', '\r', '\n'};
            begin = std::search(line.begin(), line.end(), keyword3, keyword3 + sizeof(keyword3));
            if (begin != line.end())
            {
                for (std::vector<char>::const_iterator it = begin + 4; it != line.end() - boundary.size() - 3; it++)
                    content += *it;
                // std::cout << "pos = " << pos << std::endl;
                std::cout << "content = " << content << std::endl;
            }
            else
            {
                std::cout << "Return" << std::endl;
                return 400;
            }
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
                        if (errno == EEXIST)
                        {
                            size_t pos = filename.find_last_of('_');
                            filename.erase(pos + 1, (filename.end() - filename.begin()) - pos);
                        }
                    }
                    else
                        break;
                }
            }
            new_file.open(filename.c_str());
            if(!new_file.is_open())
            {
                std::cout << "error creating new file" << std::endl;
                return 500;
            }
            std::cout << "File = " << filename << std::endl;
            new_file << content;
            new_file.close();
            // std::cout << "line = " << line << std::endl;
        }
        else
        {
            std::cout << "ici" << std::endl;
            return 415;
        }
        succes_code = 201;
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
            succes_code = 204;
        }
        else
            return 404;
    }
    if (_methode == "GET")
        succes_code = 200;
    std::string response;
    response = create_response(succes_code);
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

void    Request::add_request(char buffer[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        _request.push_back(buffer[i]);
    }
    // for(size_t i = 0; i < size; i++)
    // {
    //     std::cout << _request[i];
    // }
    // std::cout << std::endl;
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

int Request::set_body()
{
    //a modifier car \0
    const char word[] = {'\r', '\n', '\r', '\n'};
    std::vector<char>::const_iterator it = std::search(_request.begin(), _request.end(), word, word + 4);
    if (it != _request.end())
    {
        it += 4;
        for (; it != _request.end(); it++)
            _body.push_back(*it);
    }
    else
        return 1;
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

std::string Request::create_response(int succes_code) {
    std::string root_path = "./";
    std::ostringstream response;
    std::ifstream file(_url.c_str(), std::ios::binary);
    if (!file) {
        return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    }
    else
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string body = ss.str();
        if (succes_code == 200)
        {
            
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: " << get_file_type(_url) << "\r\n";
            response << "Content-Length: " << body.size() << "\r\n";
            response << "Connection: keep-alive\r\n";
            response << "\r\n";
            response << body;
        }
        else if (succes_code == 201)
        {           
            response << "HTTP/1.1 201 Created\r\n";
            response << _url << "\r\n";
            response << "Content-Type: " << get_file_type(_url) << "\r\n";
            response << "Content-Length: " << "27" << "\r\n";
            response << "Connection: keep-alive\r\n";
            response << "\r\n";
            response << "File successfully created\r\n";
            
        }
        else if (succes_code == 204)
            response << "HTTP/1.1 204 No Content\r\n";
    }
    return response.str();
}
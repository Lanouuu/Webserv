#include "Client.hpp"
#include <sys/wait.h>
#include <dirent.h>
#include <iterator>

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

std::vector<char> & Request::getRequest()
{
    return _request;
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
    // std::cout << "req ici = " << req << std::endl;
    end = req.substr(req.find("\r\n\r\n"), 4);
    std::cout << "size - 4 = " << req.size() - 4 << std::endl;
    std::cout << "pos = "<< req.find("\r\n\r\n") << std::endl;
    if (req.size() < 4)
    {
        std::cout << "error size" << std::endl;
        return 1;
    }
    // for (size_t i = 0; i < end.size(); ++i)
    //     std::cout << "char[" << i << "] = " << static_cast<int>(end[i]) << std::endl;
    if(end != "\r\n\r\n")
    {
        std::cout << "end = " << end;
        std::cout << "ici" << std::endl;
        return 1;
    }
    for(std::string::const_iterator it = req.end() - req.find("\r\n\r\n"); it != req.begin(); it--)
    {
        if(*it == '\n' && *(it - 1) != '\r' )
            return 1;
        if(*it == '\r' && *it == req[0])
            return 1;
        // if(*it == '\r' && *(it - 1) && isalnum(*(it - 1)) == 0)
        // {
        //     std::cout << "Not alnum" << std::endl;
        //     return 1;
        // }
    }
    std::cout << "format get ok" << std::endl;
    return 0;
}

int Request::getEOF_Pos() {
    int i = _request.size();
    std::cout << "I = " << i << std::endl;

    std::vector<char>::const_iterator it = _request.end();
    while (it != _request.begin()) {
        --it;
        --i;
        if (*it == '\n') {
            break;
        }
    }
    return i;
}

//check si on a bien \r\n a la fin des lignes de la requete post, retunr error 1
int Request::check_request_format_post() {
    int end_found = 0;
    
    int end = getEOF_Pos();
    std::cout << "req end = " << static_cast<int>(_request[end]) << std::endl;
    if(_request[end] != '\n' || _request[end - 1] != '\r')
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
    unsigned long end = getEOF_Pos();
    std::cout << "---------------------------- end = " << end << "--------------------------\n";
    if (end == 0 || end >= _request.size()) {
        std::cout << "error: end out of bounds\n";
        return 1;
    }
    if(_request[end] != '\n' || _request[end - 1] != '\r')
    {
        std::cout << "error ici 0\n";
        return 1;
    }
    for(std::vector<char>::iterator it = _request.begin(); it != _request.end(); it++)
    {
        std::vector<char>::const_iterator next = it + 1;
        if (next == _request.end())
            break;
        if(*it == '\r' && *(it + 1) != '\n' )
        {
            std::cout << "error ici 1\n";
            return 1;
        }
        if(isalnum(*it) == 0 && *it == _request[0])
        {
            std::cout << "error ici 2\n";
            return 1;
        }
        if(std::distance(it, _request.end()) >= 4 && *it == '\r' && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n')
        {
            ++end_found;

            std::vector<char>::const_iterator it2 = it + 4;
            while (it2 != _request.end()) {
                std::vector<char>::const_iterator next2 = it2 + 1;
                if (next2 == _request.end())
                    break;

                if ((*it2 == '\r' && *next2 != '\n'))
                {
                    std::cout << "error ici 3\n";
                    return 1;
                }

                ++it2;
            }
            break;
        }
        else if(*it == '\r' && *(it + 1) && isalnum(*(it + 1)) == 0 && *(it + 1) != '\n' && end_found == 0)
        {
            std::cout << "error ici 4\n";
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

std::string Request::convert_to_string() {
    std::string res;
    for(std::vector<char>::iterator i = _request.begin(); i != _request.end(); i++) {
        res += *i;
    }
    return res;
}

int Request::parse_request(Client & client, Server const & server) {
    std::string line;
    std::string word;
    int succes_code = 0;
    std::string req;
    std::string response;

    req = convert_to_string();
    if(req.compare(0, 3, "GET") == 0 && check_request_format_get(req) == 1)
    {
        response = create_response_html(400, "badreq");
        send(client.getClientFd(), response.c_str(), response.length(), 0);
        close(client.getClientFd());
        return 1;
    }
    if(req.compare(0, 4, "POST") == 0 && check_request_format_post() == 1 && check_request_format_post_multi() == 1)
    {
        std::cout << "-------- bad req upload file --------\n";
        response = create_response_html(400, "badreq");
        send(client.getClientFd(), response.c_str(), response.length(), 0);
        close(client.getClientFd());
        return 1;
    }
    std::istringstream buf(req.c_str());
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
                        {
                            response = create_response_html(400, "badreq");
                            send(client.getClientFd(), response.c_str(), response.length(), 0);
                            close(client.getClientFd());
                            return 1;
                        }
                        break ;
                    }
                    else if(i == sizeof(methods) / sizeof(methods[0]) && word != methods[sizeof(methods) / sizeof(methods[0])])
                        {
                            response = create_response_html(400, "badreq");
                            send(client.getClientFd(), response.c_str(), response.length(), 0);
                            close(client.getClientFd());
                            return 1;
                        }
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
                {
                    response = create_response_html(400, "badreq");
                    send(client.getClientFd(), response.c_str(), response.length(), 0);
                    close(client.getClientFd());
                    return 1;
                }
        }
        if(_content_type == "application/x-www-form-urlencoded\r")
        {
            if(parse_body_form() == 1 && check_request_format_post() == 1)
            {
                response = create_response_html(400, "badreq");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            if(_url == "/delete")
            {
                const char keyword[] = {"file_to_delete="};
                const char end[] = {'&'};
                std::vector<char>::iterator begin_file_name = std::search(_body.begin(), _body.end(), keyword, keyword + sizeof(keyword) - 1);
                std::vector<char>::iterator end_file_name = std::search(_body.begin(), _body.end(), end, end + sizeof(end));
                
                std::string filename_to_delete;
                filename_to_delete.assign(begin_file_name + sizeof(keyword) - 1, end_file_name);
                
                std::cout << "file to delete = " << filename_to_delete << std::endl;
                std::ifstream file(("./upload/" + filename_to_delete).c_str());
                if(file.is_open())
                {
                    std::remove(("./upload/" + filename_to_delete).c_str());
                    if (std::ifstream(("./upload/" + filename_to_delete).c_str()))
                    {
                        response = create_response_html(403, "forbidden");
                        send(client.getClientFd(), response.c_str(), response.length(), 0);
                        close(client.getClientFd());
                        return 1;
                    }
                    std::string response = create_response_html(200, "delete");
                    send(client.getClientFd(), response.c_str(), response.length(), 0);
                    close(client.getClientFd());
                    return 0;
                }
                else
                {
                    response = create_response(404, server);
                    send(client.getClientFd(), response.c_str(), response.length(), 0);
                    close(client.getClientFd());
                    return 1;
                }
                
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
                        response = create_response_html(500, "ise");
                        send(client.getClientFd(), response.c_str(), response.length(), 0);
                        close(client.getClientFd());
                        return 1;
                    }
                    else
                        break;
                }
            }
            new_file.open(filename.c_str());
            if(!new_file.is_open())
            {
                // std::cout << "----------------- error ici ----------------------\n";
                response = create_response_html(500, "ise");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
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
                        response = create_response_html(500, "ise");
                        send(client.getClientFd(), response.c_str(), response.length(), 0);
                        close(client.getClientFd());
                        return 1;
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
                response = create_response_html(500, "ise");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            std::cout << "upload file = " << filename.c_str() << std::endl;
            new_file.write(&_body[0], _body.size());
            new_file.close();
        }
        else if(_content_type.find("multipart/form-data", 0) != std::numeric_limits<size_t>::max())
        {
            std::string content_disposition, name, filename, content_type, content;
            std::vector<char> line;
            std::string boundary = "--";
            size_t pos = _content_type.find("boundary=", 0);
            if (pos == std::numeric_limits<size_t>::max())
            {
                response = create_response_html(400, "badreq");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            boundary += _content_type.substr(pos + 9, _content_type.end() - _content_type.begin() - pos + 9) + '\n';
            std::vector<char>::iterator begin = std::search(_body.begin(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            if (begin == _body.end())
            {
                response = create_response_html(400, "badreq");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            std::vector<char>::iterator end = std::search(begin + boundary.size(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            if (end == _body.end())
            {
                for(std::string::iterator it = boundary.end() - 1; it != boundary.end(); it++)
                {
                    if (boundary.size() >= 2)
                    boundary.erase(boundary.size() - 2);
                    boundary += "--\r\n";
                    break;
                }
                end =  std::search(_body.begin(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            }
            size_t boundary_pos = end - _body.begin();
            begin += boundary.size();
            for (;begin != end; begin++)
            {
                line.push_back(*begin);
            }
            //upload
            const char keyword[] = {"Content-Disposition: form-data; name=\"filename\""};
            begin = std::search(line.begin(), line.end(), keyword, keyword + sizeof(keyword) - 1);
            if (begin != line.end())
            {
                const char pos[] = {'\r'};
                end = std::search(line.begin() + 51, line.end(), pos, pos + sizeof(pos));
                if (end != line.end())
                {
                    for (std::vector<char>::const_iterator it = line.begin() + 51; it != end; it++) 
                    filename += *it;
                }
            }
            else
            {
                response = create_response_html(400, "badreq");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            begin = std::search(_body.begin() + boundary_pos, _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            end = std::search(begin + boundary.size(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
            if (end != _body.end())
            {
                for(std::string::iterator it = boundary.end() - 1; it != boundary.end(); it++)
                {
                    if (boundary.size() >= 2)
                    boundary.erase(boundary.size() - 2);
                    boundary += "--\r\n";
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
                for(std::vector<char>::const_iterator it = begin + 14; *it != '\r'; it++)
                {
                    content_type += *it;
                }
                std::cout << "Content-type = " << content_type << std::endl;
            }
            // else
            // {
            //     std::cout << "Pas ok" << std::endl;
            //     response = create_response_html(400, "badreq");
            //     send(client.getClientFd(), response.c_str(), response.length(), 0);
            //     close(client.getClientFd());
            //     return 1;
            // }
            const char keyword3[] = {'\r', '\n', '\r', '\n'};
            begin = std::search(line.begin(), line.end(), keyword3, keyword3 + sizeof(keyword3));
            std::string delimiter = "\r\n";
            std::vector<char>::iterator body_start = begin + 4; // skip \r\n\r\n
            std::vector<char>::iterator body_end = std::search(body_start,line.end(),delimiter.begin(),delimiter.end());
                    
            if (body_end != line.end()) {
                content.assign(body_start, body_end);
            }
            else
            {
                response = create_response_html(400, "badreq");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            std::ofstream new_file;
            std::ifstream test_open_file(("./upload/" + filename).c_str());
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
            new_file.open(("./upload/" + filename).c_str());
            if(!new_file)
            {
                response = create_response_html(500, "ise");
                send(client.getClientFd(), response.c_str(), response.length(), 0);
                close(client.getClientFd());
                return 1;
            }
            std::cout << "File = " << filename << std::endl;
            new_file << content;
            new_file.close();
        }
        else
        {
            response = create_response_html(415, "umt");
            send(client.getClientFd(), response.c_str(), response.length(), 0);
            close(client.getClientFd());
            return 1;
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
        {
            response = create_response_html(404, "nofound");
            send(client.getClientFd(), response.c_str(), response.length(), 0);
            close(client.getClientFd());
            return 1;
        }
    }
    if (_methode == "GET")
        succes_code = 200;
    response = create_response(succes_code, server);
    std::cout << "response : " << response.c_str() << std::endl;
    int received = send(client.getClientFd(), response.c_str(), response.length(), 0);
    std::cout << "byte send = " << received << std::endl;
    std::cout << "respose lenght = " << response.length() << std::endl;
    close(client.getClientFd());
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
    {
        std::cout << "set body error" << std::endl;
        return 1;
    }
    return 0;
}

std::string Request::get_file_type(const std::string& path) {
    size_t dot = path.rfind('.');
    if (dot == std::string::npos) return "application/octet-stream";
    std::string ext = path.substr(dot + 1);
    if (ext == "html") return "text/html";
    if (ext == "css") return "text/css";
    if (ext == "js") return "application/javascript";
    if (ext == "png") return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "ico") return "image/x-icon";
    if (ext == "txt") return "text/plain";
    if (ext == "sh") return "script";
    return "application/octet-stream";
}

std::string Request::create_response(int succes_code, Server const & server) {
    std::ostringstream response;
    struct stat s;
    std::string temp;
    // std::cout << "url = " << _url << std::endl;
    if (_url == "/")
        _url = "www/index.html";
    else if (_url == "/www/style.css")
        _url = "www/style.css";
    else if(_url == "/favicon.ico")
        _url = "images/icon/favicon.ico";
    else if(_url == "/images/icon/favicon.ico")
        _url = "images/icon/favicon.ico";
    else
    {
        location_map locations = server.getLocaMap();
        location_map::iterator it = locations.find(_url);
        if (it != locations.end())
        {
            std::cout << "location trouver" << std::endl;
            if(stat(_url.c_str(), &s) == 0)
            {
                if( s.st_mode & S_IFDIR )
                {
                    // it's a directory
                    _url = it->second.getUrl() + it->second.getIndexes().front();
                }
                else if( s.st_mode & S_IFREG )
                {
                    // it's a file
                    _url = it->second.getUrl();
                }
            }
        }
        else
        {
            if (_url[0] == '/')
                _url.erase(0, 1);
            // std::cout << "stat url = " << _url << std::endl;
            if(stat(_url.c_str(), &s) == 0)
            {
                if( s.st_mode & S_IFDIR )
                {
                    // it's a directory
                    if (_url[_url.size() - 1] != '/')
                        _url += '/';
                    temp = _url;
                    // std::cout << "_urlRoot = " << _url << std::endl;
                    _url = server.getRoot() + temp + server.getIndexes().front();
                }
                else if( s.st_mode & S_IFREG )
                {
                    // it's a file
                    temp = _url;
                    _url = server.getRoot() + temp;
                }
            }
        }
    }
    if (_url[0] == '/')
        _url.erase(0, 1);
    // std::cout << "url after = " << _url << std::endl;
    std::ifstream file(_url.c_str(), std::ios::binary);
    std::cout << "File = " << _url << std::endl;
    std::ostringstream ss;
    std::string body;
    if (!file)
        return create_response_html(404, "nofound");
    std::stringstream html;
    if (!file &&  s.st_mode & S_IFREG)
        return create_response_html(404, "nofound");
    else if (!file &&  s.st_mode & S_IFDIR)
    {
        _url = temp;
        if (_url[0] == '/')
            _url.erase(0, 1);
        std::cout << "OpenDir url = " << _url << std::endl;
        DIR *dir = opendir(_url.c_str());
        struct dirent *openDir = NULL;
        struct stat currentDir;
        html << "<html><body><h1>Index of " << _url << "</h1><ul>";
        while ((openDir = readdir(dir)) != NULL)
        {
            std::string file(openDir->d_name);
            std::cout << "file = " << file << std::endl;
            std::cout << "dans readdir" << std::endl;
            if (openDir->d_name[0] == '.')
                continue;
            std::cout << "stat file = " << file.c_str() << std::endl;
            stat((_url + file).c_str(), &currentDir);
            if(currentDir.st_mode & S_IFDIR)
            {
                std::cout << "is a dir" << std::endl;
                // it's a directory
                file += '/';
            }
            html << "<li><a href='" << file  << "'>" << file << "</a></li>";
            
        }
        html << "</ul></body></html>";
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html" << "\r\n";
        response << "Content-Length: " << html.str().size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << html.str();
    }
    else if(get_file_type(_url) == "script")
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            response << "error";
            return response.str();
        }

        pid_t pid;
        pid = fork();
        if(pid < 0)
        {
            response << "error";
            return response.str();  
        }
        if(pid == 0)
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            std::string file_name;
            std::string::size_type pos = _url.find_last_of('/');
            if(pos == std::string::npos)    
                file_name = _url.substr(pos + 1);
            else
                file_name = _url;
            char *argv[] = {
                (char *)file_name.c_str(),
                NULL
            };

            // ===> Variables d'environnement CGI
            char *envp[] = {
                (char *)"REQUEST_METHOD=GET",
                (char *)"GATEWAY_INTERFACE=CGI/1.1",
                (char *)"SERVER_PROTOCOL=HTTP/1.1",
                // (char *)"CONTENT_LENGTH=0",
                // (char *)"SCRIPT_NAME=/cgi-bin/script.sh",
                NULL
            };
            // std::cout << "url avant exec : " << _url.c_str() << std::endl;
            execve(_url.c_str(), argv, envp);
            perror("execve");
            exit(1);
        }
        else
        {
            std::cout << "in parent" << std::endl;
            close(pipefd[1]);
            char buffer[4096];
            ssize_t count;
            wait(NULL);
            while((count = read((pipefd[0]), buffer, sizeof(buffer))) > 0)
            {
                // write(client_fd, buffer, count);
                response << buffer;
                // std::cout << "response in sh : " << response.str() << std::endl;
            }    
            close(pipefd[0]);
            return response.str();
        }
    }
    else
    {
        std::cout << "File found " << _url << std::endl;
        ss << file.rdbuf();
        body = ss.str();
        if (succes_code == 200)
        {     
            if (!file &&  s.st_mode & S_IFDIR)
            {
                response << "HTTP/1.1 200 OK\r\n";
                response << "Content-Type: text/html" << "\r\n";
                response << "Content-Length: " << body.size() << "\r\n";
                response << "Connection: keep-alive\r\n";
                response << "\r\n";
                response << html.str();
            }
            else
            {
                response << "HTTP/1.1 200 OK\r\n";
                response << "Content-Type: " << get_file_type(_url) << "\r\n";
                response << "Content-Length: " << body.size() << "\r\n";
                response << "Connection: keep-alive\r\n";
                response << "\r\n";
                response << body;
            }
        }
        else if (succes_code == 201)
        {
            std::ifstream file("./www/codes_pages/201.html", std::ios::binary | std::ios::ate);
            if (!file.is_open())
                return create_response_html(500, "ise");
            std::streamsize ssize = file.tellg();
            file.seekg(0);
            long size = static_cast<long>(ssize);
            size += 27;

            std::ostringstream ss;
            ss << file.rdbuf();
            std::string body = ss.str();
            
            response << "HTTP/1.1 201 Created\r\n";
            response << "Content-Type: " << get_file_type(_url) << "\r\n";
            response << "Content-Length: " << body.size() << "\r\n";
            response << "Connection: keep-alive\r\n";
            response << "\r\n";
            response << ss.str();
        }
        else if (succes_code == 204)
            response << "HTTP/1.1 204 No Content\r\n";
    }
    return response.str();
}

//create response with the html file related (file successfully created for ex)
std::string Request::create_response_html(int succes_code, std::string mode) {
    std::ostringstream response;
    if(succes_code == 200 && mode == "delete")
    {
        std::ifstream file("./www/codes_pages/delete_success.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        std::stringstream ss;
        ss << file.rdbuf();
        std::string body = ss.str();
        
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << ss.str();
        return response.str();
    }
    else if(succes_code == 400 && mode == "badreq")
    {
        std::stringstream ss;
        std::string body;
        std::ifstream file("www/codes_pages/400.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        ss << file.rdbuf();
        body = ss.str();
        response << "HTTP/1.1 400 Bad Request\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }
    else if(succes_code == 403 && mode == "forbidden")
    {
        std::stringstream ss;
        std::string body;
        std::ifstream file("www/codes_pages/403.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        ss << file.rdbuf();
        body = ss.str();
        response << "HTTP/1.1 403 Forbidden\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }
    else if(succes_code == 404 && mode == "nofound")
    {
        std::stringstream ss;
        std::string body;
        std::ifstream file("www/codes_pages/404.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        ss << file.rdbuf();
        body = ss.str();
        response << "HTTP/1.1 404 Not Found\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }
    else if(succes_code == 415 && mode == "umt")
    {
        std::stringstream ss;
        std::string body;
        std::ifstream file("www/codes_pages/415.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        ss << file.rdbuf();
        body = ss.str();
        response << "HTTP/1.1 415 Unsupported Media Type\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }
    else if(succes_code == 500 && mode == "ise")
    {
        std::stringstream ss;
        std::string body;
        std::ifstream file("www/codes_pages/500.html", std::ios::binary);
        if (!file.is_open())
            return create_response_html(500, "ise");
        ss << file.rdbuf();
        body = ss.str();
        response << "HTTP/1.1 500 Internal Server Error\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }
    else
        return 0;
}
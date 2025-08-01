#include "Client.hpp"
#include <sys/wait.h>
#include <dirent.h>
#include "Utils.hpp"

Request::Request() : _reqLocation(NULL)
{

}

Request::~Request(void)
{
    if(_reqLocation)
        delete _reqLocation;
    return ;
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

std::vector<char> const & Request::getRequest() const
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

int Request::set_accept(std::string const & line)
{
    if(_accept.empty())
    {
        for (std::string::const_iterator it = line.begin() + 8; it != line.end(); it++)
        _accept += *it;
    }
    return 0;
}

int Request::set_accept_language(std::string const & line)
{
    if(_accept_language.empty())
    {
        for (std::string::const_iterator it = line.begin() + 17; it != line.end(); it++)
            _accept_language += *it;
    }
    return 0;
}

int Request::set_accept_encoding(std::string const & line)
{
    if(_accept_encoding.empty())
    {
        for (std::string::const_iterator it = line.begin() + 17; it != line.end(); it++)
            _accept_encoding += *it;
    }
    return 0;
}

int Request::set_connection(std::string const & line)
{
    if(_connection.empty())
    {
        for (std::string::const_iterator it = line.begin() + 12; it != line.end(); it++)
            _connection += *it;
    }
    return 0;
}

int Request::set_host(std::string const & line)
{
    if(_host.empty())
    {
        for (std::string::const_iterator it = line.begin() + 6; it != line.end(); it++)
        _host += *it;
    }
    return 0;
}

int Request::set_content_type(std::string const & line)
{
    if(_content_type.empty())
    {
        for (std::string::const_iterator it = line.begin() + 14; it != line.end(); it++)
        {
            _content_type += *it;
        }
    }
    return 0;
}

int Request::set_content_length(std::string const & line)
{
    if(_content_length.empty())
    {
        for (std::string::const_iterator it = line.begin() + 16; it != line.end() - 1; it++)
            _content_length += *it;
    }
    return 0;
}

//pas utilise pour l'instant
void Request::set_content_length_begin(std::vector<char>::const_iterator it)
{
    for (; *it != '\r'; it++)
        _content_length += *it;
}

//check si on a bien \r\n a la fin des lignes de la requete get, return error 1
int Request::check_request_format_get(std::string const &req) {
    std::string end;
    end = req.substr(req.find("\r\n\r\n"), 4);
    if (req.size() < 4)
        return 1;
    if(end != "\r\n\r\n")
        return 1;
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

int Request::getEOF_Pos()
{
    int i = _request.size();
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
    if(_request[end] != '\n' || _request[end - 1] != '\r')
        return 1;
    for(std::vector<char>::const_iterator it = _request.end() - 2; it != _request.begin(); it--)
    {
        if(*it == '\n' && *(it - 1) != '\r' )
            return 1;
        if(*it == '\r' && *it == _request[0])
            return 1;
        if(*it == '\r' && *(it - 1) == '\n' && *(it - 2) == '\r' && *(it + 1) == '\n')
        {
            ++end_found;
            if(end_found != 1)
                return 1;
        }
        else if(*it == '\r' && *(it - 1) && isalnum(*(it - 1)) == 0 && *(it - 1) != '\n' && end_found == 1)
            return 1;
    }
    if(end_found != 1)
        return 1;
    std::cout << "format post xform ok" << std::endl;
    return 0;
}

int Request::check_request_format_post_multi(std::string &boundary) {
    std::cout << std::endl;
    const std::string rnrn = "\r\n\r\n";
    std::string clean_boundary = boundary;
    while (!clean_boundary.empty() && (clean_boundary[clean_boundary.size() - 1] == '\r' || clean_boundary[clean_boundary.size() - 1] == '\n'))
        clean_boundary.erase(clean_boundary.size() - 1);

    std::string end_boundary = clean_boundary + "--";
    std::vector<char>::iterator headers_end = std::search(_request.begin(), _request.end(),rnrn.begin(), rnrn.end());
    if (headers_end == _request.end())
        return 1;
    std::vector<char>::iterator boundary_pos = std::search(headers_end, _request.end(),end_boundary.begin(), end_boundary.end());
    if (boundary_pos == _request.end())
        return 1;
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
    // std::cout << "body content = " << _body_data.find("Content")->second.c_str() << std::endl; 
}

std::string Request::convert_to_string() {
    std::string res;
    for(std::vector<char>::iterator i = _request.begin(); i != _request.end(); i++) {
        res += *i;
    }
    return res;
}

void Request::findLocation(const Server & server, const std::string url)
{
    location_map    map_temp = server.getLocaMap();
    Location        loca_temp;
    std::string     uri_str;

    if (_url[_url.length() - 1] != '/')
    {
        int status;
        status = access(("." + _url).c_str(), F_OK);
    
        if (status == -1)
            _url += "/";
    }
    for (location_map::const_iterator it = map_temp.begin(); it != map_temp.end(); it++)
    {
        size_t pos = url.find((*it).first);
        if (pos == 0)
        {
            if ((*it).first.size() > uri_str.size())
            {
                loca_temp = (*it).second;
                uri_str = (*it).first;
            }

        }
    }
    if (!loca_temp.getBaseUri().empty())
        _reqLocation = new Location(loca_temp);
    return ;   
}

int Request::parse_request(Client & client, Server const & server) {
    std::string line, word, req, response;
    int succes_code = 0;

    req = convert_to_string();
    if(req.compare(0, 3, "GET") == 0 && check_request_format_get(req) == 1)
    {
        std::cout << RED << "ICI 354" << END << std::endl;
        return request_error(server, client, 400, "badreq");
    }
    // if(req.compare(0, 4, "POST") == 0 && check_request_format_post() == 1)
    // {
    //     std::cout << RED << "ICI 359" << END << std::endl;
    //     return request_error(server, client, 400, "badreq");
    // }
    // if(req.compare(0, 4, "POST") == 0 && check_request_format_post() == 1 && check_request_format_post_multi() == 1)
    // {
    //     std::cout << RED << "ICI 359" << END << std::endl;
    //     return request_error(server, client, 400, "badreq");
    // }
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
                            return request_error(server, client, 400, "badreq");
                        break ;
                    }
                    else if(i == sizeof(methods) / sizeof(methods[0]) && word != methods[sizeof(methods) / sizeof(methods[0])])
                        return request_error(server, client, 400, "badreq");
                            
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
    if((size_t)atoi(_content_length.c_str()) > _request.size() && _methode == "POST")
        return 1;
    findLocation(server, _url);
    // -> proteger ??????????????????????????????????????????
    size_t rbody_size = (size_t)atoi(_content_length.c_str());
    if (errno == ERANGE)
        return request_error(server, client, 400, "badreq");
    if (_reqLocation)
    {
        if (rbody_size > _reqLocation->getBodySize())
        {
            request_error(server, client, 400, "badreq");
            return 0;
        }
    }
    else
    {
        if (rbody_size > server.getBodySize())
        {
            request_error(server, client, 400, "badreq");
            return 0;
        }
    }
    int (Request::*methodsTab[])(int &, Client const &, Server const &) = {&Request::get_request_handler, &Request::post_request_handler, &Request::delete_request_handler};
    (this->*methodsTab[get_request_type(_methode)])(succes_code, client, server);
    if (get_file_type(_url) == "script")
        return 0;
    response = create_response(succes_code, server);
    // std::cout << "response : " << response.c_str() << std::endl;
    std::cout << _methode << " " << _url << std::endl;
    int received = send(client.getClientFd(), response.c_str(), response.length(), 0);
    std::cout << "byte send = " << received << std::endl;
    std::cout << "respose lenght = " << response.length() << std::endl;
    return 0;
}

int Request::get_request_handler(int & succes_code, Client const & client, Server const & server)
{
    (void)client;
    succes_code = 200;
    std::string response;
    if(get_file_type(_url) == "script")
    {
        std::ostringstream temp;
        execCgi(server.getCgi(), temp, _url, succes_code, _methode);
        if (succes_code == 404 || succes_code == 500)
        {
            if (succes_code == 404)
                response = status_response_html(server, succes_code, "nofound");
            else if (succes_code == 500)
                response = status_response_html(server, succes_code, "ise");
            send(client.getClientFd(), response.c_str(), response.length(), 0);
            return 1;
        }
        response = temp.str();
        send(client.getClientFd(), response.c_str(), response.length(), 0);
        return (0);
    }
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
                std::cout << RED << "ICI 437" << END << std::endl;
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
        else if (*it == '\r' && _http_version.empty())
        {
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
        _request.push_back(buffer[i]);
}

int Request::set_body()
{
    _body.clear();
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
    if (ext == "py") return "script";
    return "application/octet-stream";
}

std::string Request::create_response(int succes_code, Server const & server) {
    std::ostringstream response;
    struct stat s;
    std::string temp;
    cgi_map     cgi_temp = server.getCgi();
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
            cgi_temp = (*it).second.getCgi();
            if(stat(_url.c_str(), &s) == 0)
            {
                if( s.st_mode & S_IFDIR )
                    _url = it->second.getUrl() + it->second.getIndexes().front();
                else if( s.st_mode & S_IFREG )
                    _url = it->second.getUrl();
            }
        }
        else
        {
            if (_url[0] == '/')
                _url.erase(0, 1);
            if(stat(_url.c_str(), &s) == 0)
            {
                if( s.st_mode & S_IFDIR )
                {
                    if (_url[_url.size() - 1] != '/')
                        _url += '/';
                    temp = _url;
                    _url = server.getRoot() + temp + server.getIndexes().front();
                }
                else if( s.st_mode & S_IFREG )
                {
                    temp = _url;
                    _url = server.getRoot() + temp;
                }
            }
        }
    }
    if (_url[0] == '/')
        _url.erase(0, 1);
    std::ifstream file(_url.c_str(), std::ios::binary);
    std::ostringstream ss;
    std::string body;
    if (!file)
        return status_response_html(server, 404, "nofound");
    std::stringstream html;
    if (!file &&  s.st_mode & S_IFREG)
        return status_response_html(server, 404, "nofound");
    else if (!file &&  s.st_mode & S_IFDIR)
    {
        _url = temp;
        if (_url[0] == '/')
            _url.erase(0, 1);
        DIR *dir = opendir(_url.c_str());
        struct dirent *openDir = NULL;
        struct stat currentDir;
        html << "<html><body><h1>Index of " << _url << "</h1><ul>";
        while ((openDir = readdir(dir)) != NULL)
        {
            std::string file(openDir->d_name);
            if (openDir->d_name[0] == '.')
                continue;
            stat((_url + file).c_str(), &currentDir);
            if(currentDir.st_mode & S_IFDIR)
                file += '/';
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
    else
    {
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
                return status_response_html(server, 500, "ise");
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

int Request::get_request_type(std::string const & method) const
{
    int i = 0;
    std::string methods[] = {"GET", "POST", "DELETE"};

    for (; i < 3; i++)
    {
        if (method == methods[i])
            break ;
    }
    return i;
}

int Request::delete_file(Client const & client, Server const & server)
{
    std::string response;

    const char keyword[] = {"file_to_delete="};
    const char end[] = {'&'};
    std::vector<char>::iterator begin_file_name = std::search(_body.begin(), _body.end(), keyword, keyword + sizeof(keyword) - 1);
    std::vector<char>::iterator end_file_name = std::search(_body.begin(), _body.end(), end, end + sizeof(end));
    
    std::string filename_to_delete;
    filename_to_delete.assign(begin_file_name + sizeof(keyword) - 1, end_file_name);
    
    std::ifstream file(("./www/upload/" + filename_to_delete).c_str());
    if(file.is_open())
    {
        std::remove(("./www/upload/" + filename_to_delete).c_str());
        if (std::ifstream(("./www/upload/" + filename_to_delete).c_str()))
            return request_error(server, client, 403, "forbidden");
        std::string response = status_response_html(server, 200, "delete");
        send(client.getClientFd(), response.c_str(), response.length(), 0);
        return 0;
    }
    return request_error(server, client, 404, "nofound");
}

int Request::urlencoded_handler(Client const & client, Server const & server)
{
    std::string response;
    int succes_code = 0;
    if(parse_body_form() == 1 && check_request_format_post() == 1)
        return request_error(server, client, 400, "badreq");
    if(get_file_type(_url) == "script")
    {
        std::ostringstream temp;
        execCgi(server.getCgi(), temp, _url, succes_code, _methode);
        if (succes_code == 404 || succes_code == 500)
        {
            if (succes_code == 404)
                response = status_response_html(server, succes_code, "nofound");
            else if (succes_code == 500)
                response = status_response_html(server, succes_code, "ise");
            send(client.getClientFd(), response.c_str(), response.length(), 0);
            return 1;
        }
        response = temp.str();
        send(client.getClientFd(), response.c_str(), response.length(), 0);
        return (0);
    }
    if(_url == "/delete")
        return delete_file(client, server);
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
                return request_error(server, client, 500, "ise");
            }
            else
                break;
        }
    }
    new_file.open(filename.c_str());
    if(!new_file.is_open())
        return request_error(server, client, 500, "ise");
    decode_content();
    new_file << _body_data.find("Content")->second.c_str();
    new_file.close();
    return 0;
}

int Request::textPlain_Handler(Client const & client, Server const & server)
{
    std::string filename = "upload.txt";
    std::ofstream new_file;
    std::ifstream test_open_file(filename.c_str());
    std::string response;
    if(parse_body_form() == 1 && check_request_format_post() == 1)
        return request_error(server, client, 400, "badreq");
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
                return request_error(server, client, 500, "ise");
            }
            else
                break; //std::cerr << "Erreur d'ouverture du fichier\n";
        }
    }
    new_file.open(filename.c_str(), std::ostream::in | std::ostream::out);
    if(!new_file.is_open())
        return request_error(server, client, 500, "ise");
    new_file.write(&_body[0], _body.size());
    new_file.close();
    return 0;
}

int Request::request_error(const Server & server, Client const & client, int const error_code, std::string const mode)
{
    std::string response = status_response_html(server, error_code, mode);
    send(client.getClientFd(), response.c_str(), response.length(), 0);
    return 1;
}

int Request::multipart_formData_handler(Client const & client, Server const & server)
{
    // std::vector<char> filename; filename a faire en vector de char ????
    std::string content_disposition, name, filename, content_type, content, response;
    std::string boundary = "--";
    size_t pos = _content_type.find("boundary=", 0);
    if (pos == std::numeric_limits<size_t>::max())
        return request_error(server, client, 400, "badreq");
    boundary += _content_type.substr(pos + 9);
    if(check_request_format_post_multi(boundary) == 1)
        return request_error(server, client, 400, "badreq");
    //1er boundary trouve
    std::vector<char>::iterator begin = std::search(_body.begin(), _body.end(), boundary.c_str(), boundary.c_str() + boundary.size());
    if (begin == _body.end())
        return request_error(server, client, 400, "badreq");
    begin += boundary.size();
    if (std::distance(begin, _body.end()) >= 2 && *begin == '\r' && *(begin + 1) == '\n')
        begin += 2;
    std::string final_boundary = boundary;
    final_boundary.erase(final_boundary.size()-1, 1);
    std::string end_boundary = final_boundary + "--";
    std::cout << "end boundary = [" << end_boundary << "]" << std::endl;
    std::vector<char>::iterator end = std::search(begin, _body.end(),boundary.begin(), boundary.end());
    if (end == _body.end())
    {
        end = std::search(begin, _body.end(), end_boundary.begin(), end_boundary.end());
        if (end == _body.end())
            return request_error(server, client, 400, "badreq");
    }

    std::vector<char> line(begin,end);
    size_t boundary_pos = end - _body.begin();
    std::string keyword;

    if(_url == "/create/")
        keyword = "Content-Disposition: form-data; name=\"filename\"";
    if(_url == "/upload/")
        keyword = "Content-Disposition: form-data; name=\"filename\"; filename=\"";

    begin = std::search(line.begin(), line.end(), keyword.begin(), keyword.end());
    if (begin != line.end() && _url == "/upload/")
    {
        std::vector<char>::iterator filename_start = begin + keyword.size();
        std::vector<char>::iterator filename_end = std::find(filename_start, line.end(), '"');

        if (filename_end != line.end())
            filename.assign(filename_start, filename_end);
        else
            return request_error(server, client, 400, "badreq");
        const char keyword2[] = {"Content-Type:"}; 
        begin = std::search(line.begin(), line.end(), keyword2, keyword2 + sizeof(keyword2) - 1);
        if (begin != line.end())
        {
            for(std::vector<char>::const_iterator it = begin + 14; *it != '\r'; it++)
                content_type += *it;
        }
        else
            return request_error(server, client, 400, "badreq");
        const char header_end_seq[] = {'\r','\n','\r','\n'};
        std::vector<char>::iterator body_start = std::search(begin, line.end(), header_end_seq, header_end_seq + 4);
        if(body_start == line.end())
            return request_error(server, client, 400, "badreq");
        body_start += 4;
        std::vector<char> content_upload(body_start, line.end());
        std::ifstream test_open_file(("./www/upload/" + filename).c_str());
        if(test_open_file.is_open())
        {
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
        std::ofstream new_file;
        new_file.open(("./www/upload/" + filename).c_str(), std::ofstream::out | std::ios::binary);
        if(!new_file)
            return request_error(server, client, 500, "ise");
        if (!content_upload.empty())
            new_file.write(&content_upload[0], content_upload.size());
        new_file.close();
        request_error(server, client, 201, "crok");
        return 0;
    }
    else if (begin != line.end() && _url == "/create/")
    {
        std::vector<char>::iterator filename_start = begin + keyword.size() + 4;
        filename.assign(filename_start, line.end()- 2);
        keyword = "Content-Disposition: form-data; name=\"content\"";
        begin = std::search(_body.begin() + boundary_pos, _body.end(), keyword.begin(), keyword.end());
        if(begin == _body.end())
            return request_error(server, client, 400, "badreq");
        const char end_header[] = {'\r', '\n', '\r', '\n'};
        std::vector<char>::iterator content_start = std::search(begin, _body.end(), end_header, end_header + 4);
        if(content_start == _body.end())
            return request_error(server, client, 400, "badreq");
        content_start += 4;
        std::string boundary_search = boundary;
        size_t end = boundary_search.find_first_of("\r\n"); // delete "\r\n" at the end of boundary
        if (end != std::string::npos)
            boundary_search = boundary_search.substr(0, end);
        std::vector<char>::iterator content_end = std::search(content_start, _body.end(), boundary_search.begin(), boundary_search.end());
        if(content_end == _body.end())
            return request_error(server, client, 400, "badreq");
        std::vector<char>content_create;
        content_create.assign(content_start, content_end -2);

        std::ifstream test_open_file(("./www/upload/" + filename).c_str());
        if(test_open_file.is_open())
        {
            std::cout << "File name already exist, add suffix" << std::endl;
            filename += "_";
            for(int i = 0; i < std::numeric_limits<int>::max(); i++)
            {
                std::stringstream ss;
                ss << i;
                filename += ss.str();
                if(open(("./www/upload/" + filename).c_str(), O_CREAT | O_EXCL, 0644) == -1)
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
        std::ofstream new_file;
        new_file.open(("./www/upload/" + filename).c_str(), std::ofstream::out | std::ios::binary);
        if(!new_file)
            return request_error(server, client, 500, "ise");
        new_file.write(&content_create[0], content_create.size());
        new_file.close();
        request_error(server, client, 201, "crok");
        return 0;
    }
    else
    {
        //quel erreur ?
        // {
        //     response = status_response_html(server, 400, "badreq");
        //     send(client.getClientFd(), response.c_str(), response.length(), 0);
        //     return 1;
        // }
        request_error(server, client, 415, "umt");
        return 1;
    }
}


int  Request::post_request_handler(int & success_code, Client const & client, Server const & server)
{
    if(_host.empty() == true || _content_length.empty() == true || _content_type.empty() == true || (_body.empty() == true && _url == "/upload"))
        return 1;
    std::cout << "_content length = [" << _content_length << "]" << std::endl;
    for(std::string::const_iterator it = _content_length.begin(); it != _content_length.end() - 1; it++)
    {
        if(!isdigit(*it))
            return request_error(server, client, 400, "badreq");
    }

    int i = 0;
    std::string types[] = {"application/x-www-form-urlencoded\r", "text/plain\r", "multipart/form-data"};
    int (Request::*PostRequestsHandler[])(Client const &, Server const &) = {&Request::urlencoded_handler, &Request::textPlain_Handler, &Request::multipart_formData_handler};
    for (; i < 3; i++)
    {
        if (_content_type.find(types[i], 0) != std::numeric_limits<size_t>::max())
            break ;
    }
    if (i < 3)
    {
        int code = (this->*PostRequestsHandler[i])(client, server);
        if (code != 0)
            return code;
    }
    else
        return 415;
    success_code = 201;
    return 0;
}

int Request::delete_request_handler(int & success_code, Client const & client, Server const & server)
{
    _url.erase(0, 1);
    std::ifstream file(_url.c_str());

    if(file.is_open())
    {
        std::remove(_url.c_str());
        if (std::ifstream(_url.c_str()))
            std::cout << "Error, file could no be deleted" << std::endl;
        success_code = 204;
    }
    else
        return request_error(server, client, 404, "nofound");
    return 0;
}

std::string Request::searchErrPages(const errpage_map & err_pages, int code)
{
    int         stop = 0;
    std::string custom_page;

    for (errpage_map::const_iterator errit = err_pages.begin(); errit != err_pages.end(); errit++)
    {
        std::vector<int> temp = (*errit).second;
        for (std::vector<int>::const_iterator vit = temp.begin(); vit != temp.end(); vit++)
        {
            if ((*vit) == code)
            {
                custom_page = (*errit).first;
                stop = 1;
                break ;
            }
        }
        if (stop)
            break ;
    }
    return (custom_page);
}

//create response with the html file related (file successfully created for ex)
std::string Request::status_response_html(const Server & server, int succes_code, std::string mode)
{
    std::string (Request::*funct[])(const Server &) = {&Request::error_code_200, &Request::error_code_201, &Request::error_code_400, &Request::error_code_403, &Request::error_code_404, &Request::error_code_415, &Request::error_code_500};
    std::ostringstream  response;
    std::string         custom_page;
    std::stringstream   ss;
    
    custom_page = searchErrPages(server.getErrPages(), succes_code);
    if (!custom_page.empty())
    {
        custom_page = '.' + custom_page;
        std::ifstream file(custom_page.c_str(), std::ios::binary);
        if (!file.is_open())
            return status_response_html(server, 500, "ise");
        ss << file.rdbuf();
        response << "HTTP/1.1 " << succes_code <<  " Ta requete est pourrie\r\n";
        response << "Content-Type: " << "text/html" << "\r\n";
        response << "Content-Length: " << ss.str().size() << "\r\n";
        response << "Connection: keep-alive\r\n";
        response << "\r\n";
        response << ss.str();
        return (response.str());
    }
    switch (succes_code)
    {
        case 200:
            if (mode == "delete")
                return (this->*funct[0])(server);
        case 201:
            if (mode == "crok")
                return (this->*funct[1])(server);
        case 400:
            if (mode == "badreq")
                return (this->*funct[2])(server);
        case 403:
            if (mode == "forbidden")
                return (this->*funct[3])(server);
        case 404:
            if (mode == "nofound")
                return (this->*funct[4])(server);
        case 415:
            if (mode == "umt")
                return (this->*funct[5])(server);
        case 500:
            if (mode == "ise")
                return (this->*funct[6])(server);
        default:
            return 0;
    }
}
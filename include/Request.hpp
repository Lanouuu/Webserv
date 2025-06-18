#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits>
#include <vector>
#include <algorithm>
#include "Server.hpp"

class Client;

class Request {
    private:
        std::string _methode;
        std::string _url;
        std::string _http_version;
        std::string _accept;
        std::string _accept_language;
        std::string _accept_encoding;
        std::string _connection;
        std::string _host;
        std::vector<char> _body;
        std::string _content_type;
        std::string _content_length;
        std::map<std::string, std::string> _body_data;
        std::vector<char> _request;

    public:
        Request();
        std::string get_methode();
        std::string get_url();
        std::string get_http_version();
        std::string get_accept();
        std::string get_accept_language();
        std::string get_accept_encoding();
        std::string get_connection();
        std::string get_host();
        void        get_body();
        std::string get_content_type();
        std::string get_content_length();
        std::vector<char> & getRequest();

        int set_methode(std::string const & line);
        int set_accept(std::string const & line);
        int set_accept_language(std::string const & line);
        int set_accept_encoding(std::string const & line);
        int set_connection(std::string const & line);
        int set_host(std::string const & line);
        int set_content_type(std::string const & line);
        int set_content_length(std::string const & line);
        int set_body();
        int parse_request(Client & client, Server const & server);
        int check_request_format_get(std::string const &req);
        int check_request_format_post();
        int check_request_format_post_multi();
        int parse_body_form();
        void    add_request(char buffer[], size_t size);
        void    decode_content();
        std::string convert_to_string();

        std::string create_response(int succes_code, Server const & server);
};

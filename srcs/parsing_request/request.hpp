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
        std::string _body;
        std::string _content_type;
        std::string _content_length;
        std::map<std::string, std::string> _body_data;

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
        std::string get_body();
        std::string get_content_type();
        std::string get_content_length();

        int set_methode(std::string const & line);
        int set_accept(std::string const & line);
        int set_accept_language(std::string const & line);
        int set_accept_encoding(std::string const & line);
        int set_connection(std::string const & line);
        int set_host(std::string const & line);
        int set_content_type(std::string const & line);
        int set_content_length(std::string const & line);
        int set_body(std::string const & line);
        int parse_request(std::string const &req);
        int check_request_format_get(std::string const &req);
        int check_request_format_post(std::string const &req);
        int check_request_format_post_multi(std::string const &req);
        int parse_body_form();
        void    decode_content();

        std::string create_response();
};

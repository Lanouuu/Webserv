#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

class Request {
    private:
        std::string _methode;
        std::string _url;
        std::string _http_version;
        std::string _accept;
        std::string _accept_language;
        std::string _accept_encoding;
        std::string _connection;
        std::string _body;

    public:
        Request();
        std::string get_methode();
        std::string get_url();
        std::string get_http_version();
        std::string get_accept();
        std::string get_accept_language();
        std::string get_accept_encoding();
        std::string get_connection();
        std::string get_body();

        int set_methode(std::string const & line);
        int set_accept(std::string const & line);
        int set_accept_language(std::string const & line);
        int set_accept_encoding(std::string const & line);
        int set_connection(std::string const & line);
        int parse_request(std::string const &req);

        std::string create_response();
};

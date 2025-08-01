#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <cstring>
# include <map>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits>
# include <vector>
# include <algorithm>
# include "Server.hpp"
# include <dirent.h>
# include "cgi.h"

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
        Location*                           _reqLocation;

        void                                findLocation(const Server & server, const std::string url);

    public:
        Request(void);
        ~Request(void);

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
        std::vector<char> const & getRequest() const;

        int set_methode(std::string const & line);
        int set_accept(std::string const & line);
        int set_accept_language(std::string const & line);
        int set_accept_encoding(std::string const & line);
        int set_connection(std::string const & line);
        int set_host(std::string const & line);
        int set_content_type(std::string const & line);
        int set_content_length(std::string const & line);
        void set_content_length_begin(std::vector<char>::const_iterator it);
        int set_body();
        int parse_request(Client & client, Server const & server);
        int check_request_format_get(std::string const &req);
        int check_request_format_post();
        int check_request_format_post_multi(std::string &boundary);
        int get_request_handler(int & succes_code, Client const & client, Server const & server);
        int post_request_handler(int & success_code, Client const & client, Server const & server);
        int delete_request_handler(int & success_code, Client const & client, Server const & server);
        int get_request_type(std::string const & method) const;
        int parse_body_form();
        void    add_request(char buffer[], size_t size);
        void    decode_content();
        int urlencoded_handler(Client const & client, Server const & server);
        int textPlain_Handler(Client const & client, Server const & server);
        int multipart_formData_handler(Client const & client, Server const & server);
        int getEOF_Pos();
        int delete_file(Client const & client, Server const & server);
        int request_error(const Server & server, Client const & client, int const error_code, std::string const mode);
        std::string convert_to_string();

        std::string create_response(int succes_code, Server const & server);
        std::string status_response_html(const Server & server, int succes_code, std::string mode);
        std::string searchErrPages(const errpage_map & err_pages, int code);
        std::string get_file_type(const std::string& path);

        std::string error_code_200(const Server & server);
        std::string error_code_201(const Server & server);
        std::string error_code_400(const Server & server);
        std::string error_code_403(const Server & server);
        std::string error_code_404(const Server & server);
        std::string error_code_415(const Server & server);
        std::string error_code_500(const Server & server);
};

#endif
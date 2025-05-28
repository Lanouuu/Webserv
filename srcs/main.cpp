#include <iostream>
#include <string>
#include "parsing_request/request.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    (void)av;
    (void)ac;
    
    char request_test[] = "GET Makefile HTTP/1.1\r\n"
     "Host: localhost:8080\r\n"
     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
     "Accept-Language: en-US,en;q=0.5\r\n"
     "Accept-Encoding: gzip, deflate, br, zstd\r\n"
     "Content-Type: multipart/form-data; boundary=Test\r\n"
     "Content-Length: 42\r\n"
     "Connection: keep-alive\r\n"
     "\r\n";
    //  "--Test\r\n"
    //  "Content-Disposition: form-data; name=\"filename\"; filename=\"ex01.cpp\"\r\n"
    //  "\r\n"
    //  "gferwgrega\r\n"
    //  "--Test\r\n"
    //  "Content-Disposition: form-data; name=\"content\"; filename=\"ex01.cpp\"\r\n"
    //  "Content-Type: text/x-c++src\r\n"
    //  "\r\n"
    //  "class Aw\0esome\r\n"
    //  "{\r\n"
    //  "public:\r\n"
    //  "Awesome( void ) : _n( 42 ) { return; }\r\n"
    //  "int get( void ) const {\0 return this->_n; }\r\n"
    //  "--Test--\r\n";

    //body
    //  "This is the preamble.  It is to be ignored, though it "
    //  "is a handy place for mail composers to include an "
    //  "explanatory note to non-MIME compliant readers.\r\n"
    //  "--Test\r\n"
    //  "\r\n"
    //  "This is implicitly typed plain ASCII text.\r\n"
    //  "It does NOT end with a linebreak.\r\n"
    //  "--Test\r\n"
    //  "Content-type: text/plain; charset=us-ascii\r\n"
    //  "\r\n"
    //  "This is explicitly typed plain ASCII text.\r\n"
    //  "It DOES end with a linebreak.\r\n"
    //  "\r\n"
    //  "--Test--\r\n"
    //  "This is the epilogue. It is also to be ignored\r\n"
    // for(size_t i = 0; i < sizeof(request_test); i++)
    // {
    //     std::cout << request_test[i];
    // }
    // std::cout << std::endl;
    // memset(&line, 0, sizeof(line));
    // size_t readed = read(0, &line, 300);
    // if (readed <= 0)
    //     return 0;
    int exit_code = 0;
    try
    {
        Request request;
        size_t size = sizeof(request_test);
        request.add_request(request_test, size);
        if((exit_code = request.parse_request(request_test)) != 0)
            std::cout << "parsing error" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    // std::cout << request.get_methode() << request.get_url() << request.get_http_version() << std::endl;
    // std::cout << "host : " << request.get_host() << std::endl;
    // std::cout << "accept : " << request.get_accept() << std::endl;
    // std::cout << "accept_l : " << request.get_accept_language() << std::endl;
    // std::cout << "accept_e : " << request.get_accept_encoding() << std::endl;
    // std::cout << "Content type : " << request.get_content_type() << std::endl;
    // std::cout << "Content length : " << request.get_content_length() << std::endl;
    // std::cout << "connection : " << request.get_connection() << std::endl;
    // request.get_body();

    return (exit_code);
}
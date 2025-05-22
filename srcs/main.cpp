#include <iostream>
#include <string>
#include "parsing_request/request.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    (void)av;
    (void)ac;

    std::string request_test = "DELETE /ex01 HTTP/1.1\r\n";
    // request_test += "Host: localhost:8080\r\n";
    // request_test += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
    // request_test += "Accept-Language: en-US,en;q=0.5\r\n";
    // request_test += "Accept-Encoding: gzip, deflate, br, zstd\r\n";
    // request_test += "Content-Type: multipart/form-data; boundary=Test\r\n";
    // request_test += "Content-Length: 42\r\n";
    // request_test += "Connection: keep-alive\r\n";
    request_test += "\r\n";

    //body
    // request_test += "This is the preamble.  It is to be ignored, though it ";
    // request_test += "is a handy place for mail composers to include an ";
    // request_test += "explanatory note to non-MIME compliant readers.\r\n"; 
    // request_test += "--Test\r\n"; 
    // request_test += "\r\n";
    // request_test += "This is implicitly typed plain ASCII text.\r\n"; 
    // request_test += "It does NOT end with a linebreak.\r\n"; 
    // request_test += "--Test\r\n";
    // request_test += "Content-type: text/plain; charset=us-ascii\r\n"; 
    // request_test += "\r\n";
    // request_test += "This is explicitly typed plain ASCII text.\r\n";
    // request_test += "It DOES end with a linebreak.\r\n";
    // request_test += "\r\n";
    // request_test += "--Test--\r\n";
    // request_test += "This is the epilogue. It is also to be ignored\r\n";

    // request_test += "--Test\r\n";
    // request_test += "Content-Disposition: form-data; name=\"filename\"; filename=\"ex01.cpp\"\r\n";
    // request_test += "\r\n";
    // request_test += "gferwgrega\r\n";
    request_test += "--Test\r\n";
    request_test += "Content-Disposition: form-data; name=\"content\"; filename=\"ex01.cpp\"\r\n";
    request_test += "Content-Type: text/x-c++src\r\n";
    request_test += "\r\n";
    request_test += "class Awesome\r\n";
    request_test += "{\r\n";
    request_test += "public:\r\n";
    request_test += "Awesome( void ) : _n( 42 ) { return; }\r\n";
    request_test += "int get( void ) const { return this->_n; }\r\n";
    request_test += "--Test--\r\n";

    Request request;
    if(request.parse_request(request_test) == 1)
        std::cout << "parsing error" << std::endl;

    std::cout << request.get_methode() << request.get_url() << request.get_http_version() << std::endl;
    std::cout << "host : " << request.get_host() << std::endl;
    std::cout << "accept : " << request.get_accept() << std::endl;
    std::cout << "accept_l : " << request.get_accept_language() << std::endl;
    std::cout << "accept_e : " << request.get_accept_encoding() << std::endl;
    std::cout << "Content type : " << request.get_content_type() << std::endl;
    std::cout << "Content length : " << request.get_content_length() << std::endl;
    std::cout << "connection : " << request.get_connection() << std::endl;
    std::cout << "body : " << request.get_body() << std::endl;

    return (0);
}
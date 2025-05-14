#include <iostream>
#include <string>
#include "parsing_request/request.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    (void)av;
    (void)ac;

    std::string request_test = "POST srcs/images/must.png HTTP/1.1\r\n";
    request_test += "Host: localhost:8080\r\n";
    request_test += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
    request_test += "Accept-Language: en-US,en;q=0.5\r\n";
    request_test += "Accept-Encoding: gzip, deflate, br, zstd\r\n";
    request_test += "Content-Type: application/x-www-form-urlencoded\r\n";
    request_test += "Content-Length: 42\r\n";
    request_test += "Connection: keep-alive\r\n";
    request_test += "\r\n";

    //body
    request_test += "File+name=gfjnmfhynmjgy&Content=jnmyhtgdjm&date=22&heure=12\r\n";

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
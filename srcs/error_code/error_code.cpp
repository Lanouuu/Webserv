#include "Request.hpp"

std::string Request::error_code_200(const Server & server)
{
    std::ostringstream response;
    std::ifstream file("./www/codes_pages/delete_success.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");
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

std::string Request::error_code_201(const Server & server)
{
    std::ostringstream response;
    std::ifstream file("./www/codes_pages/201.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");

    std::stringstream ss;
    ss << file.rdbuf();
    std::string body = ss.str();
    
    response << "HTTP/1.1 201 Created\r\n";
    response << "Content-Type: text/html" << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: keep-alive\r\n";
    response << "\r\n";
    response << ss.str();
    return response.str();
}

std::string Request::error_code_400(const Server & server)
{
    std::ostringstream response;
    std::stringstream ss;
    std::string body;
    std::ifstream file("www/codes_pages/400.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");
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

std::string Request::error_code_403(const Server & server)
{
    std::ostringstream response;
    std::stringstream ss;
    std::string body;
    std::ifstream file("www/codes_pages/403.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");
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

std::string Request::error_code_404(const Server & server)
{
    std::ostringstream response;
    std::stringstream ss;
    std::string body;
    std::ifstream file("www/codes_pages/404.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");
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

std::string Request::error_code_415(const Server & server)
{
    std::ostringstream response;
    std::stringstream ss;
    std::string body;
    std::ifstream file("www/codes_pages/415.html", std::ios::binary);
    if (!file.is_open())
        return status_response_html(server, 500, "ise");
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

std::string Request::error_code_500(const Server & server)
{
    (void)server;
    std::ostringstream response;
    std::stringstream ss;
    std::ifstream file("www/codes_pages/500.html", std::ios::binary);
    if (!file.is_open())
        ss << "Error 500";
    else
        ss << file.rdbuf();
    response << "HTTP/1.1 500 Internal Server Error\r\n";
    response << "Content-Type: " << "text/html" << "\r\n";
    response << "Content-Length: " << ss.str().size() << "\r\n";
    response << "Connection: keep-alive\r\n";
    response << "\r\n";
    response << ss.str();
    return response.str();
}
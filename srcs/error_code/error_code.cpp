#include "Error_code.hpp"

std::string error_code_200(void)
{
    std::ostringstream response;
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

std::string error_code_201(void)
{
    std::ostringstream response;
    std::ifstream file("./www/codes_pages/201.html", std::ios::binary);
    if (!file.is_open())
        return create_response_html(500, "ise");

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

std::string error_code_400(void)
{
    std::ostringstream response;
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

std::string error_code_403(void)
{
    std::ostringstream response;
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

std::string error_code_404(void)
{
    std::ostringstream response;
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

std::string error_code_415(void)
{
    std::ostringstream response;
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

std::string error_code_500(void)
{
    std::ostringstream response;
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
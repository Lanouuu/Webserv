#include "cgi.h"

static std::string    getExtension(const std::string & _url)
{
    std::string file_name;
    std::string::size_type pos = _url.find_last_of('/');
    file_name = (pos == std::string::npos) ? _url : _url.substr(pos + 1);
    std::string extension = file_name.substr(file_name.find('.'));
    return (extension);
}

static std::string  getBin(const cgi_map & cgi, const std::string & extension)
{
    std::string bin;
    for (cgi_map::const_iterator it = cgi.begin(); it != cgi.end(); it++)
    {
        if ((*it).first == extension)
        {
            bin = (*it).second;
            break ;
        }
    }
    return (bin);
}

static void childRoutine(const cgi_map & cgi, int pipefd[2], const std::string & _url, const std::string & method)
{
    std::string extension = getExtension(_url);
    std::string bin = getBin(cgi, extension);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    std::string final_url;
    if (method == "POST")
        final_url = "." +_url;
    else
        final_url = _url;
    char *argv[] = {
        (char *)bin.c_str(),
        (char *)final_url.c_str(),
        NULL
    };

    char *envget[] = {
        (char *)("REQUEST_METHOD=GET"),
        (char *)("GATEWAY_INTERFACE=CGI/1.1"),
        (char *)("SERVER_PROTOCOL=HTTP/1.1"),
        NULL,
    };
    char *envpost[] = {
        (char *)("REQUEST_METHOD=POST"),
        (char *)("GATEWAY_INTERFACE=CGI/1.1"),
        (char *)("SERVER_PROTOCOL=HTTP/1.1"),
        NULL,
    };
    if (method == "GET")
        execve(bin.c_str(), argv, envget);
    else if (method == "POST")
        execve(bin.c_str(), argv, envpost);
    std::cerr << RED << "Webserv: execve: " << END << strerror(errno) << std::endl;
    exit(1);
}

static void parentRoutine(int pipefd[2], std::ostringstream & response, int & succes_code, const std::string & method)
{
    close(pipefd[1]);
    char buffer[4096];
    ssize_t count;
    int status;
    int exit_code = 0;
    wait(&status);
    if (WIFEXITED(status))
        exit_code = WEXITSTATUS(status);
    if (exit_code == 1)
    {
        close(pipefd[0]);
        succes_code = 500;
        return ;
    }
    if (method == "POST")
    {
        std::string cgi_output;
        while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
            cgi_output.append(buffer, count);
        close(pipefd[0]);
        if (cgi_output.find("404 Not Found") != std::string::npos)
        {
            succes_code = 404;
            return ;
        }
        std::string::size_type header_end = cgi_output.find("\r\n\r\n");
        if (header_end == std::string::npos)
            header_end = cgi_output.find("\n\n");
        if (header_end != std::string::npos) 
        {
            std::string headers = cgi_output.substr(0, header_end);
            std::string body = cgi_output.substr(header_end + 4);
            response << "HTTP/1.1 200 OK\r\n";
            response << headers << "\r\n\r\n";
            response << body;
        } 
        else 
            response << cgi_output;
    }
    else if (method == "GET")
    {
        while((count = read((pipefd[0]), buffer, sizeof(buffer))) > 0)
            response.write(buffer, count);
        close(pipefd[0]);
    }
    return ;
}


void execCgi(const cgi_map & cgi, std::ostringstream & response, const std::string & _url, int & succes_code, const std::string & method)
{
    if (cgi.empty())
    {
        succes_code = 500;
        std::cerr << RED << "Webserv: " << END << "No rules for cgi" << std::endl;
        return ;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        succes_code = 500;
        std::cerr << RED << "Webserv: pipe: " << END << strerror(errno) << std::endl;
        return ;
    }
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        succes_code = 500;
        std::cerr << RED << "Webserv: fork: " << END << strerror(errno) << std::endl;
        return ;
    }
    if(pid == 0)
        childRoutine(cgi, pipefd, _url, method);
    else
        parentRoutine(pipefd, response, succes_code, method);
    return ;
}
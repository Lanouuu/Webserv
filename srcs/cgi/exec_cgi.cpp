#include "cgi.h"

static std::string    getExtension(std::string & _url)
{
    std::string file_name;
    std::string::size_type pos = _url.find_last_of('/');
    file_name = (pos == std::string::npos) ? _url : _url.substr(pos + 1);
    std::string extension = file_name.substr(file_name.find('.'));
    return (extension);
}

static std::string  getBin(const cgi_map & cgi, std::string & extension)
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

static void childRoutine(const cgi_map & cgi, int pipefd[2], std::string & _url)
{
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    std::string extension = getExtension(_url);
    std::string bin = getBin(cgi, extension);
    char *envp[] = {
        (char *)"REQUEST_METHOD=GET",
        (char *)"GATEWAY_INTERFACE=CGI/1.1",
        (char *)"SERVER_PROTOCOL=HTTP/1.1",
        // (char *)"CONTENT_LENGTH=0",
        // (char *)"SCRIPT_NAME=/cgi-bin/script.sh",
        NULL
    };
    char *argv[] = {
        (char *)bin.c_str(),
        (char *)_url.c_str(),
        NULL
    };
    execve(bin.c_str(), argv, envp);
    std::cerr << "Webserv: execve: " << strerror(errno);
    exit(1);
}

static void parentRoutine(int pipefd[2], std::ostringstream & response)
{
    close(pipefd[1]);
    char buffer[4096];
    ssize_t count;
    wait(NULL);
    while((count = read((pipefd[0]), buffer, sizeof(buffer))) > 0)
        response.write(buffer, count);
    close(pipefd[0]);
    return ;
}


void execCgi(const cgi_map & cgi, std::ostringstream & response, std::string & _url)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        response << "error";
        return;
    }
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        response << "error";
        return;
    }
    if(pid == 0)
        childRoutine(cgi, pipefd, _url);
    else
        parentRoutine(pipefd, response);
    return ;
}
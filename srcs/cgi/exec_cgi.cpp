#include "cgi.h"

static void childRoutine(Server const & server, int pipefd[2], std::string & _url)
{
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    std::string file_name;
    std::string::size_type pos = _url.find_last_of('/');
    if(pos == std::string::npos)    
        file_name = _url.substr(pos + 1);
    else
        file_name = _url;
    char *argv[] = {
        (char *)file_name.c_str(),
        NULL
    };
    // ===> Variables d'environnement CGI
    char *envp[] = {
        (char *)"REQUEST_METHOD=GET",
        (char *)"GATEWAY_INTERFACE=CGI/1.1",
        (char *)"SERVER_PROTOCOL=HTTP/1.1",
        // (char *)"CONTENT_LENGTH=0",
        // (char *)"SCRIPT_NAME=/cgi-bin/script.sh",
        NULL
    };
    // std::cout << "url avant exec : " << _url.c_str() << std::endl;
    execve(_url.c_str(), argv, envp);
    std::cerr << "Webserv: execve: " << strerror(errno);
    exit(1);
}


void execCgi(Server const & server, std::ostringstream & response, std::string & _url)
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
        childRoutine(server, pipefd, _url);
    else
    {
        std::cout << "in parent" << std::endl;
        close(pipefd[1]);
        char buffer[4096];
        ssize_t count;
        wait(NULL);
        while((count = read((pipefd[0]), buffer, sizeof(buffer))) > 0)
        {
            // write(client_fd, buffer, count);
            response << buffer;
            // std::cout << "response in sh : " << response.str() << std::endl;
        }    
        close(pipefd[0]);
    }
    return ;
}
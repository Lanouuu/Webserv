#ifndef CGI_H
# define CGI_H

# include <iostream>
# include <unistd.h>
# include <sstream>
# include <sys/wait.h>
# include "Server.hpp"

void execCgi(const cgi_map & cgi, std::ostringstream & response, std::string & url);

#endif
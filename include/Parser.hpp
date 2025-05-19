#ifndef PARSER_HPP
# define PARSER_HPP

# define GREEN 	"\033[0;32m"
# define RED	"\033[0;31m"
# define END	"\033[0m"

# include <set>
# include <sstream>
# include "Lexer.hpp"
# include "Server.hpp"

typedef std::map<std::string, Server>   server_map;

class   Parser : public Lexer
{
    public:

        Parser(const std::string file);
        ~Parser(void);

        void    confParser(server_map & servers);
        
    private:

        std::string             _fileName;

        std::set<std::string>   _validBlocks;
        std::set<std::string>   _servIdentifiers;
        std::set<std::string>   _locaIdentifiers;

        void            getValidBlocks(void);
        void            getServIdent(void);
        void            getLocaIdent(void);

        void            checkTokens(void);
        std::string     tokenError(std::string error, t_token & token);
};

#endif
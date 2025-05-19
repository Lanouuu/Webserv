#ifndef PARSER_HPP
# define PARSER_HPP

# include <set>
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

        std::set<std::string>   _validBlocks;
        std::set<std::string>   _servIdentifiers;
        std::set<std::string>   _locaIdentifiers;

        void    getValidBlocks(void);
        void    getServIdent(void);
        void    getLocaIdent(void);

        void    checkTokens(void) const;
};

#endif
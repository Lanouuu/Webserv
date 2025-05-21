#ifndef PARSER_HPP
# define PARSER_HPP

# define GREEN 	"\033[0;32m"
# define RED	"\033[0;31m"
# define END	"\033[0m"

# include <set>
# include <sstream>
# include <algorithm>
# include "Lexer.hpp"
# include "Server.hpp"

typedef std::map<std::string, Server>   server_map;

class   Parser : public Lexer
{
    public:

        Parser(const std::string file);
        ~Parser(void);

        void    parseConf(server_map & servers);
        
    private:

        std::string                     _fileName;

        std::set<std::string>           _validBlocks;
        std::set<std::string>           _servID;
        std::set<std::string>           _locaID;

        std::vector<t_token>::iterator  _current;

        void            getValidBlocks(void);
        void            getServIdent(void);
        void            getLocaIdent(void);

        std::string     tokenErr(std::string error, t_token & token);

        void            parseServer(Server & serv_temp);
        void            parseLocation(Server & serv_temp);
        void            parseServDirective(Server & serv_temp);
        void            parseLocaDirective(Location & loca_temp);

        t_token         *peek(int offset);
        bool            match(std::string key);
        bool            expect(int type);
        void            advance(void);
        void            advanceAndCheck(void);
        void            checkEOF(void);
};

#endif
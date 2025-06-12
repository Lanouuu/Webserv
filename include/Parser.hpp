#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Server.hpp"

class   Parser : public Lexer
{
    public:

        Parser(const std::string file);
        ~Parser(void);

        void    parseConf(serv_vector & servers);
        
    private:

        std::vector<t_token>::iterator  _current;

        void            parseServer(Server & serv_temp);
        void            parseServDirective(Server & serv_temp);
        void            parseLocation(Server & serv_temp);
        void            parseLocFile(Server & serv_temp);
        void            parseLocDir(Server & serv_temp);
        void            parseLocaDirective(Location & loca_temp);
        void            parsePort(Server & serv_temp, std::string & value);
        std::string     parseIP(std::string value);
        void            parseErrorPage(Server & serv_temp);

        t_token         *peek(int offset);
        bool            match(std::string key);
        bool            expect(int type);
        void            advance(void);
        void            advanceAndCheck(void);
        void            checkEOF(void);
};

#endif
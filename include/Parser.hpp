#ifndef PARSER_HPP
# define PARSER_HPP

# define MAX_BODY_SIZE 1073741824

# include "Lexer.hpp"
# include "Server.hpp"
# include <unistd.h>

typedef std::vector<Server>   serv_vector;

class   Parser : public Lexer
{
    public:

        Parser(const std::string file);
        ~Parser(void);

        void                                parseConf(serv_vector & servers);
        
    private:

        std::vector<t_token>::iterator      _current;

        void                                parseServer(Server & serv_temp);
        void                                parseServDirective(Server & serv_temp);
        void                                parseLocation(Server & serv_temp);
        void                                parseLocFile(Server & serv_temp);
        void                                parseLocDir(Server & serv_temp);
        void                                parseLocaDirective(Location & loca_temp);
        void                                parsePort(Server & serv_temp, std::string & value);
        std::string                         parseIP(std::string value);
        void                                parseErrorPage(Server & serv_temp);
        size_t                              parseBodySize(std::string & size);
        void                                parseAlias(Location & loca_temp);
        void                                parseAutoIndex(Location & loca_temp);
        void                                parseSetMethod(Location & loca_temp);
        void                                parseLocaIndex(Location & loca_temp);
        template<typename C>
        void                                parseCgi(C & target);

        t_token                             *peek(int offset);
        bool                                match(std::string key);
        bool                                expect(int type);
        void                                advance(void);
        void                                advanceAndCheck(void);
        void                                checkEOF(void);
        bool                                isExecutableFile(const std::string & path);
};

template<typename C>
void    Parser::parseCgi(C & target)
{
    std::pair<std::string, std::string> pair_temp;
    std::string extension;
    std::string bin_path;
    if (_current->type != String)
         throw std::invalid_argument(tokenErr("expected an extension", *_current));
    extension = _current->value;
    std::cout << extension << std::endl;
    advanceAndCheck();
    if (_current->type != String)
         throw std::invalid_argument(tokenErr("expected a bin path", *_current));
    bin_path = _current->value;
    advanceAndCheck();
    if (_current->type != Semicolon)
        throw std::invalid_argument(tokenErr("expected a \';\' at the end of directive", *peek(-1)));
    if (extension != ".py" && extension != ".sh")
         throw std::invalid_argument(tokenErr("invalid extension", *peek(-2)));
    if (bin_path[0] != '/')
         throw std::invalid_argument(tokenErr("invalid bin path", *peek(-1)));
    if (bin_path[bin_path.size() - 1] == '/')
         throw std::invalid_argument(tokenErr("invalid bin path", *peek(-1)));
    if (!isExecutableFile(bin_path))
        throw std::invalid_argument(tokenErr("no such file or permission denied", *peek(-1)));
    pair_temp.first = extension;
    pair_temp.second = bin_path;
    target.addCgi(pair_temp);
    return ;
}

#endif
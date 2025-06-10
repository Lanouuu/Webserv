#ifndef LEXER_HPP
# define LEXER_HPP

# define GREEN 	"\033[0;32m"
# define RED	"\033[0;31m"
# define BLUE	"\033[0;36m"
# define END	"\033[0m"

# include <iostream>
# include <sstream>
# include <fstream>
# include <algorithm>
# include <vector>
# include <set>
# include <exception>
# include <errno.h>

typedef struct s_token
{
    int         type;
    std::string value;
    size_t      line;
    size_t      pos;
} t_token;

class   Lexer
{
    public:

        Lexer(const std::string file);
        ~Lexer(void);

        enum TokenType
        {
            OpenBrace   = 0,
            CloseBrace  = 1,
            Semicolon   = 2,
            Block       = 3,
            Identifier  = 4,
            String      = 5,
            EndOfFile   = 6,
        };

        void    printTokens(void) const;

    protected:
        
        std::string             _fileName;
        
        std::set<std::string>   _servID;
        std::set<std::string>   _servError;
        std::set<std::string>   _locaID;

        std::vector<t_token>    _tokens;

        std::string     tokenErr(std::string error, t_token & token);

        void            initServIdent(void);
        void            initLocaIdent(void);
        void            initErrorPages(void);

        void            loadFile(const std::string & file);
        bool            checkEmptyLine(const std::string & line);
        void            addDelimToken(int type, const char value, size_t n_line, size_t pos);
        void            addStrToken(const std::string value, size_t n_line, size_t pos);
        void            tokenizer(const std::string & line, size_t n_line);
};

#endif
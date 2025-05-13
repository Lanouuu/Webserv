#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <fstream>
# include <vector>

typedef struct s_token
{
    int         type;
    std::string value;
    int         line;
    int         pos;
} t_token;

class   Lexer
{
    public:

        Lexer(const std::string file);
        ~Lexer();

        enum TokenType
        {
            OpenBrace = 0,
            CloseBrace = 1,
            Semicolon = 2,
            Identifier = 3,
            Number = 4,
            String = 5,
            EndOfFile = 6,
        };

    private:
        
        std::vector<t_token>    _tokens;
        
        void    loadFile(const std::string & file);
        bool    checkEmptyLine(const std::string & line);
        void    tokenizer(const std::string & line, int n_line);
};

#endif
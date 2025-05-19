#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <fstream>
# include <vector>

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
        
        std::vector<t_token>    _tokens;
        
        void    loadFile(const std::string & file);
        bool    checkEmptyLine(const std::string & line);
        void    addDelimToken(int type, const char value, size_t n_line, size_t pos);
        void    addStrToken(const std::string value, size_t n_line, size_t pos);
        void    tokenizer(const std::string & line, size_t n_line);
};

#endif
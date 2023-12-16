#ifndef CS241_SCANNER_H
#define CS241_SCANNER_H
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include <ostream>

class Token;

std::vector<Token> scan(const std::string &input);

class Token
{
public:
    enum Kind
    {
        ID = 0,
        NUM,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        RETURN,
        IF,
        ELSE,
        WHILE,
        PRINTLN,
        WAIN,
        BECOMES,
        INT,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PCT,
        COMMA,
        SEMI,
        NEW,
        DELETE,
        LBRACK,
        RBRACK,
        AMP,
        NULLTOKEN,
        SPACE,
        TAB,
        COMMENT,
    };

private:
    Kind kind;
    std::string lexeme;

public:
    Token(Kind kind, std::string lexeme);

    Kind getKind() const;
    const std::string &getLexeme() const;

    int64_t toNumber() const;
};

std::ostream &operator<<(std::ostream &out, const Token &tok);

class ScanningFailure
{
    std::string message;

public:
    ScanningFailure(std::string message);

    const std::string &what() const;
};

#endif

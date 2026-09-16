#pragma once

#include <string>
#include <vector>

using namespace std;

enum class TokenType
{
    BRIEFS,
    BULGE,
    TIGHT,
    STRAP,
    EXPOSE,
    PACKAGE,
    GRAB,

    IDENTIFIER,
    STRING,
    NUMBER,

    EQUALS,
    EQUAL_EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,

    COMMA,

    END_OF_FILE
};

struct Token
{
    TokenType type;
    string value;
};

class Lexer
{
public:
    Lexer(const string& source);

    vector<Token> Tokenize();

private:
    string source;
    size_t position = 0;
};
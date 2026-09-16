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

    IDENTIFIER,
    STRING,
    NUMBER,

    EQUALS,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    PLUS,

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
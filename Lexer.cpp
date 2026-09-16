#include "Lexer.h"

#include <cctype>

Lexer::Lexer(const string& source)
    : source(source)
{
}

vector<Token> Lexer::Tokenize()
{
    vector<Token> tokens;

    while (position < source.length())
    {
        char current = source[position];

        // Ignore whitespace
        if (isspace(static_cast<unsigned char>(current)))
        {
            position++;
            continue;
        }

        // Single-character symbols
        if (current == '=')
        {
            tokens.push_back({ TokenType::EQUALS, "=" });
            position++;
            continue;
        }

        if (current == '(')
        {
            tokens.push_back({ TokenType::LEFT_PAREN, "(" });
            position++;
            continue;
        }

        if (current == ')')
        {
            tokens.push_back({ TokenType::RIGHT_PAREN, ")" });
            position++;
            continue;
        }

        if (current == '{')
        {
            tokens.push_back({ TokenType::LEFT_BRACE, "{" });
            position++;
            continue;
        }

        if (current == '}')
        {
            tokens.push_back({ TokenType::RIGHT_BRACE, "}" });
            position++;
            continue;
        }

        position++;
    }

    tokens.push_back({ TokenType::END_OF_FILE, "" });

    return tokens;
}
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

        // String literals
        if (current == '"')
        {
            position++; // Skip opening quote

            string value;

            while (position < source.length() && source[position] != '"')
            {
                value += source[position];
                position++;
            }

            // Skip closing quote
            if (position < source.length() && source[position] == '"')
            {
                position++;
            }

            tokens.push_back({ TokenType::STRING, value });
            continue;
        }

        // Numbers
        if (isdigit(static_cast<unsigned char>(current)))
        {
            string number;

            while (position < source.length() &&
                isdigit(static_cast<unsigned char>(source[position])))
            {
                number += source[position];
                position++;
            }

            tokens.push_back({ TokenType::NUMBER, number });
            continue;
        }

        // Words: keywords and identifiers
        if (isalpha(static_cast<unsigned char>(current)) || current == '_')
        {
            string word;

            while (position < source.length() &&
                (isalnum(static_cast<unsigned char>(source[position])) ||
                    source[position] == '_'))
            {
                word += source[position];
                position++;
            }

            if (word == "briefs")
                tokens.push_back({ TokenType::BRIEFS, word });
            else if (word == "bulge")
                tokens.push_back({ TokenType::BULGE, word });
            else if (word == "tight")
                tokens.push_back({ TokenType::TIGHT, word });
            else if (word == "strap")
                tokens.push_back({ TokenType::STRAP, word });
            else if (word == "expose")
                tokens.push_back({ TokenType::EXPOSE, word });
            else
                tokens.push_back({ TokenType::IDENTIFIER, word });

            continue;
        }

        // Single-character symbols

        if (current == '+')
        {
            tokens.push_back({ TokenType::PLUS, "+" });
            position++;
            continue;
        }

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
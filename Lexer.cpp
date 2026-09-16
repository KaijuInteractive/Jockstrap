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
            position++;

            string value;

            while (position < source.length() &&
                source[position] != '"')
            {
                value += source[position];
                position++;
            }

            if (position < source.length() &&
                source[position] == '"')
            {
                position++;
            }

            tokens.push_back({ TokenType::STRING, value });
            continue;
        }

        // Numbers
        if (isdigit(static_cast<unsigned char>(current)) ||
            (current == '.' &&
                position + 1 < source.length() &&
                isdigit(static_cast<unsigned char>(
                    source[position + 1]))))
        {
            string number;
            bool foundDecimal = false;

            while (position < source.length())
            {
                char c = source[position];

                if (isdigit(static_cast<unsigned char>(c)))
                {
                    number += c;
                    position++;
                }
                else if (c == '.' && !foundDecimal)
                {
                    foundDecimal = true;
                    number += c;
                    position++;
                }
                else
                {
                    break;
                }
            }

            tokens.push_back({ TokenType::NUMBER, number });
            continue;
        }

        // Keywords and identifiers
        if (isalpha(static_cast<unsigned char>(current)) ||
            current == '_')
        {
            string word;

            while (position < source.length() &&
                (isalnum(static_cast<unsigned char>(
                    source[position])) ||
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
            else if (word == "package")
                tokens.push_back({ TokenType::PACKAGE, word });
            else if (word == "grab")
                tokens.push_back({ TokenType::GRAB, word });
            else
                tokens.push_back({ TokenType::IDENTIFIER, word });

            continue;
        }

        // Two-character comparison operators
        if (current == '=' &&
            position + 1 < source.length() &&
            source[position + 1] == '=')
        {
            tokens.push_back({ TokenType::EQUAL_EQUAL, "==" });
            position += 2;
            continue;
        }

        if (current == '!' &&
            position + 1 < source.length() &&
            source[position + 1] == '=')
        {
            tokens.push_back({ TokenType::NOT_EQUAL, "!=" });
            position += 2;
            continue;
        }

        if (current == '>' &&
            position + 1 < source.length() &&
            source[position + 1] == '=')
        {
            tokens.push_back({ TokenType::GREATER_EQUAL, ">=" });
            position += 2;
            continue;
        }

        if (current == '<' &&
            position + 1 < source.length() &&
            source[position + 1] == '=')
        {
            tokens.push_back({ TokenType::LESS_EQUAL, "<=" });
            position += 2;
            continue;
        }

        // Single-character operators
        if (current == '>')
        {
            tokens.push_back({ TokenType::GREATER, ">" });
            position++;
            continue;
        }

        if (current == '<')
        {
            tokens.push_back({ TokenType::LESS, "<" });
            position++;
            continue;
        }

        if (current == '=')
        {
            tokens.push_back({ TokenType::EQUALS, "=" });
            position++;
            continue;
        }

        if (current == '+')
        {
            tokens.push_back({ TokenType::PLUS, "+" });
            position++;
            continue;
        }

        if (current == '-')
        {
            tokens.push_back({ TokenType::MINUS, "-" });
            position++;
            continue;
        }

        if (current == '*')
        {
            tokens.push_back({ TokenType::STAR, "*" });
            position++;
            continue;
        }

        if (current == '/')
        {
            tokens.push_back({ TokenType::SLASH, "/" });
            position++;
            continue;
        }

        // Grouping
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

        if (current == ',')
        {
            tokens.push_back({ TokenType::COMMA, "," });
            position++;
            continue;
        }

        // Unknown character
        position++;
    }

    tokens.push_back({ TokenType::END_OF_FILE, "" });

    return tokens;
}
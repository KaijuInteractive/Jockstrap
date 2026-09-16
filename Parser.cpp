#include "Parser.h"

#include <iostream>

Parser::Parser(const vector<Token>& tokens)
    : tokens(tokens)
{
}

Token Parser::CurrentToken()
{
    return tokens[position];
}

void Parser::Advance()
{
    if (position < tokens.size() - 1)
    {
        position++;
    }
}

void Parser::Parse()
{
    while (CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::BRIEFS)
        {
            ParseVariableDeclaration();
        }
        else if (CurrentToken().type == TokenType::EXPOSE)
        {
            ParseExpose();
        }
        else
        {
            Advance();
        }
    }
}

void Parser::ParseVariableDeclaration()
{
    // Skip "briefs"
    Advance();

    if (CurrentToken().type != TokenType::IDENTIFIER)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected a variable name after briefs." << endl;
        return;
    }

    string variableName = CurrentToken().value;
    Advance();

    if (CurrentToken().type != TokenType::EQUALS)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '=' after " << variableName << "." << endl;
        return;
    }

    Advance();

    if (CurrentToken().type != TokenType::STRING)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected a string value." << endl;
        return;
    }

    string value = CurrentToken().value;
    Advance();

    variables[variableName] = value;

    cout << "Created briefs: "
        << variableName
        << " = \""
        << value
        << "\""
        << endl;
}

void Parser::ParseExpose()
{
    // Skip "expose"
    Advance();

    if (CurrentToken().type != TokenType::IDENTIFIER)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected something to expose." << endl;
        return;
    }

    string variableName = CurrentToken().value;
    Advance();

    if (variables.find(variableName) == variables.end())
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Unknown briefs: " << variableName << endl;
        return;
    }

    cout << variables[variableName] << endl;
}
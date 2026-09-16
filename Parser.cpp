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

    if (CurrentToken().type == TokenType::STRING)
    {
        string value = CurrentToken().value;
        Advance();

        variables[variableName] = value;

        cout << "Created briefs: "
            << variableName
            << " = \""
            << value
            << "\""
            << endl;

        return;
    }

    if (CurrentToken().type == TokenType::NUMBER)
    {
        double value = stod(CurrentToken().value);
        Advance();

        numberVariables[variableName] = value;

        cout << "Created briefs: "
            << variableName
            << " = "
            << value
            << endl;

        return;
    }

    cout << "WARDROBE MALFUNCTION!" << endl;
    cout << "Expected a string or number." << endl;
}

void Parser::ParseExpose()
{
    // Skip "expose"
    Advance();

    // Strings are printed directly
    if (CurrentToken().type == TokenType::STRING)
    {
        cout << CurrentToken().value << endl;
        Advance();
        return;
    }

    // Everything else is treated as a numeric expression
    double result = ParseExpression();
    cout << result << endl;
}

double Parser::ParseExpression()
{
    double left = 0;

    // Get the left side
    if (CurrentToken().type == TokenType::NUMBER)
    {
        left = stod(CurrentToken().value);
        Advance();
    }
    else if (CurrentToken().type == TokenType::IDENTIFIER)
    {
        string variableName = CurrentToken().value;
        Advance();

        if (numberVariables.find(variableName) != numberVariables.end())
        {
            left = numberVariables[variableName];
        }
        else
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "Unknown numeric briefs: " << variableName << endl;
            return 0;
        }
    }
    else
    {
        return 0;
    }

    // Do we have a + ?
    if (CurrentToken().type == TokenType::PLUS)
    {
        Advance();

        double right = ParseExpression();

        return left + right;
    }

    return left;
}
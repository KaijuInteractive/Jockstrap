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
        else if (CurrentToken().type == TokenType::TIGHT)
        {
            ParseTight();
        }
        else if (CurrentToken().type == TokenType::STRAP)
        {
            ParseStrap();
        }
        else if (CurrentToken().type == TokenType::IDENTIFIER)
        {
            ParseAssignment();
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

    // String variable
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

    // Numeric variable
    double value = ParseExpression();

    numberVariables[variableName] = value;

    cout << "Created briefs: "
        << variableName
        << " = "
        << value
        << endl;
}

void Parser::ParseAssignment()
{
    string variableName = CurrentToken().value;
    Advance();

    if (CurrentToken().type != TokenType::EQUALS)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '=' after " << variableName << "." << endl;
        return;
    }

    Advance();

    // Existing string variable
    if (variables.find(variableName) != variables.end())
    {
        if (CurrentToken().type != TokenType::STRING)
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "Expected a string for " << variableName << "." << endl;
            return;
        }

        variables[variableName] = CurrentToken().value;
        Advance();

        return;
    }

    // Existing numeric variable
    if (numberVariables.find(variableName) != numberVariables.end())
    {
        double value = ParseExpression();

        numberVariables[variableName] = value;

        return;
    }

    cout << "WARDROBE MALFUNCTION!" << endl;
    cout << "Unknown briefs: " << variableName << endl;
}

void Parser::ParseExpose()
{
    // Skip "expose"
    Advance();

    // Literal string
    if (CurrentToken().type == TokenType::STRING)
    {
        cout << CurrentToken().value << endl;
        Advance();
        return;
    }

    // String variable
    if (CurrentToken().type == TokenType::IDENTIFIER)
    {
        string variableName = CurrentToken().value;

        if (variables.find(variableName) != variables.end())
        {
            cout << variables[variableName] << endl;
            Advance();
            return;
        }
    }

    // Numeric expression
    double result = ParseExpression();

    cout << result << endl;
}

double Parser::ParseExpression()
{
    double left = ParseTerm();

    while (CurrentToken().type == TokenType::PLUS ||
        CurrentToken().type == TokenType::MINUS)
    {
        TokenType operation = CurrentToken().type;
        Advance();

        double right = ParseTerm();

        if (operation == TokenType::PLUS)
        {
            left += right;
        }
        else
        {
            left -= right;
        }
    }

    return left;
}

double Parser::ParseTerm()
{
    double left = ParseFactor();

    while (CurrentToken().type == TokenType::STAR ||
        CurrentToken().type == TokenType::SLASH)
    {
        TokenType operation = CurrentToken().type;
        Advance();

        double right = ParseFactor();

        if (operation == TokenType::STAR)
        {
            left *= right;
        }
        else
        {
            if (right == 0)
            {
                cout << "WARDROBE MALFUNCTION!" << endl;
                cout << "Division by zero would tear the fabric of reality."
                    << endl;

                return 0;
            }

            left /= right;
        }
    }

    return left;
}

double Parser::ParseFactor()
{
    // Number
    if (CurrentToken().type == TokenType::NUMBER)
    {
        double value = stod(CurrentToken().value);
        Advance();

        return value;
    }

    // Numeric variable
    if (CurrentToken().type == TokenType::IDENTIFIER)
    {
        string variableName = CurrentToken().value;
        Advance();

        if (numberVariables.find(variableName) != numberVariables.end())
        {
            return numberVariables[variableName];
        }

        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Unknown numeric briefs: "
            << variableName
            << endl;

        return 0;
    }

    // Parenthesized expression
    if (CurrentToken().type == TokenType::LEFT_PAREN)
    {
        Advance();

        double value = ParseExpression();

        if (CurrentToken().type != TokenType::RIGHT_PAREN)
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "Expected ')'." << endl;

            return value;
        }

        Advance();

        return value;
    }

    // Unary minus
    if (CurrentToken().type == TokenType::MINUS)
    {
        Advance();

        return -ParseFactor();
    }

    cout << "WARDROBE MALFUNCTION!" << endl;
    cout << "Expected a number, variable, or expression." << endl;

    return 0;
}

bool Parser::ParseCondition()
{
    double left = ParseExpression();

    TokenType comparison = CurrentToken().type;

    if (comparison != TokenType::GREATER &&
        comparison != TokenType::LESS &&
        comparison != TokenType::EQUAL_EQUAL)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected comparison operator." << endl;

        return false;
    }

    Advance();

    double right = ParseExpression();

    if (comparison == TokenType::GREATER)
    {
        return left > right;
    }

    if (comparison == TokenType::LESS)
    {
        return left < right;
    }

    return left == right;
}

void Parser::ParseTight()
{
    // Skip "tight"
    Advance();

    if (CurrentToken().type != TokenType::LEFT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '(' after tight." << endl;
        return;
    }

    Advance();

    bool condition = ParseCondition();

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected ')' after tight condition." << endl;
        return;
    }

    Advance();

    if (CurrentToken().type != TokenType::LEFT_BRACE)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '{' after tight condition." << endl;
        return;
    }

    Advance();

    if (condition)
    {
        ExecuteBlock();
    }
    else
    {
        SkipBlock();
    }

    // bulge = else
    if (CurrentToken().type == TokenType::BULGE)
    {
        Advance();

        if (CurrentToken().type != TokenType::LEFT_BRACE)
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "Expected '{' after bulge." << endl;
            return;
        }

        Advance();

        if (!condition)
        {
            ExecuteBlock();
        }
        else
        {
            SkipBlock();
        }
    }
}

void Parser::ParseStrap()
{
    // Save where the strap statement begins.
    size_t strapPosition = position;

    // Skip "strap"
    Advance();

    if (CurrentToken().type != TokenType::LEFT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '(' after strap." << endl;
        return;
    }

    Advance();

    // Save where the condition begins.
    size_t conditionPosition = position;

    // Parse once so we can locate the body.
    bool condition = ParseCondition();

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected ')' after strap condition." << endl;
        return;
    }

    Advance();

    if (CurrentToken().type != TokenType::LEFT_BRACE)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '{' after strap condition." << endl;
        return;
    }

    Advance();

    size_t bodyPosition = position;

    // Find the token immediately after the loop body.
    int depth = 1;

    while (depth > 0 &&
        CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::LEFT_BRACE)
        {
            depth++;
        }
        else if (CurrentToken().type == TokenType::RIGHT_BRACE)
        {
            depth--;
        }

        Advance();
    }

    size_t afterBodyPosition = position;

    // Execute while condition remains true.
    while (condition)
    {
        position = bodyPosition;

        ExecuteBlock();

        // Re-evaluate the condition using current variable values.
        position = conditionPosition;

        condition = ParseCondition();
    }

    // Continue after the strap block.
    position = afterBodyPosition;

    // Prevent unused-variable warning if warnings are enabled.
    (void)strapPosition;
}

void Parser::ExecuteBlock()
{
    while (CurrentToken().type != TokenType::RIGHT_BRACE &&
        CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::BRIEFS)
        {
            ParseVariableDeclaration();
        }
        else if (CurrentToken().type == TokenType::EXPOSE)
        {
            ParseExpose();
        }
        else if (CurrentToken().type == TokenType::TIGHT)
        {
            ParseTight();
        }
        else if (CurrentToken().type == TokenType::STRAP)
        {
            ParseStrap();
        }
        else if (CurrentToken().type == TokenType::IDENTIFIER)
        {
            ParseAssignment();
        }
        else
        {
            Advance();
        }
    }

    if (CurrentToken().type == TokenType::RIGHT_BRACE)
    {
        Advance();
    }
}

void Parser::SkipBlock()
{
    int depth = 1;

    while (depth > 0 &&
        CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::LEFT_BRACE)
        {
            depth++;
        }
        else if (CurrentToken().type == TokenType::RIGHT_BRACE)
        {
            depth--;
        }

        Advance();
    }
}
#include "Parser.h"

#include <iostream>
#include <limits>

Parser::Parser(const vector<Token>& tokens)
    : tokens(tokens)
{
}

Token Parser::CurrentToken()
{
    return tokens[position];
}

Token Parser::PeekToken()
{
    if (position + 1 < tokens.size())
    {
        return tokens[position + 1];
    }

    return tokens.back();
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
        else if (CurrentToken().type == TokenType::GRAB)
        {
            ParseGrab();
        }
        else if (CurrentToken().type == TokenType::TIGHT)
        {
            ParseTight();
        }
        else if (CurrentToken().type == TokenType::STRAP)
        {
            ParseStrap();
        }
        else if (CurrentToken().type == TokenType::PACKAGE)
        {
            ParsePackage();
        }
        else if (CurrentToken().type == TokenType::IDENTIFIER)
        {
            if (PeekToken().type == TokenType::LEFT_PAREN)
            {
                ParseFunctionCall();
            }
            else
            {
                ParseAssignment();
            }
        }
        else
        {
            Advance();
        }
    }
}

void Parser::ParseVariableDeclaration()
{
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

    if (variables.find(variableName) != variables.end())
    {
        if (CurrentToken().type != TokenType::STRING)
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "Expected a string for "
                << variableName
                << "."
                << endl;
            return;
        }

        variables[variableName] = CurrentToken().value;
        Advance();

        return;
    }

    if (numberVariables.find(variableName) !=
        numberVariables.end())
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

void Parser::ParseGrab()
{
    // Skip "grab"
    Advance();

    if (CurrentToken().type != TokenType::IDENTIFIER)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected a briefs name after grab." << endl;
        return;
    }

    string variableName = CurrentToken().value;
    Advance();

    // String input
    if (variables.find(variableName) != variables.end())
    {
        string value;

        getline(cin >> ws, value);

        variables[variableName] = value;

        return;
    }

    // Numeric input
    if (numberVariables.find(variableName) !=
        numberVariables.end())
    {
        double value;

        if (!(cin >> value))
        {
            cout << "WARDROBE MALFUNCTION!" << endl;
            cout << "That package requires a number." << endl;

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n');

            return;
        }

        numberVariables[variableName] = value;

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n');

        return;
    }

    cout << "WARDROBE MALFUNCTION!" << endl;
    cout << "Unknown briefs: "
        << variableName
        << endl;
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
                cout << "Division by zero would tear the "
                    << "fabric of reality."
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
    if (CurrentToken().type == TokenType::NUMBER)
    {
        double value = stod(CurrentToken().value);
        Advance();

        return value;
    }

    if (CurrentToken().type == TokenType::IDENTIFIER)
    {
        string variableName = CurrentToken().value;
        Advance();

        if (numberVariables.find(variableName) !=
            numberVariables.end())
        {
            return numberVariables[variableName];
        }

        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Unknown numeric briefs: "
            << variableName
            << endl;

        return 0;
    }

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

    if (CurrentToken().type == TokenType::MINUS)
    {
        Advance();
        return -ParseFactor();
    }

    cout << "WARDROBE MALFUNCTION!" << endl;
    cout << "Expected a number, variable, or expression."
        << endl;

    return 0;
}

bool Parser::ParseCondition()
{
    double left = ParseExpression();

    TokenType comparison = CurrentToken().type;

    if (comparison != TokenType::GREATER &&
        comparison != TokenType::LESS &&
        comparison != TokenType::EQUAL_EQUAL &&
        comparison != TokenType::NOT_EQUAL &&
        comparison != TokenType::GREATER_EQUAL &&
        comparison != TokenType::LESS_EQUAL)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected comparison operator." << endl;

        return false;
    }

    Advance();

    double right = ParseExpression();

    if (comparison == TokenType::GREATER)
        return left > right;

    if (comparison == TokenType::LESS)
        return left < right;

    if (comparison == TokenType::GREATER_EQUAL)
        return left >= right;

    if (comparison == TokenType::LESS_EQUAL)
        return left <= right;

    if (comparison == TokenType::NOT_EQUAL)
        return left != right;

    return left == right;
}

void Parser::ParseTight()
{
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
    Advance();

    if (CurrentToken().type != TokenType::LEFT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '(' after strap." << endl;
        return;
    }

    Advance();

    size_t conditionPosition = position;

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

    int depth = 1;

    while (depth > 0 &&
        CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::LEFT_BRACE)
        {
            depth++;
        }
        else if (CurrentToken().type ==
            TokenType::RIGHT_BRACE)
        {
            depth--;
        }

        Advance();
    }

    size_t afterBodyPosition = position;

    while (condition)
    {
        position = bodyPosition;

        ExecuteBlock();

        position = conditionPosition;

        condition = ParseCondition();
    }

    position = afterBodyPosition;
}

void Parser::ParsePackage()
{
    Advance();

    if (CurrentToken().type != TokenType::IDENTIFIER)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected a package name." << endl;
        return;
    }

    string functionName = CurrentToken().value;
    Advance();

    if (CurrentToken().type != TokenType::LEFT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '(' after package name." << endl;
        return;
    }

    Advance();

    vector<string> parameters;

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        while (true)
        {
            if (CurrentToken().type != TokenType::IDENTIFIER)
            {
                cout << "WARDROBE MALFUNCTION!" << endl;
                cout << "Expected parameter name." << endl;
                return;
            }

            parameters.push_back(CurrentToken().value);
            Advance();

            if (CurrentToken().type == TokenType::COMMA)
            {
                Advance();
                continue;
            }

            break;
        }
    }

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected ')' after package parameters."
            << endl;
        return;
    }

    Advance();

    if (CurrentToken().type != TokenType::LEFT_BRACE)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '{' after package declaration."
            << endl;
        return;
    }

    Advance();

    size_t bodyStart = position;

    int depth = 1;

    while (depth > 0 &&
        CurrentToken().type != TokenType::END_OF_FILE)
    {
        if (CurrentToken().type == TokenType::LEFT_BRACE)
        {
            depth++;
        }
        else if (CurrentToken().type ==
            TokenType::RIGHT_BRACE)
        {
            depth--;
        }

        Advance();
    }

    size_t bodyEnd = position - 1;

    functions[functionName] =
    {
        parameters,
        bodyStart,
        bodyEnd
    };

    cout << "Packed function: "
        << functionName
        << endl;
}

void Parser::ParseFunctionCall()
{
    string functionName = CurrentToken().value;
    Advance();

    auto functionIt = functions.find(functionName);

    if (functionIt == functions.end())
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Unknown package: "
            << functionName
            << endl;
        return;
    }

    if (CurrentToken().type != TokenType::LEFT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected '(' after package name." << endl;
        return;
    }

    Advance();

    vector<double> arguments;

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        while (true)
        {
            double value = ParseExpression();
            arguments.push_back(value);

            if (CurrentToken().type == TokenType::COMMA)
            {
                Advance();
                continue;
            }

            break;
        }
    }

    if (CurrentToken().type != TokenType::RIGHT_PAREN)
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Expected ')' after package arguments."
            << endl;
        return;
    }

    Advance();

    Function& function = functionIt->second;

    if (arguments.size() != function.parameters.size())
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Package "
            << functionName
            << " expected "
            << function.parameters.size()
            << " arguments, but received "
            << arguments.size()
            << "."
            << endl;

        return;
    }

    size_t returnPosition = position;

    unordered_map<string, double> savedValues;
    unordered_map<string, bool> existedBefore;

    for (size_t i = 0;
        i < function.parameters.size();
        i++)
    {
        string parameterName = function.parameters[i];

        auto existing =
            numberVariables.find(parameterName);

        if (existing != numberVariables.end())
        {
            existedBefore[parameterName] = true;
            savedValues[parameterName] =
                existing->second;
        }
        else
        {
            existedBefore[parameterName] = false;
        }

        numberVariables[parameterName] =
            arguments[i];
    }

    position = function.bodyStart;

    ExecuteBlock();

    for (const string& parameterName :
        function.parameters)
    {
        if (existedBefore[parameterName])
        {
            numberVariables[parameterName] =
                savedValues[parameterName];
        }
        else
        {
            numberVariables.erase(parameterName);
        }
    }

    position = returnPosition;
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
        else if (CurrentToken().type == TokenType::GRAB)
        {
            ParseGrab();
        }
        else if (CurrentToken().type == TokenType::TIGHT)
        {
            ParseTight();
        }
        else if (CurrentToken().type == TokenType::STRAP)
        {
            ParseStrap();
        }
        else if (CurrentToken().type == TokenType::PACKAGE)
        {
            ParsePackage();
        }
        else if (CurrentToken().type == TokenType::IDENTIFIER)
        {
            if (PeekToken().type == TokenType::LEFT_PAREN)
            {
                ParseFunctionCall();
            }
            else
            {
                ParseAssignment();
            }
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
        else if (CurrentToken().type ==
            TokenType::RIGHT_BRACE)
        {
            depth--;
        }

        Advance();
    }
}
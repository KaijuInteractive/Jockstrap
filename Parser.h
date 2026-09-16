#pragma once

#include <vector>
#include <string>
#include "Lexer.h"

#include <unordered_map>

using namespace std;

class Parser
{
public:
    Parser(const vector<Token>& tokens);

    void Parse();

private:
    vector<Token> tokens;
    size_t position = 0;

    unordered_map<string, string> variables;
    unordered_map<string, double> numberVariables;

    Token CurrentToken();
    void Advance();

    void ParseVariableDeclaration();
    void ParseExpose();
    double ParseExpression();
};

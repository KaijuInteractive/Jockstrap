#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Lexer.h"

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
    void ParseAssignment();
    void ParseExpose();
    void ParseTight();
    void ParseStrap();

    double ParseExpression();
    double ParseTerm();
    double ParseFactor();

    bool ParseCondition();

    void ExecuteBlock();
    void SkipBlock();
};
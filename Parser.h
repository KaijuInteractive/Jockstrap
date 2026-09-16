#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Lexer.h"

using namespace std;

struct Function
{
    vector<string> parameters;
    size_t bodyStart;
    size_t bodyEnd;
};

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
    unordered_map<string, Function> functions;

    Token CurrentToken();
    Token PeekToken();

    void Advance();

    void ParseVariableDeclaration();
    void ParseAssignment();
    void ParseExpose();
    void ParseGrab();

    void ParseTight();
    void ParseStrap();

    void ParsePackage();
    void ParseFunctionCall();

    double ParseExpression();
    double ParseTerm();
    double ParseFactor();

    bool ParseCondition();

    void ExecuteBlock();
    void SkipBlock();
};
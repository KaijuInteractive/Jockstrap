#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"

using namespace std;

string TokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::BRIEFS:      return "BRIEFS";
    case TokenType::BULGE:       return "BULGE";
    case TokenType::TIGHT:       return "TIGHT";
    case TokenType::STRAP:       return "STRAP";
    case TokenType::EXPOSE:      return "EXPOSE";
    case TokenType::IDENTIFIER:  return "IDENTIFIER";
    case TokenType::STRING:      return "STRING";
    case TokenType::NUMBER:      return "NUMBER";
    case TokenType::EQUALS:      return "EQUALS";
    case TokenType::LEFT_PAREN:  return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:  return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
    case TokenType::END_OF_FILE: return "END_OF_FILE";
    }

    return "UNKNOWN";
}

int main()
{
    cout << "Jockstrap Programming Language" << endl;
    cout << "Version 0.1" << endl;
    cout << endl;

    ifstream file("hello.jock");

    if (!file.is_open())
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Could not open hello.jock" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string source = buffer.str();

    Lexer lexer(source);
    vector<Token> tokens = lexer.Tokenize();

    Parser parser(tokens);
    parser.Parse();

    cout << "Securing package..." << endl;
    cout << endl;

    for (const Token& token : tokens)
    {
        cout << TokenTypeToString(token.type);

        if (!token.value.empty())
        {
            cout << " : " << token.value;
        }

        cout << endl;
    }

    cout << endl;
    cout << "Everything looks snug." << endl;

    return 0;
}
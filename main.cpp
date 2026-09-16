#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main()
{
    // Load the Jockstrap program
    ifstream file("hello.jock");

    if (!file.is_open())
    {
        cout << "WARDROBE MALFUNCTION!" << endl;
        cout << "Could not open hello.jock" << endl;
        return 1;
    }

    // Read the entire .jock file
    stringstream buffer;
    buffer << file.rdbuf();

    string source = buffer.str();

    // Turn source code into tokens
    Lexer lexer(source);
    vector<Token> tokens = lexer.Tokenize();

    // Execute the Jockstrap program
    Parser parser(tokens);
    parser.Parse();

    return 0;
}
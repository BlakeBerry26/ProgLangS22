/*********************************************************************
Name: Andrew Blake Berry NetID: abb576
Course: CSE 4714 Assignment: Part 3
Programming Environment: VS Code
Purpose of File: driver for Project Part 3 (parser for TIPS)
*********************************************************************/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string>
#include <set>

using namespace std;

#include "lexer.h"
#include "productions.h"
#include "parse_tree_nodes.h"

extern "C" {
    // Instantiate global variables
    extern FILE *yyin; // input stream
    extern FILE *yyout; // output stream
    extern int yyleng; // length of current lexeme
    extern char *yytext; // text of current lexeme
    extern int yylineno; // line number for current lexeme
    extern int yylex(); // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex


// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
set<string> symbolTable; // Symbol Table


//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

    // Set the input stream
    if (argc > 1) {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
  
    if (!yyin) {
        printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;

    // Get the first token
    nextToken = yylex();
    ProgramNode* p = new ProgramNode();

    // Fire up the parser!
    try {
        
        program(p);  // Process <program> production

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near " << yytext << ", error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;

    // Print out the symbol table
    cout << endl << "User Defined Symbols:" << endl;
    set<string>::iterator it;
    for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << *it << endl;
    }

    cout << endl << endl << "*** In order traversal of parse tree ***" << endl;
    cout << *p;

    // cout << endl << endl << "*** Delete the parse tree ***" << endl;
    // delete p;
    // p = nullptr;

    cout << endl;
    delete yyin;
    delete yyout;

    return EXIT_SUCCESS;
}

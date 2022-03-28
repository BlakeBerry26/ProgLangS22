//*****************************************************************************
// purpose: driver file for parser example - flex generated lexer
//          builds parse tree while parsing the input
// version: Spring 2022
// author: Joe Crumpton / Ed Swan
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"
#include <iostream>
using namespace std;


extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;   // input stream
  extern FILE *yyout;  // output stream
  extern int yyleng;   // length of current lexeme
  extern char *yytext; // text of current lexeme
  extern int yylex();  // the generated lexical analyzer
}

int main( int argc, char* argv[] )
{
   /* Open the input data file and process its contents */
  if ((yyin = fopen("front.in", "r")) == NULL) {
    cout << "ERROR - cannot open front.in" << endl;
    return(EXIT_FAILURE);
  }
  // Uncomment this line to interactively type your own expression.
  // If so, to end must type "Ctrl-D" (generate EOF character)
  yyin = stdin;

  // Create the root of the parse tree
  ExprNode* root = nullptr;

  lex();  // prime the pump (get first token)
  do {
    root = expr(); // start symbol is <expr>
  } while (nextToken != TOK_EOF);

  if (yyin)
    fclose(yyin);

  cout << endl << "*** In order traversal of parse tree ***" << endl;
  cout << *root << endl << endl;

  cout << "*** Delete the parse tree ***" << endl;
  delete root;
  root = nullptr;

  return(EXIT_SUCCESS);
}

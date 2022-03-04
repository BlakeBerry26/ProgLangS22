/*
Complete this file header...

*/

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

extern "C"
{
  extern int   yylex();      // the generated lexical analyzer
  extern char *yytext;       // text of current lexeme
}

int nextToken = 0;

// Production functions
bool is_noun_phrase(int);
bool is_adjective_phrase(int);
bool is_verb_phrase(int);
bool is_article_or_possessive();
bool is_adjective();
bool is_noun();
bool is_adverb();
bool is_verb();


//*****************************************************************************
// Get the next lexeme (word in sentence)
int lex() {
  nextToken = yylex();
  if( nextToken == TOK_EOF ) {
    yytext[0] = 'E'; yytext[1] = 'O'; yytext[2] = 'F'; yytext[3] = 0;   
  }

  printf("Next token: %d, lexeme: |%s|\n", nextToken, yytext);
  return nextToken;
}
//*****************************************************************************
// Report what we found
void output( string what ) {
  cout << "===> Accepted " << what << ": |" << yytext << "| <===" << endl;
}
//*****************************************************************************
// <sentence> -> <noun phrase> <verb phrase> <noun phrase>
void sentence() 
{
  if(nextToken != ARTICLE && nextToken != POSSESSIVE)
    throw( "<sentence> did not start with an article or possessive." );

  cout << "Enter <sentence>" << endl;

  is_noun_phrase(1);

  is_verb_phrase(1);

  is_noun_phrase(2);

  cout << "Exit <sentence>" << endl;
} 
//*****************************************************************************
bool is_noun_phrase(int num) {
  if(nextToken != ARTICLE && nextToken != POSSESSIVE)
    throw( "<noun phrase> did not start with an article or possessive." );
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Enter <noun phrase> " << num << "\n";

  is_adjective_phrase(num);

  if (is_noun() == false)
    throw("<noun phrase> did not have a noun.");

  lex();
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Exit <noun phrase> " << num << "\n";
  return true;
}

bool is_adjective_phrase(int num) {
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Enter <adjective phrase> " << num << "\n";
  if (is_article_or_possessive() != true)
    throw("<adjective phrase> did not start with an article or possessive.");

  lex();
  if (is_adjective() != true)
    throw("<adjective phrase> did not have an adjective.");

  lex();
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Exit <adjective phrase> " << num << "\n";
  return true;
}

bool is_verb_phrase(int num) {
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Enter <verb phrase> " << num << "\n";
  if (is_adverb()) {
    lex();
    is_verb_phrase(num + 1);
  }
  else if (is_verb() == false && is_adverb() == false)
    throw("<verb phrase> did not start with a verb or an adverb.");
  else
    lex();
  for (int i = 0; i < num; i++) {
    cout << "  ";
  }
  cout << "Exit <verb phrase> " << num << "\n";
  return true;
}

bool is_article_or_possessive() {
  if (nextToken == ARTICLE) {
    output("ARTICLE");
    return true;
  }
  else if (nextToken == POSSESSIVE) {
    output("POSSESSIVE");
    return true;
  }
  else {
    return false;
  }
}

bool is_adjective() {
  if (nextToken == ADJECTIVE) {
    output("ADJECTIVE");
    return true;
  }
  else {
    return false;
  }
}

bool is_noun() {
  if (nextToken == NOUN) {
    output("NOUN");
    return true;
  }
  else {
    return false;
  }
}

bool is_adverb() {
  if (nextToken == ADVERB) {
    output("ADVERB");
    return true;
  }
  else {
    return false;
  }
}

bool is_verb() {
  if (nextToken == VERB) {
    output("VERB");
    return true;
  }
  else {
    return false;
  }
}
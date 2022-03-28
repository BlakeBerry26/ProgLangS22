//*****************************************************************************
// purpose: header file for flex example, defines tokens
// version: Spring 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef LEXER_H
#define LEXER_H

// Include standard IO streams
#include <stdio.h>

// List of token codes

// Data Types
#define TOK_INT_LIT      10  // integer literal
#define TOK_IDENT        11  // identifier

// Operators
#define TOK_ADD_OP       21
#define TOK_SUB_OP       22
#define TOK_MULT_OP      23
#define TOK_DIV_OP       24

// Punctuation
#define TOK_LEFT_PAREN   25
#define TOK_RIGHT_PAREN  26
 
#define TOK_EOF          90  // end of file
#define TOK_UNKNOWN      99  // unknown lexeme, not used in front.c;
                             // the textbook's parser used EOF for unknown
                             // lexemes and end-of-file

#endif


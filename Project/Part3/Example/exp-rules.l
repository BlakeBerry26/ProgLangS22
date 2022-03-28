/******************************************************************* 
purpose: CSE 4713 / 6713 example rules file for flex,
         recognizes lexemes for mathematical expressions 
version: Spring 2022
author: Joe Crumpton / Ed Swan
 *******************************************************************/
%option noyywrap
%{
#include "lexer.h"
%}

%%

 /* Data Types */

[0-9]+                 { return TOK_INT_LIT; }
[A-Za-z][0-9A-Za-z]*   { return TOK_IDENT; }

 /* Operators */

\+        { return TOK_ADD_OP; }
-         { return TOK_SUB_OP; }
\*        { return TOK_MULT_OP; }
\/        { return TOK_DIV_OP; }

 /* Punctuation */

\(        { return TOK_LEFT_PAREN; }
\)        { return TOK_RIGHT_PAREN; }


 /* Eat any whitespace */

[ \t\r\n]+             /* nop */

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }



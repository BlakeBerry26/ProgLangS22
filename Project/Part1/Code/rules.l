/******************************************************************* 
write an appropriate header comment, see the assignment
*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

 /* Useful Abstraction Regex */
digit [0-9]
letter [A-Z]
identifier [A-Z][0-9A-Z]{0,7}
integer [+-]?{digit}+
floating [+-]?{digit}+"."{digit}+
string '[^\n']{0,80}'
not_string '[^\n']{81,}'

%%

 /* Keyword Lexemes */
"BEGIN" { return TOK_BEGIN;}
"BREAK" { return TOK_BREAK;}
"CONTINUE" { return TOK_CONTINUE;}
"DOWNTO" { return TOK_DOWNTO;}
"ELSE" { return TOK_ELSE;}
"END" { return TOK_END;}
"FOR" { return TOK_FOR;}
"IF" { return TOK_IF;}
"LET" { return TOK_LET;}
"PROGRAM" { return TOK_PROGRAM;}
"READ" { return TOK_READ;}
"THEN" { return TOK_THEN;}
"TO" { return TOK_TO;}
"VAR" { return TOK_VAR;}
"WHILE" { return TOK_WHILE;}
"WRITE" { return TOK_WRITE;}

 /* Datatype Specifier Lexemes */
"INTEGER" { return TOK_INTEGER;}
"REAL" { return TOK_REAL;}

 /* Punctuation Lexemes */
";" { return TOK_SEMICOLON;}
":" { return TOK_COLON;}
"(" { return TOK_OPENPAREN;}
")" { return TOK_CLOSEPAREN;}
"{" { return TOK_OPENBRACE;}
"}" { return TOK_CLOSEBRACE;}

 /* Operator Lexemes */
"+" { return TOK_PLUS;}
"-" { return TOK_MINUS;}
"*" { return TOK_MULTIPLY;}
"/" { return TOK_DIVIDE;}
":=" { return TOK_ASSIGN;}
"=" { return TOK_EQUALTO;}
"<" { return TOK_LESSTHAN;}
">" { return TOK_GREATERTHAN;}
"<>" { return TOK_NOTEQUALTO;}
"MOD" { return TOK_MOD;}
"NOT" { return TOK_NOT;}
"OR" { return TOK_OR;}
"AND" { return TOK_AND;}

 /* Useful Abstraction Lexemes */
{identifier} { return TOK_IDENT;}
{integer} { return TOK_INTLIT;}
{floating} { return TOK_FLOATLIT;}
{string} {return TOK_STRINGLIT;}

[ \t\r\n]+             /* nop */




 /* Found an unknown character */

. { return TOK_UNKNOWN; }
{not_string} { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }



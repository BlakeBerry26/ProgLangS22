/*********************************************************************
  Name: Any Student             NetID: as3
  Course: CSE 4714              
  Lab: 01 - Lexical Analyzer 
  Example of reading lexemes and counting lines.
**********************************************************************/

%option noyywrap
%{
#include "lexer.h"

// global variable to hold current line number being read
int line_number = 1;
%}

 /* Cleaner way of specifying variable name */
F_CHAR   [a-zA-Z_]
S_CHAR   [a-zA-Z_0-9]

%%

 /* Integer constant */
[+-]?[0-9][0-9]*        { return INT_CONSTANT; }

 /* C-style variable name */
 /* [a-zA-Z_][a-zA-Z_0-9]*  { return VARIABLE_NAME; } */

 /* C-style variable name */
{F_CHAR}{S_CHAR}*       { return VARIABLE_NAME; }

 /* Eat any whitespace, will need to be modified to count lines */
 /* Includes carriage return */
[ \t\r]+

 /* Count lines */
\n                      { line_number++; }

 /* Found an unknown character */
.*                      { return UNKNOWN_TOKEN; }

 /* Recognize end of file */
<<EOF>>                 { return EOF_TOKEN; }


 /* A point of this file is to show the effect of different text file
    encodings on flex-formal regular expressions.  The test* files
    illustrate this.  Relevant commands:
    
    EMACS coding commands:
      % Open file without any decoding:
      M-x find-file-literally

      % List all available coding systems:    
      list-coding-systems            

      % See definition of current coding (it's a variable):
      C-h v buffer-file-coding-system    

      % See current coding and other choices:    
      describe-coding-system

      % Complex way to set coding:
      C-x C-m c universal-coding-system-argument  

    TERMINAL:    
      xxd hex viewer
 */

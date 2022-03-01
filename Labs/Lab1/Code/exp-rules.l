/*********************************************************************
  Name: Any Student             NetID: as3
  Course: CSE 4714              
  Lab: 01 - Lexical Analyzer 
  Purpose of File: Contains the ....


This is the file that you should modify for Lab 1.
**********************************************************************/

%option noyywrap
%{
#include "lexer.h"

// global variable to hold current line number being read
int line_number = 1;
%}

%%


 /* Eat any whitespace, will need to be modified to count lines */

[ \t\r\n]+             /* nop */


 /* Found an unknown character */

.         { return UNKNOWN_TOKEN; }

 /* Recognize end of file */

<<EOF>>   { return EOF_TOKEN; }



/*********************************************************************
Name: Andrew Blake Berry NetID: abb576
Course: CSE 4714 Assignment: Part 2
Programming Environment: VS Code
Purpose of File: Contains the parsing code
*********************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include <iostream>

extern set<string> symbolTable;
int nextToken = 0;  // token returned from yylex
int level = 0;  // used to indent output to approximate parse tree


extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
void program();
bool is_block();
bool is_statement();
bool is_assignment_statement();
bool is_compound_statement();
bool is_if_statement();
bool is_while_statement();
bool is_read_statement();
bool is_write_statement();
bool is_expression();
bool is_simple_expression();
bool is_term();
bool is_factor();

// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
bool first_of_program();
bool first_of_factor();



inline void indent(){
    for (int i = 0; i<level; i++)
        cout << ("    ");
}

void output(){
    indent();
    cout << "-->found " << yytext << endl;
    nextToken = yylex();
}

void enter(string type) {
    indent();
    cout << "enter <" << type << ">"  << endl;
    ++level;
}

void exit(string type) {
    --level;
    indent();
    cout << "exit <" << type << ">" << endl;
}

void program() {

    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    
    indent();
    cout << "enter <program>" << endl;
    ++level;

    output();
    if (nextToken == TOK_IDENT) {
        output();
        if (nextToken == TOK_SEMICOLON) {
            output();
            if (!is_block())
                return;
        }
        else
            throw "14: ';' expected";
    }
    else
        throw "2: identifier expected";

    --level;
    indent();
    cout << "exit <program>" << endl;
    
}

bool is_block() {
    enter("block");
    bool out;

    if (nextToken != TOK_VAR && nextToken != TOK_BEGIN)
        throw "18: error in declaration part OR 17: 'BEGIN' expected";

    if (nextToken == TOK_VAR) {
        output();
        if (nextToken == TOK_IDENT) {
            do {
                if (symbolTable.find(yytext) == symbolTable.end())
                    symbolTable.insert(yytext);
                else
                    throw "101: identifier declared twice";
                output();
                if (nextToken == TOK_COLON) {
                    output();
                    if (nextToken == TOK_INTEGER || nextToken == TOK_REAL) {
                        output();
                        if (nextToken == TOK_SEMICOLON) {
                            output();
                        }
                        else {
                            throw "14: ';' expected";
                        }
                    }
                    else {
                        throw "10: error in type";
                    }
                }
                else {
                    throw "5: ':' expected";
                    break;
                }
                cout << endl;
            } while (nextToken == TOK_IDENT);
        }
        else
            throw "2: identifier expected";
    }

    if (is_compound_statement()) {
        out = true;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("block");
    return true;
}

bool is_assignment_statement() {
    bool out;
    if (nextToken == TOK_IDENT) {
        enter("assignment");
        output();
        if (nextToken == TOK_ASSIGN) {
            output();
            if (is_expression())
                out = true;
            else
                out = false;
        }
        else
            throw "51: ':=' expected";
    }
    else
        throw "2: identifier expected";

    if (!out)
        return false;

    exit("assignment");
    return out;
}

bool is_compound_statement() {
    bool out;
    if (nextToken == TOK_BEGIN) {
        enter("compound_statement");
        output();
        
        if (is_statement()) {
            while (nextToken == TOK_SEMICOLON) {
                output();
                if (!is_statement()) {
                    out = false;
                    break;
                }
            }
            if (nextToken == TOK_END) {
                output();
                out = true;
            }
            else
                throw "13: 'END' expected";
        }
        else
            out = false;
    }
    else
        throw "17: 'BEGIN' expected";

    if (!out)
        return false;

    exit("compound_statement");
    return out;
}

bool is_if_statement() {
    bool out;
    if (nextToken == TOK_IF) {
        enter("if statement");
        output();
        if (is_expression()) {
            if (nextToken == TOK_THEN) {
                output();
                if (is_statement()) {
                    if (nextToken == TOK_ELSE) {
                        output();
                        if (is_statement())
                            out = true;
                    }
                    else
                        out = true;
                }
                else
                    out = false;
            }
            else
                throw "52: 'THEN' expected";
        }
        else
            out = false;
    }

    if (!out)
        return false;

    exit("if statement");
    return out;
}

bool is_while_statement() {
    bool out;
    if (nextToken == TOK_WHILE) {
        enter("while statement");
        output();
        if (is_expression()) {
            if (is_statement())
                out = true;
            else
                out = false;
        }
        else
            out = false;
    }

    if (!out)
        return false;

    exit("while statement");
    return out;
}

bool is_read_statement() {
    bool out;
    if (nextToken == TOK_READ) {
        enter("read");
        output();
        if (nextToken == TOK_OPENPAREN) {
            output();
            if (nextToken == TOK_IDENT) {
                output();
                if (nextToken == TOK_CLOSEPAREN) {
                    output();
                    out = true;
                }
                else
                    throw "4: ')' expected";
            }
            else
                throw "2: identifier expected";
        }
        else
            throw "9: '(' expected";
    }

    if (!out)
        return false;

    exit("read");
    return out;
}

bool is_write_statement() {
    bool out;

    if (nextToken == TOK_WRITE) {
        enter("write");
        output();
        if (nextToken == TOK_OPENPAREN) {
            output();
            if (nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT) {
                output();
                if (nextToken == TOK_CLOSEPAREN) {
                    output();
                    out = true;
                }
                else
                    throw "4: ')' expected";
            }
            else
                throw "134: illegal type of operand(s)";
        }
        else
            throw "9: '(' expected";
    }

    if (!out)
        return false;

    exit("write");
    return out;
}

bool is_statement() {
    enter("statement");
    bool out;

    switch (nextToken) {
        case TOK_IDENT:
            out = is_assignment_statement();
            break;
        case TOK_BEGIN:
            out = is_compound_statement();
            break;
        case TOK_IF:
            out = is_if_statement();
            break;
        case TOK_WHILE:
            out = is_while_statement();
            break;
        case TOK_READ:
            out = is_read_statement();
            break;
        case TOK_WRITE:
            out = is_write_statement();
            break;
        default:
            throw "900: illegal type of statement";
            break;
    }

    if (!out)
        return false;

    exit("statement");
    return out;
}

bool is_factor() {
    enter("factor");
    if (!first_of_factor())
        throw "903: illegal type of factor";

    bool out;
    if (nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT) {
        output();
        out = true;
    }
    else if (nextToken == TOK_IDENT && symbolTable.find(yytext) != symbolTable.end()) {
        output();
        out = true;
    }
    else if (nextToken == TOK_IDENT) {
        throw "104: identifier not declared";
    }
    else if (nextToken == TOK_OPENPAREN) {
        output();
        if (is_expression()) {
            if (nextToken == TOK_CLOSEPAREN) {
                output();
                out = true;
            }
            else
                throw "4: ')' expected";
        }
        else
            out = false;
    }
    else if (nextToken == TOK_NOT || nextToken == TOK_MINUS) {
        output();
        if (is_factor())
            out = true;
        else
            out = false;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("factor");
    return out;
}

bool is_expression() {
    enter("expression");
    if (!first_of_factor())
        throw "144: illegal type of expression";

    bool out;
    if (is_simple_expression()) {
        if (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO) {
            output();
            if (is_simple_expression()) {
                out = true;
            }
            else
                out = false;
        }
        else
            out = true;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("expression");
    return out;
}

bool is_simple_expression() {
    enter("simple expression");
    if (!first_of_factor())
        throw "901: illegal type of simple expression";

    bool out;
    if (is_term()) {
        if (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
            output();
            if (is_term())
                out = true;
            else
                out = false;
        }
        else
            out = true;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("simple expression");
    return out;
}

bool is_term() {
    enter("term");
    if (!first_of_factor())
        throw "902: illegal type of term";

    bool out;
    if (is_factor()) {
        if (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) {
            do {
                output();
                if (is_factor()) {
                    out = true;
                }
                else
                    out = false;
            } while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND);
        }
        else
            out = true;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("term");
    return out;
}


//*****************************************************************************

bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}

bool first_of_factor() {
    return nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || 
        nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || 
        nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

#endif

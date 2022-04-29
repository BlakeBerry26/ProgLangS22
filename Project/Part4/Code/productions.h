/*********************************************************************
Name: Andrew Blake Berry NetID: abb576
Course: CSE 4714 Assignment: Part 4
Programming Environment: VS Code
Purpose of File: Contains the parsing code
*********************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include "parse_tree_nodes.h"
#include <iostream>

extern map<string, int> symbolTable;
int nextToken = 0;  // token returned from yylex
int level = 0;  // used to indent output to approximate parse tree


extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
void program(ProgramNode *&p);
bool is_block(BlockNode *&block);
bool is_statement(StatementNode *&statement);
bool is_assignment_statement(StatementNode *&statement);
bool is_compound_statement(StatementNode *&compound);
bool is_if_statement(StatementNode *&statement);
bool is_while_statement(StatementNode *&statement);
bool is_read_statement(StatementNode *&statement);
bool is_write_statement(StatementNode *&statement);
bool is_expression(ExprNode *&expression);
bool is_simple_expression(SimpleExprNode *&simple);
bool is_term(TermNode *&term);
bool is_factor(FactorNode *&factor);

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

void program(ProgramNode *&p) {

    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    
    indent();
    cout << "enter <program>" << endl;
    delete p;
    ProgramNode* program = new ProgramNode();
    ++level;

    output();
    if (nextToken == TOK_IDENT) {
        program->name = new string(yytext);
        output();
        if (nextToken == TOK_SEMICOLON) {
            output();
            if (!is_block(program->blockNode))
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
    p = program;
}

bool is_block(BlockNode *&block) {
    enter("block");
    bool out;
    delete block;
    block = new BlockNode();

    if (nextToken != TOK_VAR && nextToken != TOK_BEGIN)
        throw "18: error in declaration part OR 17: 'BEGIN' expected";

    if (nextToken == TOK_VAR) {
        output();
        if (nextToken == TOK_IDENT) {
            do {
                if (symbolTable.find(yytext) == symbolTable.end())
                    symbolTable.insert({yytext, 0});
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

    StatementNode* compound = nullptr;
    if (is_compound_statement(compound)) {
        out = true;
    }
    else
        out = false;

    if (!out)
        return false;

    exit("block");
    block->compound = compound;
    return true;
}

bool is_assignment_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    AssignmentStatement* assign = new AssignmentStatement();
    if (nextToken == TOK_IDENT  && symbolTable.find(yytext) != symbolTable.end()) {
        enter("assignment");
        assign->name = new string(yytext);
        output();
        if (nextToken == TOK_ASSIGN) {
            output();
            if (is_expression(assign->expression))
                out = true;
            else
                out = false;
        }
        else
            throw "51: ':=' expected";
    }
    else if (nextToken == TOK_IDENT) {
        enter("assignment");
        throw "104: identifier not declared";
    }
    else
        throw "2: identifier expected";

    if (!out)
        return false;

    exit("assignment");
    statement = assign;
    return out;
}

bool is_compound_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    CompoundStatementNode* compound = new CompoundStatementNode();
    if (nextToken == TOK_BEGIN) {
        enter("compound_statement");
        output();
        
        compound->statements.push_back(nullptr);
        if (is_statement(compound->statements.back())) {
            while (nextToken == TOK_SEMICOLON) {
                output();
                compound->statements.push_back(nullptr);
                if (!is_statement(compound->statements.back())) {
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
    statement = compound;
    return out;
}

bool is_if_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    IfStatement* ifttt = new IfStatement();
    ExprNode* expression = new ExprNode();
    StatementNode* then = nullptr;
    StatementNode* els = nullptr;
    if (nextToken == TOK_IF) {
        enter("if statement");
        output();
        if (is_expression(expression)) {
            if (nextToken == TOK_THEN) {
                output();
                if (is_statement(then)) {
                    if (nextToken == TOK_ELSE) {
                        output();
                        if (is_statement(els)) {
                            ifttt->else_exists = true;
                            out = true;
                        }
                        else {
                            ifttt->else_exists = false;
                        }
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
    ifttt->expression = expression;
    ifttt->then = then;
    ifttt->els = els;
    statement = ifttt;
    return out;
}

bool is_while_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    WhileStatement* whil = new WhileStatement();
    ExprNode* expression = new ExprNode();
    StatementNode* loop = nullptr;
    if (nextToken == TOK_WHILE) {
        enter("while statement");
        output();
        if (is_expression(expression)) {
            if (is_statement(loop))
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
    whil->expression = expression;
    whil->loop = loop;
    statement = whil;
    return out;
}

bool is_read_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    ReadStatement* read = new ReadStatement();
    if (nextToken == TOK_READ) {
        enter("read");
        output();
        if (nextToken == TOK_OPENPAREN) {
            output();
            if (nextToken == TOK_IDENT) {
                read->ident = new string(yytext);
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
    statement = read;
    return out;
}

bool is_write_statement(StatementNode *&statement) {
    bool out;
    delete statement;
    WriteStatement* write = new WriteStatement();

    if (nextToken == TOK_WRITE) {
        enter("write");
        output();
        if (nextToken == TOK_OPENPAREN) {
            output();
            if (nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT) {
                write->ident = new string(yytext);
                if (nextToken == TOK_IDENT)
                    write->type = false;
                else
                    write->type = true;
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
    statement = write;
    return out;
}

bool is_statement(StatementNode *&statement) {
    enter("statement");
    bool out;

    switch (nextToken) {
        case TOK_IDENT:
            out = is_assignment_statement(statement);
            break;
        case TOK_BEGIN:
            out = is_compound_statement(statement);
            break;
        case TOK_IF:
            out = is_if_statement(statement);
            break;
        case TOK_WHILE:
            out = is_while_statement(statement);
            break;
        case TOK_READ:
            out = is_read_statement(statement);
            break;
        case TOK_WRITE:
            out = is_write_statement(statement);
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

bool is_factor(FactorNode *&factor) {
    enter("factor");
    if (!first_of_factor())
        throw "903: illegal type of factor";

    bool out;
    if (nextToken == TOK_INTLIT) {
        factor = new IntLitNode(yytext);
        output();
        out = true;
    }
    else if (nextToken == TOK_FLOATLIT) {
        factor = new FloatLitNode(yytext);
        output();
        out = true;
    }
    else if (nextToken == TOK_IDENT && symbolTable.find(yytext) != symbolTable.end()) {
        factor = new IdNode(yytext);
        output();
        out = true;
    }
    else if (nextToken == TOK_IDENT) {
        throw "104: identifier not declared";
    }
    else if (nextToken == TOK_OPENPAREN) {
        output();
        ExprNode* expr = new ExprNode();
        if (is_expression(expr)) {
            NestedExprNode* temp = new NestedExprNode(expr);
            factor = temp;
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
        bool type = nextToken == TOK_MINUS;
        output();
        FactorNode* f = new IdNode("x");
        if (is_factor(f)) {
            factor = new NegativeNotFactor(f, type);
            out = true;
        }
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

bool is_expression(ExprNode *&expression) {
    enter("expression");
    delete expression;
    expression = new ExprNode();
    if (!first_of_factor())
        throw "144: illegal type of expression";

    bool out;
    if (is_simple_expression(expression->firstSimple)) {
        if (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO) {
            expression->restTermOps.push_back(nextToken);
            output();
            expression->restSimple.push_back(new SimpleExprNode());
            if (is_simple_expression(expression->restSimple.back())) {
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

bool is_simple_expression(SimpleExprNode *&simple) {
    enter("simple expression");
    delete simple;
    simple = new SimpleExprNode();
    if (!first_of_factor())
        throw "901: illegal type of simple expression";

    bool out;
    if (is_term(simple->firstTerm)) {
        if (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
            simple->restTermOps.push_back(nextToken);
            output();
            simple->restTerm.push_back(new TermNode());
            if (is_term(simple->restTerm.back()))
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

bool is_term(TermNode *&term) {
    enter("term");
    delete term;
    term = new TermNode();
    if (!first_of_factor())
        throw "902: illegal type of term";

    bool out;
    if (is_factor(term->firstFactor)) {
        if (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) {
            do {
                term->restFactorOps.push_back(nextToken);
                output();
                term->restFactors.push_back(nullptr);
                if (is_factor(term->restFactors.back())) {
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

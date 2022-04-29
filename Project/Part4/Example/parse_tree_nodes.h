//*****************************************************************************
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
//          added interpret methods for each node type
// version: Spring 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

// Forward declaration of <expr> node
class ExprNode; 

// Forward declaration of operator<< for ExprNode
ostream& operator<<(ostream& os, ExprNode& en);

// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
class FactorNode {
public:
    virtual int interpret() = 0;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode(); // labeling the destructor as virtual allows 
                           // the subclass destructors to be called
};

FactorNode::~FactorNode() {}

// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
    os << "(Factor: ";
    fn.printTo(os);
    os << ") ";
    return os;
}


class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(string name);
    ~IdNode();
    void printTo(ostream & os);
    int interpret();
};

IdNode::IdNode(string name) {
    id = new string(name);
}

IdNode::~IdNode() {
    cout << "Deleting FactorNode:IdNode: " << *id << endl;
    delete id;
    id = nullptr;
}

void IdNode::printTo(ostream& os) {
    os << "(IDENT: " << *id << ") ";
}

int IdNode::interpret(){
    return 1;    // use 1 for any identifier (in this example only)
                 // look up the current value in a symbol table
                 //       in your interpreter
} 

class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int value);
    ~IntLitNode();
    void printTo(ostream & os);
    int interpret();
};

IntLitNode::IntLitNode(int value) {
    int_literal = value;
}

IntLitNode::~IntLitNode() {
    cout << "Deleting FactorNode:IntLitNode: " << int_literal << endl;
    // Nothing to do since the only member variable is not a pointer
}

void IntLitNode::printTo(ostream& os) {
    os << "(INTLIT: " << int_literal << ") ";
}

int IntLitNode::interpret(){
    return int_literal;   
}


class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(ExprNode* en);
    void printTo(ostream & os);
    int interpret();
    ~NestedExprNode();
};

NestedExprNode::NestedExprNode(ExprNode* en) {
    exprPtr = en;
}

void NestedExprNode::printTo(ostream& os) {
    os << *exprPtr;
}


class TermNode {
public:
    FactorNode* firstFactor = nullptr;
    vector<int> restFactorOps; // TOK_MULT_OP or TOK_DIV_OP
    vector<FactorNode*> restFactors;
    
    int interpret();
    ~TermNode();
};

ostream& operator<<(ostream& os, TermNode& tn) {
    os << "(Term: ";
    os << *(tn.firstFactor);

    int length = tn.restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        int op = tn.restFactorOps[i];
        if (op == TOK_MULT_OP)
            os << "* ";
        else
            os << "/ ";
        os << *(tn.restFactors[i]);
    }
    os << ") ";
    return os;
}

int TermNode::interpret(){
    // a Term is a Factor followed by 0 or more Factors separated by * or / operations

    // get the value of the first Factor
    int returnValue = firstFactor->interpret();

    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Factor
        int nextValue = restFactors[i]->interpret();

        // perform the operation (* or /) that separates the Factors
        if (restFactorOps[i] == TOK_MULT_OP)
            returnValue = returnValue * nextValue;
        else
            returnValue = returnValue / nextValue;
    }

    return returnValue;
}

TermNode::~TermNode() {
    cout << "Deleting TermNode: " << *this << endl;
    delete firstFactor;
    firstFactor = nullptr;

    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        delete restFactors[i];
        restFactors[i] = nullptr;
    }
}


class ExprNode {
public:
    TermNode* firstTerm = nullptr;
    vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
    vector<TermNode*> restTerms;
    
    int interpret();
    ~ExprNode();
};

ostream& operator<<(ostream& os, ExprNode& en) {
    os << "(Expr: ";
    os << *(en.firstTerm);

    int length = en.restTermOps.size();
    for (int i = 0; i < length; ++i) {

        int op = en.restTermOps[i];
        if (op == TOK_ADD_OP)
            os << "+ ";
        else
            os << "- ";
        os << *(en.restTerms[i]);
    }
    os << ") ";
    return os;
}

int ExprNode::interpret(){
    // an Expr is a Term followed by 0 or more Terms separated by + or - operations

    // get the value of the first Term
    int returnValue = firstTerm->interpret();

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Term
        int nextValue = restTerms[i]->interpret();

        // perform the operation (+ or -) that separates the Terms
        if (restTermOps[i] == TOK_ADD_OP)
            returnValue = returnValue + nextValue;
        else
            returnValue = returnValue - nextValue;
    }

    return returnValue;
}

ExprNode::~ExprNode() {
    cout << "Deleting ExprNode: " << *this << endl;
    delete firstTerm;
    firstTerm = nullptr;

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        delete restTerms[i];
        restTerms[i] = nullptr;
    }
}


int NestedExprNode::interpret(){
    return exprPtr->interpret();
}

NestedExprNode::~NestedExprNode() {
    cout << "Deleting FactorNode:NestedExprNode: " << *exprPtr << endl;
    delete exprPtr;
    exprPtr = nullptr;
}

#endif /* PARSE_TREE_NODES_H */

/*********************************************************************
Name: Andrew Blake Berry NetID: abb576
Course: CSE 4714 Assignment: Part 4
Programming Environment: VS Code
Purpose of File: Node classes used while building a parse tree for
//              the arithmetic expression
*********************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern map<string, int> symbolTable;

// Forward declaration of <expr> node
class ExprNode; 
class CompoundStatementNode;
class BlockNode;
class ProgramNode;

// Forward declaration of operator<< for ExprNode
ostream& operator<<(ostream& os, ExprNode& en);
ostream& operator<<(ostream& os, CompoundStatementNode& cn);
ostream& operator<<(ostream& os, BlockNode& bn);
ostream& operator<<(ostream& os, ProgramNode& pn);

//*****************************************************************************
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
class FactorNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
	virtual int interpret();
    virtual ~FactorNode(); // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};

FactorNode::~FactorNode() {}
int FactorNode::interpret() {}

// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
	fn.printTo(os);
	return os;
}

//*****************************************************************************
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(string name);
	int interpret();
    ~IdNode();
    void printTo(ostream & os);
};

int IdNode::interpret() {
	return symbolTable.find(*id)->second;
}

IdNode::IdNode(string name) {
	id = new string(name);
}

IdNode::~IdNode() {
	cout << "Deleting a factorNode" << endl;
	delete id;
	id = nullptr;
}

void IdNode::printTo(ostream& os) {
	os << "factor( ";
	os << *id; 
	os << " ) ";
}

//*****************************************************************************
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(string value);
	int interpret();
    ~IntLitNode();
    void printTo(ostream & os);
};

IntLitNode::IntLitNode(string value) {
	int_literal = stoi(value);
}

int IntLitNode::interpret() {
	return int_literal;
}

IntLitNode::~IntLitNode() {
	cout << "Deleting a factorNode" << endl;
	// Nothing to do since the only member variable is not a pointer
}

void IntLitNode::printTo(ostream& os) {
	os << "factor( ";
	os << int_literal;
	os << " ) ";
}

//*****************************************************************************
// class FloatLitNode (Integer Literal Node)
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0;

    FloatLitNode(string value);
	int interpret();
    ~FloatLitNode();
    void printTo(ostream & os);
};

FloatLitNode::FloatLitNode(string value) {
	float_literal = std::stof(value);
}

int FloatLitNode::interpret() {
	return int(float_literal);
}

FloatLitNode::~FloatLitNode() {
	cout << "Deleting a factorNode" << endl;
	// Nothing to do since the only member variable is not a pointer
}

void FloatLitNode::printTo(ostream& os) {
	os << "factor( ";
	os << float_literal;
	os << " ) ";
}

class NegativeNotFactor : public FactorNode {
public:
	bool type; // True: Minus; False: Not
    FactorNode* factorPtr = nullptr;

	NegativeNotFactor(FactorNode* en, bool type);
	int interpret();
    void printTo(ostream & os);
    ~NegativeNotFactor();
};

NegativeNotFactor::NegativeNotFactor(FactorNode* fn, bool t) {
	type = t;
	factorPtr = fn;
}

int NegativeNotFactor::interpret() {
	if (type) {
		return factorPtr->interpret() * -1;
	}
	else {
		return !factorPtr->interpret();
	}
}

void NegativeNotFactor::printTo(ostream& os) {
	os << "factor( ";
	if (type)
		os << "- ";
	else
		os << "NOT ";
	os << *factorPtr;
	os << ") ";
}

NegativeNotFactor::~NegativeNotFactor() {
	cout << "Deleting a factorNode" << endl;
	delete factorPtr;
	factorPtr = nullptr;
}

//*****************************************************************************
// class TermNode (Terminal Node)
class TermNode {
public:
    FactorNode* firstFactor = nullptr;
    vector<int> restFactorOps; // TOK_MULTIPLY, TOK_DIVIDE, or TOK_AND
    vector<FactorNode*> restFactors;

	int interpret();
    ~TermNode();
};

int TermNode::interpret() {
	// get the value of the first Term
    int returnValue = firstFactor->interpret();

    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Term
        int nextValue = restFactors[i]->interpret();

        // perform the operation (*, /, or AND) that separates the Terms
        if (restFactorOps[i] == TOK_MULTIPLY)
            returnValue = returnValue * nextValue;
        else if (restFactorOps[i] == TOK_DIVIDE)
            returnValue = returnValue / nextValue;
		else if (restFactorOps[i] == TOK_AND)
            returnValue = returnValue && nextValue;
    }

    return returnValue;
}

ostream& operator<<(ostream& os, TermNode& tn) {
	os << "term( ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
		switch (op) {
			case TOK_MULTIPLY:
				os << "* ";
				break;
			case TOK_DIVIDE:
				os << "/ ";
				break;
			case TOK_AND:
				os << "AND ";
				break;
		}
		os << *(tn.restFactors[i]);
	}
	os << ") ";
	return os;
}

TermNode::~TermNode() {
	if (firstFactor != nullptr)
		cout << "Deleting a termNode"<< endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}

//*****************************************************************************
// class ExprNode (Expression Node)
class SimpleExprNode {
public:
    TermNode* firstTerm = nullptr;
    vector<int> restTermOps; // TOK_PLUS, TOK_MINUS, and TOK_OR
    vector<TermNode*> restTerm;

	int interpret();
    ~SimpleExprNode();
};

int SimpleExprNode::interpret() {
    // get the value of the first Term
    int returnValue = firstTerm->interpret();

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Term
        int nextValue = restTerm[i]->interpret();

        // perform the operation (+, -, or OR) that separates the Terms
        if (restTermOps[i] == TOK_PLUS)
            returnValue = returnValue + nextValue;
        else if (restTermOps[i] == TOK_MINUS)
            returnValue = returnValue - nextValue;
		else if (restTermOps[i] == TOK_OR)
            returnValue = returnValue || nextValue;
    }

    return returnValue;
}

ostream& operator<<(ostream& os, SimpleExprNode& sen) {
	os << "simple_expression( ";
	os << *(sen.firstTerm);

	int length = sen.restTermOps.size();
	for (int i = 0; i < length; ++i) {

		int op = sen.restTermOps[i];
		switch (op) {
			case TOK_PLUS:
				os << "+ ";
				break;
			case TOK_MINUS:
				os << "- ";
				break;
			case TOK_OR:
				os << "OR ";
				break;
		}
		os << *(sen.restTerm[i]);
	}
	os << ") ";
	return os;
}

SimpleExprNode::~SimpleExprNode() {
	if (firstTerm != nullptr)
		cout << "Deleting a simpleExpressionNode" << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerm[i];
		restTerm[i] = nullptr;
	}
}

//*****************************************************************************
// class ExprNode (Expression Node)
class ExprNode {
public:
    SimpleExprNode* firstSimple = nullptr;
    vector<int> restTermOps; // TOK_EQUALTO, TOK_LESSTHAN, TOK_GREATERTHAN, and TOK_NOTEQUALTO
    vector<SimpleExprNode*> restSimple;

	int interpret();
    ~ExprNode();
};

int ExprNode::interpret() {
    // get the value of the first Term
    int returnValue = firstSimple->interpret();

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Term
        int nextValue = restSimple[i]->interpret();

        // perform the operation (==, <, >, or !=) that separates the Terms
        if (restTermOps[i] == TOK_EQUALTO)
            returnValue = returnValue == nextValue;
        else if (restTermOps[i] == TOK_LESSTHAN)
            returnValue = returnValue < nextValue;
		else if (restTermOps[i] == TOK_GREATERTHAN)
            returnValue = returnValue > nextValue;
		else if (restTermOps[i] == TOK_NOTEQUALTO)
            returnValue = returnValue != nextValue;
    }

    return returnValue;
}

ostream& operator<<(ostream& os, ExprNode& en) {
	os << "expression( ";
	os << *(en.firstSimple);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) {

		int op = en.restTermOps[i];
		switch (op) {
			case TOK_EQUALTO:
				os << "= ";
				break;
			case TOK_LESSTHAN:
				os << "< ";
				break;
			case TOK_GREATERTHAN:
				os << "> ";
				break;
			case TOK_NOTEQUALTO:
				os << "<> ";
				break;
		}
		os << *(en.restSimple[i]);
	}
	os << ")";
	return os;
}

ExprNode::~ExprNode() {
	if (firstSimple != nullptr)
		cout << "Deleting an expressionNode" << endl;
	delete firstSimple;
	firstSimple = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restSimple[i];
		restSimple[i] = nullptr;
	}
}

//*****************************************************************************
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

	NestedExprNode(ExprNode* e);
	int interpret();
    void printTo(ostream & os);
    ~NestedExprNode();
};

NestedExprNode::NestedExprNode(ExprNode* e) {
	exprPtr = e;
}

int NestedExprNode::interpret() {
	return exprPtr->interpret();
}

void NestedExprNode::printTo(ostream& os) {
	os << "nested_expression( ";
	os << *exprPtr;
	os << " ) ";
}

NestedExprNode::~NestedExprNode() {
	cout << "Deleting a factorNode" << endl;
	delete exprPtr;

	exprPtr = nullptr;
}

//*****************************************************************************
// Abstract class.
class StatementNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
	virtual void interpret();
    virtual ~StatementNode(); // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};

StatementNode::~StatementNode() {}
void StatementNode::interpret() {}

ostream& operator<<(ostream& os, StatementNode& sn) {
	sn.printTo(os);
	return os;
}

class AssignmentStatement : public StatementNode {
public:
    string* name;
	ExprNode* expression;

	void interpret();
    ~AssignmentStatement();

	void printTo(ostream &os) {
        os << "Assignment " << *name << " := ";
		os << *expression << endl;
    }
};

void AssignmentStatement::interpret() {
	int result = expression->interpret();
	symbolTable.erase(*name);
	symbolTable.insert({*name, result});
}

AssignmentStatement::~AssignmentStatement() {
	cout << "Deleting an assignmentNode" << endl;
	delete expression;
	expression = nullptr;
	delete name;
	name = nullptr;
}

class IfStatement : public StatementNode {
public:
	ExprNode* expression;
	StatementNode* then;
	StatementNode* els;
	bool else_exists;

	void interpret();
    ~IfStatement();

	void printTo(ostream &os) {
        os << "If " << *expression << endl;
		os << "%%%%%%%% True Statement %%%%%%%%" << endl;
		os << *then;
		os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
		if (else_exists) {
			os << "%%%%%%%% False Statement %%%%%%%%" << endl;
			os << *els;
			os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
		}
    }
};

void IfStatement::interpret() {
	if (expression->interpret() != 0) {
		then->interpret();
	}
	else if (else_exists) {
		els->interpret();
	}
}

IfStatement::~IfStatement() {
	cout << "Deleting an ifNode" << endl;
	delete expression;
	expression = nullptr;
	delete then;
	then = nullptr;
	if (else_exists) {
		delete els;
		els = nullptr;
	}
}

class WhileStatement : public StatementNode {
public:
	ExprNode* expression;
	StatementNode* loop;
	
	void interpret();
    ~WhileStatement();

	void printTo(ostream &os) {
        os << "While " << *expression << endl;
		os << "%%%%%%%% Loop Body %%%%%%%%" << endl;
		os << *loop;
		os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    }
};

void WhileStatement::interpret() {
	while (expression->interpret() != 0) {
		loop->interpret();
	}
}

WhileStatement::~WhileStatement() {
	cout << "Deleting a whileNode" << endl;
	delete expression;
	expression = nullptr;
	delete loop;
	loop = nullptr;
}

class ReadStatement : public StatementNode {
public:
	string* ident;

	void interpret();
    ~ReadStatement();

	void printTo(ostream &os) {
        os << "Read Value " << *ident << endl;
    }
};

void ReadStatement::interpret() {
	int read;
	cin >> read;
	symbolTable.erase(*ident);
	symbolTable.insert({*ident, read});
}

ReadStatement::~ReadStatement() {
	cout << "Deleting a readNode" << endl;
	delete ident;
	ident = nullptr;
}

class WriteStatement : public StatementNode {
public:
	bool type;
	string* ident;

	void interpret();
    ~WriteStatement();

	void printTo(ostream &os) {
		if (type)
			os << "Write String " << *ident << endl;
		else
			os << "Write Value " << *ident << endl;
    }
};

void WriteStatement::interpret() {
	if (type) {
		string *out = new string(*ident);
		out->pop_back();
		out->erase(out->begin());
		cout << *out << endl;
		delete out;
	}
	else {
		cout << symbolTable.find(*ident)->second << endl;
	}
}

WriteStatement::~WriteStatement() {
	cout << "Deleting a writeNode" << endl;
	delete ident;
	ident = nullptr;
}

class CompoundStatementNode : public StatementNode {
public:
	vector<StatementNode*> statements;

	void interpret();
    ~CompoundStatementNode();
	
	void printTo(ostream &os) {
        os << *this;
    }
};

void CompoundStatementNode::interpret() {
	int length = statements.size();
    for (int i = 0; i < length; ++i) {
        statements[i]->interpret();
    }
}

CompoundStatementNode::~CompoundStatementNode() {
	cout << "Deleting a compoundNode" << endl;
	int length = statements.size();
	for (int i = 0; i < length; ++i) {
		delete statements[i];
		statements[i] = nullptr;
	}
}

ostream& operator<<(ostream& os, CompoundStatementNode& cn) {
	os << "Begin Compound Statement" << endl;
	int length = cn.statements.size();
	for (int i = 0; i < length; ++i) {
		os << *(cn.statements[i]);
	}
	os << "End Compound Statement" << endl;
	return os;
}

//*****************************************************************************
// class block (Expression Node)
class BlockNode {
public:
    StatementNode* compound;

	void interpret();
    ~BlockNode();
};

void BlockNode::interpret() {
	compound->interpret();
}

BlockNode::~BlockNode() {
	cout << "Deleting a blockNode" << endl;
	delete compound;
	compound = nullptr;
}

ostream& operator<<(ostream& os, BlockNode& bn) {
	os << *(bn.compound);
	return os;
}

//*****************************************************************************
// class program (Expression Node)
class ProgramNode {
public:
    BlockNode* blockNode = nullptr;
	string* name;

	void interpret();
    ~ProgramNode();
};

void ProgramNode::interpret() {
	blockNode->interpret();
}

ProgramNode::~ProgramNode() {
	if (blockNode != nullptr)
		cout << "Deleting a programNode" << endl;
	delete blockNode;
	blockNode = nullptr;
	delete name;
	name = nullptr;
}

ostream& operator<<(ostream& os, ProgramNode& pn) {
	os << "Program Name ";
	os << *(pn.name);
	os << endl;
	os << *(pn.blockNode);
;
	
	return os;
}


#endif /* PARSE_TREE_NODES_H */

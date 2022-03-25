//*****************************************************************************
// purpose:  
//
//  author: 
//*****************************************************************************

#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>

using namespace std;

// Abstract class. Base class for StringNode, IntegerNode, and FloatNode
//
// Do NOT change this class
class DataNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~DataNode(); // labeling the destructor as virtual allows 
	                     // the subclass destructors to be called
};

// Uses double dispatch to call the overloaded method printTo in the 
// DataNodes: StringNode, IntegerNode, and FloatNode
//
// Do NOT change this method
ostream& operator<<(ostream& os, DataNode& fn) {
	os << "Data: ";
	fn.printTo(os);
	return os;
}

// Do NOT change this method
DataNode::~DataNode() {}


class StringNode : public DataNode {
public:
    string* mystring = nullptr;

    // Add constructor, destructor, and printTo methods
    StringNode(string s) {
        mystring = new string();
        *mystring = s;
    }

    ~StringNode() {
        cout << "Deleting DataNode:StringNode:" << *mystring << endl;
        delete mystring;
        mystring = nullptr;
    }

    void printTo(ostream &os) {
        os << "(string: " << *mystring << ") ";
    }
};



class IntegerNode : public DataNode {
public:
    int myinteger = 0;

    // Add constructor, destructor, and printTo methods
    IntegerNode(int i) {
        myinteger = i;
    }

    ~IntegerNode() {
        cout << "Deleting DataNode:IntegerNode:" << myinteger << endl;
    }

    void printTo(ostream &os) {
        os << "(integer: " << myinteger << ") ";
    }
};



class FloatNode : public DataNode {
public:
    float myfloat = 0.0;

    // Add constructor, destructor, and printTo methods
    FloatNode(float f) {
        myfloat = f;
    }

    ~FloatNode() {
        cout << "Deleting DataNode:FloatNode:" << myfloat << endl;
    }

    void printTo(ostream &os) {
        os << "(float: " << myfloat << ") ";
    }
};



#endif /* NODES_H */

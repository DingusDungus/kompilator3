#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>

#include "linked_list.h"

class interpreter
{
private:
    std::ifstream byteCode;
    double lhs;
    double rhs;
    double temp;
    bool isLhs;

    void parseLine(std::string line);
    linked_list entries;
    std::vector<std::string> lineVector;

    bool parse(std::string line);

    int addOp();
    int subOp();
    int mulOp();
    int divOp();
    int andOp();
    int orOp();
    int notOp();
    int ltOp();
    int gtOp();
    int eqOp();
    void print();
    void load();
    void iconst();
    void store();
    void stop();

    void printStack();

    void pop();
    void put();
    void pop_element(std::string name);

public:
    interpreter();
    void interpret();
};

#endif

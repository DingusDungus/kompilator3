#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>

struct stackEntry
{
    stackEntry() : varName(""), value(0) {}
    stackEntry(std::string name, int value) : varName(name), value(value) {}
    std::string varName;
    double value;
};

class interpreter
{
private:
    std::ifstream byteCode;
    double lhs;
    double rhs;
    double temp;
    bool isLhs;

    void parseLine(std::string line);
    std::vector<stackEntry *> entries;
    std::vector<std::string> lineVector;

    bool parse(std::string line);

    stackEntry *searchStack(std::string name);

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

    void pop();
    void put();
    void pop_element(std::string name);

public:
    interpreter();
    void interpret();
};

#endif
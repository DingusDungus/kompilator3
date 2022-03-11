#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "scope.h"
#include "Node.h"
#include <iostream>
#include <vector>

struct duplicate
{
    record *duplicateScope;
    record *duplicateRecord;
    duplicate() { duplicateScope = nullptr; duplicateRecord = nullptr; }
};

class symbolTable
{
private:
    Node *nodeRoot;
    scope *root;
    scope *current;
    std::vector<std::string> expressionElements;
    void stBuilderRec(Node *walker, Node *parent);
    void stBuilder();
    void declared(std::string key);
    bool expressionCheck();
    bool expressionCheckRec(Node *nodePtr);
    bool expressionCheckRecNode(Node *nodePtr);
    void addParameters(Node *ptr, method *scopeRecord);
    std::string getTypeLiteralExpression(std::string literal);
    bool testType(Node *ptr, std::string type);
    bool testType(Node *ptr);
    bool testTypeIdentifier(Node *ptr);
    method *methodLookup(std::string className, std::string methodName);
    std::vector<std::string> getParams(Node *ptr, std::vector<std::string> params);
    std::string getType(Node *ptr);
    bool testMethodCallParams(Node *ptr);
    bool testBoolExpression(Node *ptr);
    bool isBoolChildren(Node *ptr);
    bool equalsTestType(Node *ptr);
    bool testBoolEquals(Node *ptr);
    bool varIsBool(Node *ptr);
    bool isExpression(Node *ptr);

public:
    symbolTable();
    void enterScope();
    bool typeCheck();
    void exitScope();
    void init(Node *root);
    void printSTtree();
    void resetScopes();
    scope *operator[](int index);
    scope *getCurrent();
    int size();
    bool duplicatesFound();
    bool duplicatesFoundRec(scope *ptr);

    void put(std::string key, record *item);
    record *lookup(std::string key);
};

#endif
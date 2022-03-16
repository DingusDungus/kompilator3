#ifndef IRNODE_H
#define IRNODE_H

#include <iostream>
#include <vector>

#include "bblock.h"
#include "Node.h"

struct retStruct
{
    std::string value;
    BBlock *bblock;
    retStruct() : bblock(nullptr) {}
    retStruct(std::string value) : bblock(nullptr), value(value) {}
    retStruct(std::string value, BBlock *bblock) : bblock(bblock), value(value) {}
};

class irNode
{
public:
    irNode();
    virtual ~irNode();

    std::vector<irNode *> child;
    std::string name;
    retStruct lhs;
    retStruct rhs;
    std::string type;

    retStruct virtual genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) = 0;

    std::string genName(std::_List_iterator<Node *> node, BBlock *currentBlk);
    std::string genTempName(BBlock *currentBlk);
    std::string genBlkName();
};

class booleanExpression : public irNode
{
    public:
    booleanExpression() { type = "booleanExpression"; }
    ~booleanExpression() override {}
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class subExpression : public irNode
{
    public:
    subExpression() { type = "SUB"; }
    ~subExpression() override {}
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class addExpression : public irNode
{
    public:
    addExpression() { type = "ADD"; }
    ~addExpression() override {}
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class ifElse : public irNode
{
    public:
    ifElse() { type = "ifElse"; }
    ~ifElse() override {}
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class identifier : public irNode
{
    public:
    ~identifier() override {}
    std::string value;
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class integer : public irNode
{
    public:
    ~integer() override {}
    std::string value;
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

class temp : public irNode
{
    public:
    ~temp() override {}
    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override;
};

#endif

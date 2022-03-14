#ifndef IRNODE_H
#define IRNODE_H

#include <iostream>
#include <vector>

#include <bblock.h>

class irNode 
{
    public:
    irNode() {}

    std::vector<irNode *> child;
    std::string name;
    std::string lhs_name;
    std::string rhs_name;
    std::string type;

    std::string virtual genIr(BBlock *currentBlock);

    std::string genName();
};

class subExpression : public irNode
{
    subExpression() { type = "SUB"; }
    std::string genIr(BBlock *currentBlock) override
    {
        name = genName();
        lhs_name = child[0]->genIr(currentBlock);
        rhs_name = child[1]->genIr(currentBlock);
        tac *in = new expression("-", lhs_name, rhs_name, name);
        currentBlock->instructions.push_back(in);
        return name;
    }
};

class addExpression : public irNode
{
    addExpression() { type = "ADD"; }
    std::string genIr(BBlock *currentBlock) override
    {
        name = genName();
        lhs_name = child[0]->genIr(currentBlock);
        rhs_name = child[1]->genIr(currentBlock);
        tac *in = new expression("+", lhs_name, rhs_name, name);
        currentBlock->instructions.push_back(in);
        return name;
    }
};

class identifier : public irNode
{
    std::string value;
    std::string genIr(BBlock *currentBlock) override
    {
        return value;
    }
};

class integer : public irNode
{
    std::string value;
    std::string genIr(BBlock *currentBlock) override
    {
        return value;
    }
};

class temp: public irNode
{
    std::string genIr(BBlock *currentBlock) override
    {
        std::string tempName = "_" + std::to_string(currentBlock->tempCount);
        currentBlock->tempCount++;
        return tempName;
    }
};

#endif
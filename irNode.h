#ifndef IRNODE_H
#define IRNODE_H

#include <iostream>
#include <vector>

#include "bblock.h"
#include "Node.h"

class irNode
{
public:
    irNode() {}

    std::vector<irNode *> child;
    std::string name;
    std::string lhs_name;
    std::string rhs_name;
    std::string type;

    std::string virtual genIr(std::_List_iterator<Node *> node, BBlock *currentBlock);

    std::string genName(std::_List_iterator<Node *> node, BBlock *currentBlk)
    {
        node--;
        if ((*node)->type == "IdentifierExpression")
        {
            auto child = (*node)->children.begin();
            return (*child)->value;
        }
        else
        {
            return genTempName(currentBlk);
        }
    }
    std::string genTempName(BBlock *currentBlk)
    {
        {
            std::string temp = "_+" + std::to_string(currentBlk->tempCount);
            currentBlk->tempCount++;
            return temp;
        }
    }
};

class subExpression : public irNode
{
    subExpression() { type = "SUB"; }
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        name = genName(node, currentBlock);
        auto childNode = (*node)->children.begin();
        lhs_name = child[0]->genIr(childNode, currentBlock);
        childNode++;
        rhs_name = child[1]->genIr(childNode, currentBlock);
        tac *in = new expression("-", lhs_name, rhs_name, name);
        currentBlock->instructions.push_back(in);
        return name;
    }
};

class addExpression : public irNode
{
    addExpression() { type = "ADD"; }
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        name = genName(node, currentBlock);
        auto childNode = (*node)->children.begin();
        lhs_name = child[0]->genIr(childNode, currentBlock);
        childNode++;
        rhs_name = child[1]->genIr(childNode, currentBlock);
        tac *in = new expression("+", lhs_name, rhs_name, name);
        currentBlock->instructions.push_back(in);
        return name;
    }
};

class ifElse : public irNode
{
    ifElse() { type = "ifElse"; }
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        name = genName(node, currentBlock);
        auto childNode = (*node)->children.begin();
        lhs_name = child[0]->genIr(childNode, currentBlock);
        childNode++;
        rhs_name = child[1]->genIr(childNode, currentBlock);
        tac *in = new expression("+", lhs_name, rhs_name, name);
        currentBlock->instructions.push_back(in);
        return name;
    }
};

class identifier : public irNode
{
    std::string value;
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        return (*node)->value;
    }
};

class integer : public irNode
{
    std::string value;
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        return (*node)->value;
    }
};

class temp : public irNode
{
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        std::string tempName = "_" + std::to_string(currentBlock->tempCount);
        currentBlock->tempCount++;
        return tempName;
    }
};

#endif

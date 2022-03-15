#ifndef CFG_H
#define CFG_H

#include <iostream>
#include <string>
#include <vector>

#include "tac.h"
#include "Node.h"
#include "irNode.h"
#include "bblock.h"

class CFG
{
private:
    int blockNr;
    BBlock *root;
    BBlock *currentBlk;
    std::vector<tac *> temp;
    std::string genBlkName();
    bool isExpressionHeadNode(Node *ptr);
    bool isLeafNode(Node *ptr);
    bool isUnaryHeadNode(Node *ptr);
    std::string tacExpression(Node *ptr);

    Node *nRoot;
    irNode *iRoot;
    std::vector<std::string> postOrderList;
    std::vector<irNode*> postOrderNodes;

public:
    CFG();
    CFG(Node *root);
    void buildCFG();
    void buildCFGrec(Node *ptr);
    void postOrderTraversal(Node *leaf);
    irNode* parseNodes(Node* ptr);
    void printPostOrder();
};

class subExpression : public irNode
{
    public:
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
    public:
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

class ifStmt : public irNode
{
    public:
    ifStmt() { type = "ifStmt"; }
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

class whileStmt : public irNode
{
    public:
    whileStmt() { type = "whileStmt"; }
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
    public:
    std::string value;
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        return (*node)->value;
    }
};

class integer : public irNode
{
    public:
    std::string value;
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        return (*node)->value;
    }
};

class temp : public irNode
{
    public:
    std::string genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
    {
        std::string tempName = "_" + std::to_string(currentBlock->tempCount);
        currentBlock->tempCount++;
        return tempName;
    }
};

#endif

#ifndef BBLOCK_H
#define BBLOCK_H

#include <iostream>
#include <vector>

#include "tac.h"
#include "Node.h"
#include "irNode.h"

class BBlock
{
public:
    std::string name;
    std::vector<tac *> instructions;
    tac *condition;
    int tempCount;
    BBlock *trueExit, *falseExit;
    BBlock() : trueExit(nullptr), falseExit(nullptr), tempCount(0) {}
    BBlock(std::string name) : name(name), trueExit(nullptr), falseExit(nullptr), tempCount(0) {}
    BBlock(std::string name, BBlock *trueExit, BBlock *falseExit) : name(name), trueExit(trueExit), falseExit(falseExit), tempCount(0) {}
};

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

public:
    CFG();
    CFG(Node *root);
    void buildCFG();
    void buildCFGrec(Node *ptr);
};

#endif
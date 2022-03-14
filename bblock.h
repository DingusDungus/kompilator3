#ifndef BBLOCK_H
#define BBLOCK_H

#include <iostream>
#include <vector>

#include "tac.h"
#include "Node.h"

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
    tac *currentTac;
    std::string genBlkName();
    std::string genTempName();
    bool isExpression(Node *ptr);
    void tacExpression(Node *ptr);
    bool isLeafNode(Node *ptr);

    Node *nRoot;
    Node *currentNode;

public:
    CFG();
    CFG(Node *root);
    void buildCFG();
    void buildCFGrec();
};

#endif
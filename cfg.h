#ifndef CFG_H
#define CFG_H

#include <iostream>
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

public:
    CFG();
    CFG(Node *root);
    void buildCFG();
    void buildCFGrec(Node *ptr);
};

#endif
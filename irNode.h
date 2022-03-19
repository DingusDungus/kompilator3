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
    retStruct() : bblock(nullptr), value("") {}
    retStruct(std::string value) : bblock(nullptr), value(value) {}
    retStruct(std::string value, BBlock *bblock) : bblock(bblock), value(value) {}
    retStruct(BBlock *bblock) : bblock(bblock), value("") {}
};

class irNode
{
private:
    retStruct *connector(BBlock *currentBlock);
    retStruct *boolean(BBlock *currentBlock);
    retStruct *express(BBlock *currentBlock);
    retStruct *assignExpress(BBlock *currentBlock);
    retStruct *ifElse(BBlock *currentBlock);
    retStruct *whileStmt(BBlock *currentBlock);
    retStruct *identifier(BBlock *currentBlock);
    retStruct *integer(BBlock *currentBlock);
    retStruct *ArrayIndexAccess(BBlock *currentBlock);
    retStruct *newIntArray(BBlock *currentBlock);
    retStruct *newIdentifier(BBlock *currentBlock);
    retStruct *notOp(BBlock *currentBlock);
    retStruct *methodCall(BBlock *currentBlock);

    retStruct *temp(BBlock *currentBlock);

    Node *headNode;

public:
    irNode();
    irNode(std::string type);
    irNode(std::string type, Node *ptr);
    ~irNode();

    std::vector<irNode *> child;
    std::string name;
    retStruct *lhs;
    retStruct *rhs;
    std::string op;
    std::string type;
    int id;

    retStruct *genIr(BBlock *currentBlock);

    std::string genNameAssign(BBlock *currentBlk);
    std::string genName(BBlock *currentBlk);
    std::string genTempName(BBlock *currentBlk);
    std::string genBlkName();
};

#endif

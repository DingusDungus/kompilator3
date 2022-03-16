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
    retStruct(BBlock *bblock) : bblock(bblock), value("") {}
};

class irNode
{
private:
    retStruct boolean(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct express(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct assignExpress(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct ifElse(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct whileStmt(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct identifier(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct integer(std::_List_iterator<Node *> node, BBlock *currentBlock);
    retStruct temp(std::_List_iterator<Node *> node, BBlock *currentBlock);

public:
    irNode();
    irNode(std::string type);
    irNode(std::string type, std::string name);
    ~irNode();

    std::vector<irNode *> child;
    std::string name;
    retStruct lhs;
    retStruct rhs;
    std::string type;
    int id;

    retStruct genIr(std::_List_iterator<Node *> node, BBlock *currentBlock);

    std::string genNameAssign(std::_List_iterator<Node *> node, BBlock *currentBlk);
    std::string genName(std::_List_iterator<Node *> node, BBlock *currentBlk);
    std::string genTempName(BBlock *currentBlk);
    std::string genBlkName();
};

#endif

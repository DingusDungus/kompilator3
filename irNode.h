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
    bool expressionBool(Node *ptr);
    int id;
    retStruct *connector(BBlock *currentBlock);
    retStruct *boolean(BBlock *currentBlock);
    retStruct *express(BBlock *currentBlock);
    retStruct *assignStmt(BBlock *currentBlock);
    retStruct *ifElse(BBlock *currentBlock);
    retStruct *whileStmt(BBlock *currentBlock);
    retStruct *identifier(BBlock *currentBlock);
    retStruct *literal(BBlock *currentBlock);
    retStruct *ArrayIndexAccess(BBlock *currentBlock);
    retStruct *newIntArray(BBlock *currentBlock);
    retStruct *newIdentifier(BBlock *currentBlock);
    retStruct *notOp(BBlock *currentBlock);
    retStruct *methodCall(BBlock *currentBlock);
    retStruct *methodDec(BBlock *currentBlock);
    retStruct *methodDecList(BBlock *currentBlock);
    retStruct *thisExp(BBlock *currentBlock);
    retStruct *boolExp(BBlock *currentBlock);
    retStruct *printStmt(BBlock *currentBlock);
    std::string getBoolName(Node *node, BBlock *currentBlock);
    tac *genCondTac(Node *node, BBlock *currentBlock, std::string tempName = "");

    retStruct *temp(BBlock *currentBlock);

    Node *headNode;

    void generate_tree_content(int &count, ofstream *outStream)
    {
        id = count++;
        *outStream << "n" << id << " [label=\"" << type << "\"];" << endl;
        for (int i = 0;i < child.size();i++)
        {
            child[i]->generate_tree_content(count, outStream);
            *outStream << "n" << id << " -> n" << child[i]->id << endl;
        }
    }

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

    retStruct *genIr(BBlock *currentBlock);

    std::string genNameAssign(BBlock *currentBlk);
    std::string genName(BBlock *currentBlk);
    std::string genTempName(BBlock *currentBlk);
    std::string genBlkName();
    void generate_tree()
    {
        std::ofstream outStream;
        outStream.open("irTree.dot");

        int count = 0;
        outStream << "digraph {" << std::endl;
        generate_tree_content(count, &outStream);
        outStream << "}" << std::endl;
        outStream.close();

        std::cout << "\nBuilt a IR parse-tree:" << std::endl;
    }
};

#endif

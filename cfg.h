#ifndef CFG_H
#define CFG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

#include "tac.h"
#include "Node.h"
#include "irNode.h"
#include "bblock.h"

class CFG
{
private:
    BBlock *root;
    std::vector<tac *> temp;
    std::string genBlkName();
    irNode *expression(Node *);
    bool expressionBool(Node *);
    bool literalBool(Node *);
    std::string tacExpression(Node *ptr);

    Node *nRoot;
    irNode *iRoot;
    std::vector<std::string> postOrderList;
    std::vector<irNode> postOrderNodes;

public:
    CFG();
    CFG(Node *root);
    void buildIrNodeAST(Node *node, irNode *iNode);
    void buildIrNodeAST(Node *node);
    void buildCFG();
    void postOrderTraversal(Node *leaf);
    irNode *parseNodes(Node *ptr);
    void printPostOrder();
    Node *searchTree(Node *root, std::string value);
    void printBlocks();
    void printBlocksRec(BBlock *ptr);
    void generate_tree();
    void genByteCode();
};

#endif

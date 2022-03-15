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

    std::string virtual genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) = 0;

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


#endif

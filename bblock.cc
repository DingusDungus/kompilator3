#include "bblock.h"

std::string CFG::genBlkName()
{
    std::string blkName = "block_" + std::to_string(blockNr);
    blockNr++;
    return blkName;
}

CFG::CFG()
{
    blockNr = 0;
    root = new BBlock(genBlkName());
    currentBlk = root;
    nRoot = nullptr;
    currentNode = nullptr;
}

CFG::CFG(Node *_root)
{
    blockNr = 0;
    root = new BBlock(genBlkName());
    currentBlk = root;
    nRoot = _root;
    currentNode = nRoot;
    currentTac = nullptr;
}

bool CFG::isLeafNode(Node *ptr)
{
    if (ptr->type == "IntegerLiteral" || ptr->type == "BooleanExpression" || ptr->type == "Identifier")
    {
        return true;
    }
    return false;
}

std::string CFG::genTempName()
{
    std::string temp = "_+" + std::to_string(currentBlk->tempCount);
    currentBlk->tempCount++;
    return temp;
}

void CFG::tacExpression(Node *ptr)
{
    auto child = ptr->children.begin();
    int posCount = 0;
    if (isLeafNode((*child)))
    {

    }
    else if (isExpressionHeadNode((*child)))
    {

    }
    
}

bool CFG::isExpressionHeadNode(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "OrOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "OrOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "OrOP" || ptr->type == "MultOP" || ptr->type == "DivOP" || ptr->type == "ArrayIndexAccessExpression" || ptr->type == "newIntArray" || ptr->type == "newIdentifier" || ptr->type == "NotOP")
    {
        return true;
    }
    return false;
}

void CFG::buildCFG()
{
    for (auto i = currentNode->children.begin();i != currentNode->children.end();i++)
    {
        if ((*i)->type == "AssignStatement") 
        {
            
        }
    }
}
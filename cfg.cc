#include "cfg.h"

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
}

CFG::CFG(Node *_root)
{
    blockNr = 0;
    root = new BBlock(genBlkName());
    currentBlk = root;
    nRoot = _root;
}

bool CFG::isLeafNode(Node *ptr)
{
    if (ptr->type == "IntegerLiteral" || ptr->type == "BooleanExpression" || ptr->type == "Identifier")
    {
        return true;
    }
    return false;
}

void CFG::buildCFG()
{
    for (auto i = nRoot->children.begin();i != nRoot->children.end();i++)
    {
        
    }
}

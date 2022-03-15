#include "cfg.h"
#include "irNode.h"

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
    iRoot = nullptr;
}

CFG::CFG(Node *_root)
{
    blockNr = 0;
    root = new BBlock(genBlkName());
    currentBlk = root;
    nRoot = _root;
    iRoot = nullptr;
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

irNode* parseNodes(Node* ptr)
{
    if (ptr->type == "AddOP") {
        irNode* node = new addExpression();
    }
    else if (ptr->type == "SubOP") {

    }
    else if (ptr->type == "MultOP") {

    }
    else if (ptr->type == "Identifier") {

    }
    else if (ptr->type == "IntegerLiteral") {

    }
    else if (ptr->type == "SystemOutPrintStatement") {

    }
    else if (ptr->type == "Identifier") {

    }
    else if (ptr->type == "Identifier") {

    }
    else if (ptr->type == "Identifier") {

    }
    else if (ptr->type == "Identifier") {

    }
    return nullptr;
}

void CFG::postOrderTraversal(Node *leaf)
{
   if(leaf == nullptr) {
        return;
    }

    for (auto i = leaf->children.begin(); i != leaf->children.end(); i++) {
        postOrderTraversal((*i));
    }

    // do something with the node here
    postOrderList.push_back(leaf->type + " : " + leaf->value);
}

void CFG::printPostOrder()
{
    std::cout << "--- Printing post order ---" << std::endl;
    for(int i = 0; i < postOrderList.size(); i++)
    {
        std::cout << postOrderList[i] << std::endl;
    }
}


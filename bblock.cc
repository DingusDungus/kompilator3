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

std::string CFG::tacExpression(Node *ptr)
{
    int posCount = 0;
    tac *newTac = new tac;
    if (isLeafNode(ptr))
    {
        return ptr->value;
    }
    else if (isExpressionHeadNode(ptr))
    {
        newTac->op = ptr->type;
        auto child = ptr->children.begin();
        newTac->lhs = tacExpression((*child));
        child++;
        newTac->rhs = tacExpression((*child));
        currentBlk->instructions.push_back(newTac);
        return genTempName();
    }
    else if (isUnaryHeadNode(ptr))
    {
        newTac->op = ptr->type;
        newTac->result = genTempName();
        auto child = ptr->children.begin();
        newTac->lhs = tacExpression((*child));
        child++;
        newTac->rhs = "";
        currentBlk->instructions.push_back(newTac);
        return genTempName(); 
    }
    else if (ptr->type == "IdentifierExpression")
    {
        auto child = ptr->children.begin();
        return (*child)->value;
    }
    return " ";
}

bool CFG::isExpressionHeadNode(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "OrOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "MultOP" || ptr->type == "DivOP")
    {
        return true;
    }
    return false;
}

bool CFG::isUnaryHeadNode(Node *ptr)
{
    if (ptr->type == "ArrayIndexAccessExpression" || ptr->type == "newIntArray" || ptr->type == "newIdentifier" || ptr->type == "NotOP")
    {
        return true;
    }
    return false;
}

void CFG::buildCFG()
{
    for (auto i = nRoot->children.begin(); i != nRoot->children.end(); i++)
    {
        if ((*i)->type == "AssignStatement")
        {
            auto child = (*i)->children.begin();
            tac *newTac = new tac;
            newTac->result = (*child)->value;
            child++;
            auto expressionChild = (*child)->children.begin();
            newTac->lhs = tacExpression((*expressionChild));
            expressionChild++;
            newTac->rhs = tacExpression((*expressionChild));
            currentBlk->instructions.push_back(newTac);
        }
        buildCFGrec((*i));
    }
}

void CFG::buildCFGrec(Node *ptr)
{
    for (auto i = ptr->children.begin(); i != currentNode->children.end(); i++)
    {
        if ((*i)->type == "AssignStatement")
        {
            auto child = (*i)->children.begin();
            tac *newTac = new tac;
            newTac->result = (*child)->value;
            child++;
            auto expressionChild = (*child)->children.begin();
            newTac->lhs = tacExpression((*expressionChild));
            expressionChild++;
            newTac->rhs = tacExpression((*expressionChild));
            currentBlk->instructions.push_back(newTac);
        }
        else
        {
            buildCFGrec((*i));
        }
    }
}

#include "irNode.h"

int blockNr = 0;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type) {}
irNode::irNode(std::string type, std::string name) : type(type), name(name) {}

irNode::~irNode() {}

retStruct irNode::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    if (type == "subExpression")
    {
        return subExpression(node, currentBlock);
    }
    else if (type == "addExpression")
    {
        return addExpression(node, currentBlock);
    }
    else if (type == "booleanExpression")
    {
        return booleanExpression(node, currentBlock);
    }
    else if (type == "ifElse")
    {
        return ifElse(node, currentBlock);
    }
    else if (type == "identifier")
    {
        return identifier(node, currentBlock);
    }
    else if (type == "integer")
    {
       return integer(node, currentBlock); 
    }
    else if (type == "temp")
    {
        return temp(node, currentBlock);
    }
}

std::string irNode::genName(std::_List_iterator<Node *> node, BBlock *currentBlk)
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

std::string irNode::genTempName(BBlock *currentBlk)
{
    {
        std::string temp = "_+" + std::to_string(currentBlk->tempCount);
        currentBlk->tempCount++;
        return temp;
    }
}

std::string irNode::genBlkName()
{
    std::string blkName = "block_" + std::to_string(blockNr);
    blockNr++;
    return blkName;
}

// sub-expression
retStruct irNode::subExpression(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    name = genName(node, currentBlock);
    auto childNode = (*node)->children.begin();
    lhs = child[0]->genIr(childNode, currentBlock);
    childNode++;
    rhs = child[1]->genIr(childNode, currentBlock);
    tac *in = new expression("-", lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// add-expression
retStruct irNode::addExpression(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    name = genName(node, currentBlock);
    auto childNode = (*node)->children.begin();
    lhs = child[0]->genIr(childNode, currentBlock);
    childNode++;
    rhs = child[1]->genIr(childNode, currentBlock);
    tac *in = new expression("+", lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// boolean-expression
retStruct irNode::booleanExpression(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    auto childNode = (*node)->children.begin();
    lhs = child[0]->genIr(childNode, currentBlock);
    childNode++;
    rhs = child[1]->genIr(childNode, currentBlock);
    tac *in = new expression((*node)->type, lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr); 
}

// if-else
retStruct irNode::ifElse(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    auto childNode = (*node)->children.begin();
    child[0]->genIr((*node)->children.begin(), currentBlock);
    BBlock *trueBlock = new BBlock(genBlkName());
    lhs = child[1]->genIr(childNode, trueBlock);
    childNode++;
    BBlock *falseBlock = new BBlock(genBlkName());
    rhs = child[2]->genIr(childNode, falseBlock);
    BBlock *joinBlock = new BBlock(genBlkName());

    currentBlock->trueExit = trueBlock;
    currentBlock->falseExit = falseBlock;

    trueBlock->trueExit = joinBlock;
    falseBlock->trueExit = joinBlock;

    return retStruct("ifElse-joinBlock", joinBlock);
}

// while
retStruct irNode::whileStmt(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    auto childNode = (*node)->children.begin();
    child[0].genIr(childNode, currentBlock);
    BBlock *trueBlock = new BBlock;
    childNode++;
    child[1].genIr(childNode, trueBlock);
    BBlock *falseBlock = new BBlock;
    auto nextStmt = node++;
    child[2].genIr(nextStmt, falseBlock);

    currentBlock->trueExit = trueBlock;
    trueBlock->trueExit = currentBlock;
    currentBlock->falseExit = falseBlock;

    return retStruct("while-statement", falseBlock);
}

// identifier
retStruct irNode::identifier(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    return retStruct((*node)->value, nullptr);
}

// integer-literal
retStruct irNode::integer(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    return retStruct((*node)->value, nullptr);
}

// temp
retStruct irNode::temp(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    std::string tempName = "_" + std::to_string(currentBlock->tempCount);
    currentBlock->tempCount++;
    return retStruct(tempName, nullptr);
}

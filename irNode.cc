#include "irNode.h"

int blockNr = 0;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type) {}
irNode::irNode(std::string type, std::string name) : type(type), name(name) {}

irNode::~irNode() {}

retStruct irNode::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    if (type == "assignExpression")
    {
        return assignExpress(node, currentBlock);
    }
    if (type == "expression")
    {
        return express(node, currentBlock);
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

std::string irNode::genNameAssign(std::_List_iterator<Node *> node, BBlock *currentBlk)
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

std::string irNode::genName(std::_List_iterator<Node *> node, BBlock *currentBlk)
{
    Node *parent = (*node)->parent_node;
    if (parent->type == "IF_ElseStatement" || parent->type == "WhileStatement" || parent->type == "SystemOutPrintStatement")
    {
        return parent->type;
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

// assign-expression
retStruct irNode::assignExpress(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    name = genNameAssign(node, currentBlock);
    auto childNode = (*node)->children.begin();
    lhs = child[0]->genIr(childNode, currentBlock);
    childNode++;
    rhs = child[1]->genIr(childNode, currentBlock);
    tac *in = new expression((*node)->type, lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// expression without assign
retStruct irNode::express(std::_List_iterator<Node *> node, BBlock *currentBlock)
{
    name = genNameAssign(node, currentBlock);
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

    tac *in = new expression("if-statement", lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);

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
    child[0]->genIr(childNode, currentBlock);
    BBlock *trueBlock = new BBlock;
    childNode++;
    lhs = child[1]->genIr(childNode, trueBlock);
    BBlock *falseBlock = new BBlock;
    auto nextStmt = node++;
    rhs = child[2]->genIr(nextStmt, falseBlock);

    tac *in = new expression("while-statement", lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);

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

// boolean-literal
retStruct irNode::boolean(std::_List_iterator<Node *> node, BBlock *currentBlock)
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

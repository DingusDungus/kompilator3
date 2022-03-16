#include "irNode.h"

int blockNr = 0;

// base-class
irNode::irNode() {}

irNode::~irNode() {}

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
retStruct subExpression::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
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
retStruct addExpression::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
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
retStruct booleanExpression::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
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
retStruct ifElse::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
{
    BBlock *headBlock = new BBlock(genBlkName());
    booleanExpression *bExpression = new booleanExpression;
    bExpression->genIr((*node)->children.begin(), currentBlock);
    BBlock *trueBlock = 

}

// identifier
retStruct identifier::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
{
    return retStruct((*node)->value, nullptr);
}

// integer-literal
retStruct integer::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
{
    return retStruct((*node)->value, nullptr);
}

// temp
retStruct temp::genIr(std::_List_iterator<Node *> node, BBlock *currentBlock) override
{
    std::string tempName = "_" + std::to_string(currentBlock->tempCount);
    currentBlock->tempCount++;
    return retStruct(tempName, nullptr);
}

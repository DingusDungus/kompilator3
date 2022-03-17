#include "irNode.h"

int blockNr = 0;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type) {}
irNode::irNode(std::string type, Node *node) : type(type), headNode(node) {}

irNode::~irNode() {}

retStruct irNode::genIr(BBlock *currentBlock)
{
    std::cout << type << std::endl;
    if (type == "connector")
    {
        return connector(currentBlock);
    }
    else if (type == "assignExpression")
    {
        return assignExpress(currentBlock);
    }
    else if (type == "expression")
    {
        return express(currentBlock);
    }
    else if (type == "ifElse")
    {
        return ifElse(currentBlock);
    }
    else if (type == "whileStmt")
    {
        return whileStmt(currentBlock);
    }
    else if (type == "identifier")
    {
        return identifier(currentBlock);
    }
    else if (type == "integer")
    {
        return integer(currentBlock);
    }
    else if (type == "temp")
    {
        return temp(currentBlock);
    }
}

std::string irNode::genNameAssign(BBlock *currentBlk)
{
    Node *parent = headNode->parent_node;
    auto identifier = parent->children.begin();
    if ((*identifier)->type == "IdentifierExpression")
    {
        auto child = (*identifier)->children.begin();
        return (*child)->value;
    }
    else
    {
        return genTempName(currentBlk);
    }
}

std::string irNode::genName(BBlock *currentBlk)
{
    Node *parent = headNode->parent_node;
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

// Connector
retStruct irNode::connector(BBlock *currentBlock)
{
    for (int i = 0; i < child.size(); i++)
    {
        child[i]->genIr(currentBlock);
    }
    return retStruct("Connector", nullptr);
}

// assign-expression
retStruct irNode::assignExpress(BBlock *currentBlock)
{
    name = genNameAssign(currentBlock);
    lhs = child[0]->genIr(currentBlock);
    rhs = child[1]->genIr(currentBlock);
    tac *in = new expression(headNode->type, lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// expression without assign
retStruct irNode::express(BBlock *currentBlock)
{
    name = genNameAssign(currentBlock);
    lhs = child[0]->genIr(currentBlock);
    rhs = child[1]->genIr(currentBlock);
    tac *in = new expression(headNode->type, lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// notOp
retStruct irNode::notOp(BBlock *currentBlock)
{
    name = "notOp";
    lhs = retStruct("(");
    rhs = retStruct(")");
    op = child[0]->genIr(currentBlock).value;
    tac *in = new expression(op, lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);
    return retStruct(name, nullptr);
}

// if-else
retStruct irNode::ifElse(BBlock *currentBlock)
{
    child[0]->genIr(currentBlock);
    BBlock *trueBlock = new BBlock(genBlkName());
    lhs = child[1]->genIr(trueBlock);
    BBlock *falseBlock = new BBlock(genBlkName());
    rhs = child[2]->genIr(falseBlock);
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
retStruct irNode::whileStmt(BBlock *currentBlock)
{
    child[0]->genIr(currentBlock);
    BBlock *trueBlock = new BBlock;
    lhs = child[1]->genIr(trueBlock);
    BBlock *falseBlock = new BBlock;
    rhs = child[2]->genIr(falseBlock);

    tac *in = new expression("while-statement", lhs.value, rhs.value, name);
    currentBlock->instructions.push_back(in);

    currentBlock->trueExit = trueBlock;
    trueBlock->trueExit = currentBlock;
    currentBlock->falseExit = falseBlock;

    return retStruct("while-statement", falseBlock);
}

// identifier
retStruct irNode::identifier(BBlock *currentBlock)
{
    return retStruct(headNode->value, nullptr);
}

// integer-literal
retStruct irNode::integer(BBlock *currentBlock)
{
    return retStruct(headNode->value, nullptr);
}

// boolean-literal
retStruct irNode::boolean(BBlock *currentBlock)
{
    return retStruct(headNode->value, nullptr);
}

// temp
retStruct irNode::temp(BBlock *currentBlock)
{
    std::string tempName = "_" + std::to_string(currentBlock->tempCount);
    currentBlock->tempCount++;
    return retStruct(tempName, nullptr);
}

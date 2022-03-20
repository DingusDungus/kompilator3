#include "irNode.h"

int blockNr = 0;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type) {}
irNode::irNode(std::string type, Node *node) : type(type), headNode(node) {}

irNode::~irNode() {}

retStruct *irNode::genIr(BBlock *currentBlock)
{
    std::cout << "genIR_type: " << type << std::endl;
    std::cout << "genIR_child.size(): " << child.size() << std::endl;
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
    else if (type == "printStmt")
    {
        return printStmt(currentBlock);
    }
    else
    {
        return new retStruct();
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
        std::string temp = "_T" + std::to_string(currentBlk->tempCount);
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
retStruct *irNode::printStmt(BBlock *currentBlock)
{
    return new retStruct("printStmt", currentBlock);
}

// Connector
retStruct *irNode::connector(BBlock *currentBlock)
{
    for (int i = 0; i < child.size(); i++)
    {
        child[i]->genIr(currentBlock);
    }
    return new retStruct("Connector", nullptr);
}

// assign-expression
retStruct *irNode::assignExpress(BBlock *currentBlock)
{
    name = genNameAssign(currentBlock);
    std::cout << "Nr of childs: " << child.size() << std::endl;
    if (child.size() > 0)
    {
        std::cout << "child 0: " << child[0]->headNode->type << std::endl;
        std::cout << "child 0: " << child[0]->headNode->value << std::endl;
        lhs = child[0]->genIr(currentBlock);
    }
    else if (child.size() > 1)
    {
        rhs = child[1]->genIr(currentBlock);
    }
    std::cout << "Name: " << name << std::endl;
    if (headNode == nullptr) {
        std::cout << "head is null" << std::endl;
    }
    if (rhs == nullptr)
    {
        std::cout << "rhs is null" << std::endl;
    }
    if (lhs == nullptr)
    {
        std::cout << "lhs is null" << std::endl;
    }
    if (lhs && rhs && headNode){ // only create instruction if things are not nullptr
        tac *in = new expression(headNode->type, lhs->value, rhs->value, name);
        currentBlock->instructions.push_back(in);
        std::cout << in->result << ":=" << in->lhs << in->op << in->rhs << std::endl;
    }
    return new retStruct(name, currentBlock);
}

// expression without assign
retStruct *irNode::express(BBlock *currentBlock)
{
    name = genNameAssign(currentBlock);
    if (child.size() > 0)
    {
        lhs = child[0]->genIr(currentBlock);
    }
    else if (child.size() > 1)
    {
        rhs = child[1]->genIr(currentBlock);
    }
    tac *in = new expression(headNode->type, lhs->value, rhs->value, name);
    currentBlock->instructions.push_back(in);
    return new retStruct(name, nullptr);
}

// notOp
retStruct *irNode::notOp(BBlock *currentBlock)
{
    name = "notOp";
    lhs = new retStruct("(");
    rhs = new retStruct(")");
    if (child[0] != nullptr)
    {
        op = child[0]->genIr(currentBlock)->value;
    }
    tac *in = new expression(op, lhs->value, rhs->value, name);
    currentBlock->instructions.push_back(in);
    return new retStruct(name, nullptr);
}

// if-else
retStruct *irNode::ifElse(BBlock *currentBlock)
{
    if (currentBlock)
    {
        std::cout << "block is null" << std::endl;
    }
    child[0]->genIr(currentBlock);
    BBlock *trueBlock = new BBlock(genBlkName());
    lhs = child[1]->genIr(trueBlock);
    BBlock *falseBlock = new BBlock(genBlkName());
    rhs = child[2]->genIr(falseBlock);
    BBlock *joinBlock = new BBlock(genBlkName());

    tac *in = new expression("if-statement", lhs->value, rhs->value, name);
    currentBlock->instructions.push_back(in);

    currentBlock->trueExit = trueBlock;
    currentBlock->falseExit = falseBlock;

    trueBlock->trueExit = joinBlock;
    falseBlock->trueExit = joinBlock;

    return new retStruct("ifElse-joinBlock", joinBlock);
}

// while
retStruct *irNode::whileStmt(BBlock *currentBlock)
{
    child[0]->genIr(currentBlock);
    BBlock *trueBlock = new BBlock;
    lhs = child[1]->genIr(trueBlock);
    BBlock *falseBlock = new BBlock;
    rhs = child[2]->genIr(falseBlock);

    tac *in = new expression("while-statement", lhs->value, rhs->value, name);
    currentBlock->instructions.push_back(in);

    currentBlock->trueExit = trueBlock;
    trueBlock->trueExit = currentBlock;
    currentBlock->falseExit = falseBlock;

    return new retStruct("while-statement", falseBlock);
}

// identifier
retStruct *irNode::identifier(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// integer-literal
retStruct *irNode::integer(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// boolean-literal
retStruct *irNode::boolean(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// temp
retStruct *irNode::temp(BBlock *currentBlock)
{
    std::string tempName = "_" + std::to_string(currentBlock->tempCount);
    currentBlock->tempCount++;
    return new retStruct(tempName, nullptr);
}

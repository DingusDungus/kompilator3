#include "irNode.h"

int extern blockNr;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type) {}
irNode::irNode(std::string type, Node *node) : type(type), headNode(node) {}

irNode::~irNode() {}

retStruct *irNode::genIr(BBlock *currentBlock)
{
    std::cout << "genIR_type: " << type << " headNode_type: " << headNode->type << std::endl;
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
    else if (type == "methodCall")
    {
        return methodCall(currentBlock);
    }
    else
    {
        return connector(currentBlock);
    }
}

std::string irNode::genNameAssign(BBlock *currentBlk)
{
    auto identifier = headNode->children.begin();
    return (*identifier)->value;
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

bool irNode::expressionBool(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "OrOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "MultOP" || ptr->type == "DivOP" || ptr->type == "ArrayIndexAccessExpression" || ptr->type == "newIntArray" || ptr->type == "newIdentifier" || ptr->type == "NotOP")
    {
        return true;
    }
    return false;
}

std::string irNode::getBoolName(Node *node, BBlock *currentBlock)
{
    if (node->type == "IdentifierExpression")
    {
        auto child = node->children.begin();
        return (*child)->value;
    }
    if (expressionBool(node))
    {
        return genName(currentBlock);
    }
    else
    {
        return node->value;
    }
}

tac *irNode::genCondTac(Node *ptr, BBlock *currentBlock)
{
    tac *condTac = new tac;
    condTac->result = "condJump";
    condTac->op = " " + ptr->type + " ";
    auto child = ptr->children.begin();
    if (ptr->type == "NotOP")
    {
        condTac->lhs = getBoolName((*child), currentBlock);
        return condTac;
    }
    condTac->lhs = getBoolName((*child), currentBlock);
    child++;
    condTac->rhs = getBoolName((*child), currentBlock);
    
    return condTac;
}

retStruct *irNode::methodCall(BBlock *currentBlock)
{
}

// Connector
retStruct *irNode::printStmt(BBlock *currentBlock)
{
    return new retStruct("printStmt", currentBlock);
}

// Connector
retStruct *irNode::connector(BBlock *currentBlock)
{
    retStruct *returnVal;
    for (int i = 0; i < child.size(); i++)
    {
        returnVal = child[i]->genIr(currentBlock);
        if (returnVal->value == "ifElse-joinBlock" || returnVal->value == "while-statement-falseBlock" || returnVal->value == "connector")
        {
            currentBlock = returnVal->bblock;
        }
    }
    return new retStruct("connector", currentBlock);
}

// assign-expression
retStruct *irNode::assignExpress(BBlock *currentBlock)
{
    name = genNameAssign(currentBlock);
    if (child.size() > 0)
    {
        lhs = child[0]->genIr(currentBlock);
    }
    if (child.size() > 1)
    {
        rhs = child[1]->genIr(currentBlock);
    }
    if (lhs && rhs && headNode)
    { // only create instruction if things are not nullptr
        tac *in = new expression(" " + child[1]->headNode->type + " ", lhs->value, rhs->value, name);
        currentBlock->instructions.push_back(in);
        std::cout << in->result << ":=" << in->lhs << in->op << in->rhs << std::endl;
    }
    return new retStruct(name, currentBlock);
}

// expression without assign
retStruct *irNode::express(BBlock *currentBlock)
{
    name = genName(currentBlock);
    std::cout << name << std::endl;
    if (child.size() > 0)
    {
        std::cout << child[0]->headNode->type << std::endl;
        lhs = child[0]->genIr(currentBlock);
    }
    else
    {
        return new retStruct("empty expression", currentBlock);
    }
    if (child.size() > 1)
    {
        std::cout << child[1]->headNode->type << std::endl;
        rhs = child[1]->genIr(currentBlock);
    }
    else
    {
        return new retStruct("empty expression", currentBlock);
    }
    tac *in = new expression(" " + headNode->type + " ", lhs->value, rhs->value, name);
    currentBlock->instructions.push_back(in);
    return new retStruct(name, currentBlock);
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
    if (child[0]->child.size() > 0)
    {
        child[0]->child[0]->genIr(currentBlock);
        if (child[0]->child.size() > 1)
        {
            child[0]->child[1]->genIr(currentBlock);
        }
    }
    BBlock *trueBlock = new BBlock;
    lhs = child[1]->genIr(trueBlock);
    BBlock *falseBlock = new BBlock;

    tac *in = genCondTac((*headNode->children.begin()), currentBlock);
    currentBlock->instructions.push_back(in);

    currentBlock->trueExit = trueBlock;
    trueBlock->trueExit = currentBlock;
    currentBlock->falseExit = falseBlock;

    return new retStruct("while-statement-falseBlock", falseBlock);
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

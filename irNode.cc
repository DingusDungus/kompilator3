#include "irNode.h"
#include "tac.h"
#include <string>

int extern blockNr;
int extern tempNr;
std::vector<BBlock *> extern methodDecBlocks;

// base-class
irNode::irNode() {}
irNode::irNode(std::string type) : type(type), lhs(nullptr), rhs(nullptr) {}
irNode::irNode(std::string type, Node *node) : type(type), headNode(node), lhs(nullptr), rhs(nullptr) {}

irNode::~irNode() {}

retStruct *irNode::genIr(BBlock *currentBlock)
{
    // std::cout << "genIR_type: " << type << " headNode_type: " << headNode->type << std::endl;
    // std::cout << "genIR_child.size(): " << child.size() << std::endl;
    if (type == "connector")
    {
        return connector(currentBlock);
    }
    else if (type == "assignStmt")
    {
        return assignStmt(currentBlock);
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
    else if (type == "literal")
    {
        return literal(currentBlock);
    }
    else if (type == "temp")
    {
        return temp(currentBlock);
    }
    else if (type == "printStmt")
    {
        return printStmt(currentBlock);
    }
    else if (type == "returnExp")
    {
        return returnExp(currentBlock);
    }
    else if (type == "methodDec")
    {
        return methodDec(currentBlock);
    }
    else if (type == "methodCall")
    {
        return methodCall(currentBlock);
    }
    else if (type == "thisExp")
    {
        return thisExp(currentBlock);
    }
    else if (type == "boolExp")
    {
        return boolExp(currentBlock);
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
    if (headNode->parent_node->type == "AssignStatement") 
    {
        return (*headNode->parent_node->children.begin())->value;
    }
    else
    {
        return genTempName(currentBlk);
    }
}

std::string irNode::genTempName(BBlock *currentBlk)
{
    {
        std::string temp = "_T" + std::to_string(tempNr);
        tempNr++;
        return temp;
    }
}

std::string irNode::genBlkName()
{
    std::string blkName = "block_" + std::to_string(blockNr);
    blockNr++;
    std::cout << blkName << std::endl;
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
    else if (expressionBool(node))
    {
        return genName(currentBlock);
    }
    else
    {
        return node->value;
    }
    return " ";
}

tac *irNode::genCondTac(Node *ptr, BBlock *currentBlock, std::string tempName)
{
    tac *condTac = new tac;
    condTac->result = "condJump";
    auto child = ptr->children.begin();
    if (ptr->type == "IdentifierExpression")
    {
        condTac->lhs = (*child)->value;
        return condTac;
    }
    else if (ptr->type == "NotOP")
    {
        condTac->op = " " + ptr->type + " ";
        condTac->lhs = getBoolName((*child), currentBlock);
        return condTac;
    }
    else if (ptr->type == "MethodCall")
    {
        condTac->op = "";
        condTac->lhs = "";
        condTac->rhs = tempName;
        return condTac;
    }
    condTac->op = " " + ptr->type + " ";
    condTac->lhs = getBoolName((*child), currentBlock);
    child++;
    condTac->rhs = getBoolName((*child), currentBlock);

    return condTac;
}

// retStruct *irNode::methodDecList(BBlock *currentBlock)
// {
//     for (int i = 0; i < child.size(); i++) {
//         child[i]->genIr(currentBlock);
//     }

//     return new retStruct("methodDecList", currentBlock);
// }

retStruct *irNode::methodDec(BBlock *currentBlock)
{
    std::string blockName = genBlkName();
    std::string methodName = child[0]->genIr(currentBlock)->value;
    methodName = " ("+methodName+")";
    BBlock *methodBlock = new BBlock(blockName+methodName);
    for (int i = 0; i < child.size()-1; i++)
    {
        lhs = child[i]->genIr(methodBlock);
    }
    std::string returnIr = child[child.size()-1]->genIr(lhs->bblock)->value;
    std::cout << "method return block: " << returnIr << std::endl;

    // arrow between return block and first method block
    // lhs->bblock->trueExit = methodBlock;

    methodDecBlocks.push_back(methodBlock);
    return new retStruct("methodDec", methodBlock);
}

retStruct *irNode::methodCall(BBlock *currentBlock)
{
    name = genName(currentBlock);
    int nrOfParams = 1;
    std::string methodName = child[1]->genIr(currentBlock)->value;
    std::cout << "methodName: " << methodName << std::endl;

    // add class scope as first parameter
    std::string scopeName = child[0]->genIr(currentBlock)->value;
    std::cout << "Class scope(first param): " << scopeName << std::endl;
    tac *scopeIn = new parameter(scopeName);
    currentBlock->instructions.push_back(scopeIn);

    // find params
    std::vector<irNode *> params;
    if (child[2] != nullptr){
        params = child[2]->child;
    }
    // loop through parameters
    for (int i = 0; i < params.size(); i++)
    {
        std::string param = params[i]->genIr(currentBlock)->value;
        std::cout << "param: " << param << std::endl;
        tac *paramIn = new parameter(param);
        currentBlock->instructions.push_back(paramIn);
        nrOfParams++;
    }

    // add last call instruction
    std::cout << "genName: " << name << std::endl;
    std::string paramNrStr = std::to_string(nrOfParams);
    std::cout << "nrOfParams: " << paramNrStr << std::endl;
    tac *callIn = new methodCallTac(methodName, paramNrStr, name);
    currentBlock->instructions.push_back(callIn);

    return new retStruct(name, currentBlock);
}

// print stmt
retStruct *irNode::printStmt(BBlock *currentBlock)
{
    if (child.size() > 0)
    {
        lhs = child[0]->genIr(currentBlock);
        std::cout << "PRINT_STMT: " << lhs->value << "block: " << currentBlock->name << std::endl;
        tac *printIn = new tacPrintOut(lhs->value);
        currentBlock->instructions.push_back(printIn);
    }
    return new retStruct("printStmt", currentBlock);
}

// return expression
retStruct *irNode::returnExp(BBlock *currentBlock)
{
    if (child.size() > 0)
    {
        lhs = child[0]->genIr(currentBlock);
    }
    tac* in = new tacReturnExp(lhs->value);
    currentBlock->instructions.push_back(in);
    return new retStruct(lhs->value, currentBlock);
}

// Connector
retStruct *irNode::connector(BBlock *currentBlock)
{
    retStruct *returnVal;
    BBlock *ptr = currentBlock;
    for (int i = 0; i < child.size(); i++)
    {
        returnVal = child[i]->genIr(ptr);
        if (returnVal->value == "ifElse-joinBlock" || returnVal->value == "while-statement-falseBlock")
        {
            ptr = returnVal->bblock;
        }
    }
    return new retStruct("connector", ptr);
}

// assign-statement
retStruct *irNode::assignStmt(BBlock *currentBlock)
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
        if (child[1]->headNode->type == "MethodCall"
            || child[1]->headNode->type == "Identifier"
            || child[1]->headNode->type == "IntegerLiteral"
            || child[1]->headNode->type == "BooleanExpression" )
        {
            tac *in = new expression("", "", rhs->value, name);
            currentBlock->instructions.push_back(in);
            std::cout << in->result << ":=" << in->lhs << in->op << in->rhs << std::endl;
        }
        else
        {
            // Default case
            tac *in = new expression(" " + child[1]->headNode->type + " ", lhs->value, rhs->value, name);
            currentBlock->instructions.push_back(in);
            std::cout << in->result << ":=" << in->lhs << in->op << in->rhs << std::endl;
        }
    }
    return new retStruct(name, currentBlock);
}

// expression without assign
retStruct *irNode::express(BBlock *currentBlock)
{
    name = genName(currentBlock);
    std::cout << name << std::endl;
    tac *in = new expression(" " + headNode->type + " ", " ", " ", name);
    if (child.size() > 0)
    {
        std::cout << child[0]->headNode->type << std::endl;
        lhs = child[0]->genIr(currentBlock);
        in->lhs = lhs->value;
        if (child.size() > 1)
        {
            std::cout << child[1]->headNode->type << std::endl;
            rhs = child[1]->genIr(currentBlock);
            in->rhs = rhs->value;
        }
    }
    else
    {
        return new retStruct("empty expression", currentBlock);
    }
    currentBlock->instructions.push_back(in);
    if (headNode->parent_node->type == "IF_ElseStatement")
    {
        tac *newTac = new expression("", name, "", "ifElse");
        currentBlock->instructions.push_back(newTac);
    }
    else if (headNode->parent_node->type == "WhileStatement")
    {
        tac *newTac = new expression("", name, "", "while");
        currentBlock->instructions.push_back(newTac);
    }
    else if (headNode->parent_node->type == "SystemOutPrintStatement")
    {
        tac *newTac = new expression("", name, "", "print");
        currentBlock->instructions.push_back(newTac);
    }
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
    std::string tempName = child[0]->genIr(currentBlock)->value;

    tac *in = genCondTac((*headNode->children.begin()), currentBlock, tempName);
    currentBlock->instructions.push_back(in);

    BBlock *trueBlock = new BBlock(genBlkName() + "( IfElse_trueBlock )");
    BBlock *falseBlock = new BBlock(genBlkName() + "( IfElse_falseBlock )");
    BBlock *joinBlock = new BBlock(genBlkName() + "( IfElse_joinBlock )");

    if (child[1] != nullptr){
        lhs = child[1]->genIr(trueBlock);
        std::cout << "IfElse lhs_block: " << lhs->bblock->name << std::endl;
    }
    if (child[2] != nullptr){
        rhs = child[2]->genIr(falseBlock);
        std::cout << "IfElse rhs_block: " << rhs->bblock->name << std::endl;
    }

    std::cout << "IfElse currentBlock: " << currentBlock->name << std::endl;
    std::cout << "IfElse joinBlock: " << joinBlock->name << std::endl;
    std::cout << "IfElse trueBlock: " << trueBlock->name << std::endl;
    std::cout << "IfElse falseBlock: " << falseBlock->name << std::endl;
    // link blocks
        lhs->bblock->trueExit = joinBlock;
        rhs->bblock->trueExit = joinBlock;
    // trueBlock->trueExit = joinBlock;
    // falseBlock->trueExit = joinBlock;
    currentBlock->trueExit = trueBlock;
    currentBlock->falseExit = falseBlock;

    return new retStruct("ifElse-joinBlock", joinBlock);
}

// while
retStruct *irNode::whileStmt(BBlock *currentBlock)
{
    BBlock *headBlock = new BBlock(genBlkName() + "( while_headBlock )");
    for (int i = 0; i < child[0]->child.size(); i++) {
        rhs = child[0]->child[i]->genIr(headBlock);
        std::cout << "while rhs block: " << rhs->bblock->name << std::endl;
    }
    BBlock *trueBlock = new BBlock(genBlkName() + "( while_body/trueBlock )");
    if (child[1] != nullptr){
        lhs = child[1]->genIr(trueBlock);
        std::cout << "while lhs block: " << lhs->bblock->name << std::endl;
        // std::cout << "while lhs block trueExit: " << lhs->bblock->trueExit->name << std::endl;
        // std::cout << "while lhs block falseExit: " << lhs->bblock->falseExit->name << std::endl;
    }
    BBlock *joinBlock = new BBlock(genBlkName() + "( while_joinBlock )");

    tac *in = genCondTac((*headNode->children.begin()), currentBlock);
    headBlock->instructions.push_back(in);

    std::cout << "while headBlock: " << headBlock->name << std::endl;
    std::cout << "while trueBlock: " << trueBlock->name << std::endl;
    std::cout << "while joinBlock: " << joinBlock->name << std::endl;
    std::cout << "while currentBlock: " << currentBlock->name << std::endl;

    // link blocks
    currentBlock->trueExit = headBlock;
    headBlock->trueExit = trueBlock;
    headBlock->falseExit = joinBlock;
    lhs->bblock->trueExit = headBlock;
    // lhs->bblock->falseExit = joinBlock;

    return new retStruct("while-statement-falseBlock", joinBlock);
}

// identifier
retStruct *irNode::identifier(BBlock *currentBlock)
{
    return new retStruct(headNode->value, currentBlock);
}

// ThisExpression
retStruct *irNode::thisExp(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// BooleanExpression
retStruct *irNode::boolExp(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// integer-literal
retStruct *irNode::literal(BBlock *currentBlock)
{
    return new retStruct(headNode->value, currentBlock);
}

// boolean-literal
retStruct *irNode::boolean(BBlock *currentBlock)
{
    return new retStruct(headNode->value, nullptr);
}

// temp
retStruct *irNode::temp(BBlock *currentBlock)
{
    std::string tempName = "_" + std::to_string(tempNr);
    tempNr++;
    return new retStruct(tempName, nullptr);
}

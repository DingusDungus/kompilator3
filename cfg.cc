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
    iRoot = new irNode("connector");
}

CFG::CFG(Node *_root)
{
    blockNr = 0;
    root = new BBlock(genBlkName());
    currentBlk = root;
    nRoot = _root;
    iRoot = new irNode("connector", nRoot);
}

void CFG::buildCFG()
{
    iRoot->genIr(root);
}

Node *CFG::searchTree(Node *root, std::string value)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    std::stack<Node *> s;
    s.push(root);

    while (!s.empty())
    {
        Node *current = s.top();
        if (current->value == value)
        {
            return current;
        }
        s.pop();

        for (auto i = current->children.begin(); i != current->children.end(); i++)
        {
            s.push((*i));
        }
    }
    return nullptr;
}

bool CFG::expressionBool(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "OrOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "MultOP" || ptr->type == "DivOP" || ptr->type == "ArrayIndexAccessExpression" || ptr->type == "newIntArray" || ptr->type == "newIdentifier" || ptr->type == "NotOP")
    {
        return true;
    }
    return false;
}

irNode *CFG::expression(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "OrOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "MultOP" || ptr->type == "DivOP")
    {
        return new irNode("expression", ptr);
    }
    else if (ptr->type == "NotOP")
    {
        return new irNode("notOp", ptr);
    }
    else if (ptr->type == "ArrayIndexAccessExpression")
    {
        return new irNode("ArrayIndexAccess", ptr);
    }
    else if (ptr->type == "newIntArray")
    {
        return new irNode("newIntArray", ptr);
    }
    else if (ptr->type == "newIdentifier")
    {
        return new irNode("newIdentifier", ptr);
    }
    return nullptr;
}

void CFG::buildIrNodeAST(Node *node, irNode *iNode)
{
    if (node == nullptr)
    {
        return;
    }
    for (auto i = node->children.begin(); i != node->children.end(); i++)
    {
        irNode *newNode = parseNodes(node);
        if (newNode != nullptr)
        {
            iNode->child.push_back(newNode);
            buildIrNodeAST((*i), newNode);
        }
        else
        {
            buildIrNodeAST((*i), iNode);
        }
    }
}

void CFG::buildIrNodeAST(Node *node)
{
    // start recursion
    buildIrNodeAST(nRoot, iRoot);
}

irNode *CFG::parseNodes(Node *ptr)
{
    if (ptr->type == "StatementList" || ptr->type == "ExpressionList")
    {
        return new irNode("connector", ptr);
    }
    else if (expressionBool(ptr))
    {
        return new irNode("expression", ptr);
    }
    else if (ptr->type == "Identifier" || ptr->type == "newIdentifier")
    {
        return new irNode("identifier", ptr);
    }
    // Statements
    else if (ptr->type == "SystemOutPrintStatement")
    {
        return nullptr;
    }
    else if (ptr->type == "AssignStatement")
    {
        return new irNode("assignExpression", ptr);
    }
    else if (ptr->type == "IF_ElseStatement")
    {
        //return new irNode("ifElse", ptr);
        return nullptr;
    }
    else if (ptr->type == "WhileStatement")
    {
        //return new irNode("whileStmt", ptr);
        return nullptr;
    }
    else if (ptr->type == "ArrayIndexAssignStatement")
    {
        return nullptr;
    }
    else if (ptr->type == "methodCall")
    {
        return nullptr;
    }
    // Variables
    // else if (ptr->type == "Identifier") {
    //     irNode node("identifier");
    // }
    // else if (ptr->type == "IntegerLiteral") {
    //     irNode node("integer");
    // }
    return nullptr;
}

void CFG::postOrderTraversal(Node *leaf)
{
    if (leaf == nullptr)
    {
        return;
    }

    for (auto i = leaf->children.begin(); i != leaf->children.end(); i++)
    {
        postOrderTraversal((*i));
    }

    // do something with the node here
    postOrderList.push_back(leaf->type + " : " + leaf->value);
}

void CFG::printPostOrder()
{
    std::cout << "--- Printing post order ---" << std::endl;
    for (int i = 0; i < postOrderList.size(); i++)
    {
        std::cout << postOrderList[i] << std::endl;
    }
}

void CFG::printBlocks()
{
    BBlock *ptr = root;
    if (ptr == nullptr)
    {
        std::cout << "root was nullptr" << std::endl;
        return;
    }
    std::cout << ptr->name << std::endl;
    for (int i = 0; i < ptr->instructions.size(); i++)
    {
        ptr->instructions[i]->dump();
    }
    std::cout << std::endl;
    if (ptr->trueExit != nullptr)
    {
        printBlocksRec(ptr->trueExit);
    }
    else if (ptr->falseExit != nullptr)
    {
        printBlocksRec(ptr->falseExit);
    }
    return;
}

void CFG::printBlocksRec(BBlock *ptr)
{
    if (ptr == nullptr)
    {
        std::cout << "was nullptr" << std::endl;
        return;
    }
    for (int i = 0; i < ptr->instructions.size(); i++)
    {
        ptr->instructions[i]->dump();
    }
    std::cout << std::endl;
    if (ptr->trueExit != nullptr)
    {
        printBlocksRec(ptr->trueExit);
    }
    else if (ptr->falseExit != nullptr)
    {
        printBlocksRec(ptr->falseExit);
    }
    return;
}

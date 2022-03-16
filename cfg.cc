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

Node* CFG::searchTree(Node* root, std::string value)
{
        if (root == nullptr) {
        return nullptr;
    }
    std::stack<Node*> s;
    s.push(root);

    while (!s.empty()) {
        Node* current = s.top();
        if (current->value == value) {
            return current;
        }
        s.pop();

        for (auto i = current->children.begin(); i != current->children.end(); i++) {
            s.push((*i));
        }
    }
    return nullptr;
}

irNode* parseNodes(Node* ptr)
{
    if (ptr->type == "AddOP") {
        irNode node("addExpression");
    }
    else if (ptr->type == "SubOP") {
        irNode node("subExpression");
    }
    else if (ptr->type == "MultOP") {

    }
    else if (ptr->type == "DivOP") {

    }
    else if (ptr->type == "AndOP") {

    }
    else if (ptr->type == "OrOP") {

    }
    else if (ptr->type == "LesserOP") {

    }
    else if (ptr->type == "GreaterOP") {

    }
    else if (ptr->type == "EqualsOP") {

    }
    else if (ptr->type == "ArrayIndexAccessExpression") {

    }
    else if (ptr->type == "IdentifierExpression") {

    }
    else if (ptr->type == "ThisExpression") {

    }
    else if (ptr->type == "newIntArray") {

    }
    else if (ptr->type == "newIdentifier") {

    }
    else if (ptr->type == "NotOP") {

    }
    else if (ptr->type == "Expression") {

    }
    // Statements
    else if (ptr->type == "SystemOutPrintStatement") {

    }
    else if (ptr->type == "AssignStatement") {

    }
    else if (ptr->type == "WhileStatement") {

    }
    else if (ptr->type == "IF_ElseStatement") {

    }
    else if (ptr->type == "ArrayIndexAssignStatement") {

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


#ifndef CFG_H
#define CFG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

#include "tac.h"
#include "Node.h"
#include "irNode.h"
#include "bblock.h"

class CFG
{
private:
    int blockNr;
    BBlock *root;
    BBlock *currentBlk;
    std::vector<tac *> temp;
    std::string genBlkName();
    bool isExpressionHeadNode(Node *ptr);
    bool isLeafNode(Node *ptr);
    bool isUnaryHeadNode(Node *ptr);
    std::string tacExpression(Node *ptr);

    Node *nRoot;
    irNode *iRoot;
    std::vector<std::string> postOrderList;
    std::vector<irNode> postOrderNodes;

public:
    CFG();
    CFG(Node *root);
    void buildIrNodeAST(Node* node, irNode* iNode);
    void buildIrNodeAST(Node* node);
    void buildCFG();
    void buildCFGrec(Node *ptr);
    void postOrderTraversal(Node *leaf);
    irNode* parseNodes(Node* ptr);
    void printPostOrder();
    Node* searchTree(Node* root, std::string value);

    void generate_tree(irNode* node) {
	    std::ofstream outStream;
	    outStream.open("irTree.dot");

	    int count = 0;
	    outStream << "digraph {" << std::endl;
	    generate_tree_content(count, &outStream, node);
	    outStream << "}" << std::endl;
	    outStream.close();

	    std::cout << "\nBuilt a IR parse-tree:" << std::endl;
    }

    void generate_tree_content(int &count, ofstream *outStream, irNode* node) {
      int thisId = count++;
      node->id = count++;
      *outStream << "n" << thisId << " [label=\"" << node->type << ":" << node->name << "\"];" << endl;

	  for (int i = 0; i < node->child.size(); i++)
	  {
		  generate_tree_content(count, outStream, node);
		  *outStream << "n" << thisId << " -> n" << node->id << endl;
	  }
    }
};

#endif

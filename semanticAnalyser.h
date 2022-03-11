
#ifndef SEMANTIC_ANALYSER_HPP
#define SEMANTIC_ANALYSER_HPP

#include "symbolTable.h"

class semanticAnalyser
{
private:
   Node *root;
   Node *current;
   symbolTable ST;

public:
    semanticAnalyser(symbolTable &ST, Node *root);
    bool analyse();
    int typeCheck();
};

#endif
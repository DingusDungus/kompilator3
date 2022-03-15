#ifndef BBLOCK_H
#define BBLOCK_H

#include <iostream>
#include <vector>

#include "tac.h"
#include "Node.h"

class BBlock
{
public:
    std::string name;
    std::vector<tac *> instructions;
    tac *condition;
    int tempCount;
    BBlock *trueExit, *falseExit;
    BBlock() : trueExit(nullptr), falseExit(nullptr), tempCount(0) {}
    BBlock(std::string name) : name(name), trueExit(nullptr), falseExit(nullptr), tempCount(0) {}
    BBlock(std::string name, BBlock *trueExit, BBlock *falseExit) : name(name), trueExit(trueExit), falseExit(falseExit), tempCount(0) {}
};

#endif
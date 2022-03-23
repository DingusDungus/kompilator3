#ifndef BBLOCK_H
#define BBLOCK_H

#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

#include "tac.h"
#include "Node.h"

class BBlock
{
public:
    int id;
    std::string name;
    bool visited;
    std::vector<tac *> instructions;
    tac *condition;
    int tempCount;
    BBlock *trueExit, *falseExit;
    BBlock() : trueExit(nullptr), falseExit(nullptr), tempCount(0), visited(true) {}
    BBlock(std::string name) : name(name), trueExit(nullptr), falseExit(nullptr), tempCount(0), visited(true) {}
    BBlock(std::string name, BBlock *trueExit, BBlock *falseExit) : name(name), trueExit(trueExit), falseExit(falseExit), tempCount(0), visited(true) {}

    int generate_tree(ofstream &outStream, int count)
    {
        generate_tree_content(count, &outStream);
        return count;
        std::cout << "\nBuilt a CFG parse-tree:" << std::endl;
    }

    void generate_tree_content(int &count, ofstream *outStream)
    {
        id = count++;
        visited = false;
        *outStream << "n" << id << " [label=\"" << name << "\n\l";
        for (int i = 0; i < instructions.size(); i++)
        {
            *outStream << instructions[i]->result << " : " << instructions[i]->lhs << " : " << instructions[i]->op << " : " << instructions[i]->rhs << "\l" << endl;
        }
        *outStream << "\"];" << endl;
        if (trueExit != nullptr && trueExit->visited != false)
        {
            trueExit->generate_tree_content(count, outStream);
            *outStream << "n" << id << " -> n" << trueExit->id << endl;
        }
        else if (trueExit != nullptr && trueExit->visited != true)
        {
            *outStream << "n" << trueExit->id << " -> n" << id << endl;
        }

        if (falseExit != nullptr && falseExit->visited != false )
        {
            falseExit->generate_tree_content(count, outStream);
            *outStream << "n" << id << " -> n" << falseExit->id << endl;
        }
        else if (falseExit != nullptr && falseExit->visited != true)
        {
            *outStream << "n" << falseExit->id << " -> n" << id << endl;
        }
    }
};

#endif
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

    void generate_tree()
    {
        std::ofstream outStream;
        outStream.open("cfgTree.dot");

        int count = 0;
        outStream << "digraph {" << std::endl;
        generate_tree_content(count, &outStream);
        outStream << "}" << std::endl;
        outStream.close();

        std::cout << "\nBuilt a CFG parse-tree:" << std::endl;
    }

    void generate_tree_content(int &count, ofstream *outStream)
    {
        id = count++;
        visited = true;
        *outStream << "n" << id << " [label=\"";
        for (int i = 0; i < instructions.size(); i++)
        {
            *outStream << instructions[i]->result << " : " << instructions[i]->lhs << " : " << instructions[i]->op << " : " << instructions[i]->rhs << "\l" << endl;
        }
        *outStream << "\"];" << endl;
        if (trueExit->visited != true)
        {
            trueExit->generate_tree_content(count, outStream);
            *outStream << "n" << id << " -> n" << trueExit->id << endl;
        }
        else
        {
            *outStream << "n" << trueExit->id << " -> n" << id << endl;
        }

        if (falseExit->visited != true)
        {
            falseExit->generate_tree_content(count, outStream);
            *outStream << "n" << id << " -> n" << falseExit->id << endl;
        }
        else
        {
            *outStream << "n" << falseExit->id << " -> n" << id << endl;
        }
    }
};

#endif
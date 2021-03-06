#include <iostream>
#include <vector>
#include "parser.tab.hh"
#include "symbolTable.h"
#include "semanticAnalyser.h"
#include "cfg.h"
#include "interpreter.h"

extern Node *root;
extern FILE *yyin;

int blockNr = 0;
int tempNr = 0;
int printNr = 0;
std::vector<BBlock*> methodDecBlocks;
symbolTable SYMBOL_TABLE;

void yy::parser::error(std::string const &err)
{
  std::cout << "Cannot generate a syntax tree for this input: " << err << std::endl;
}

int main(int argc, char **argv)
{
  //Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
  symbolTable ST;
  if (argc > 1)
  {
    if (!(yyin = fopen(argv[1], "r")))
    {
      perror(argv[1]);
      return 1;
    }
  }

  yy::parser parser;

  if (!parser.parse())
  {
    root->generate_tree();
    root->generate_parents(0, root);
  }
  ST.init(root);
  ST.printSTtree();
  SYMBOL_TABLE = ST;
  semanticAnalyser SA(ST, root);
  interpreter it;
  if (SA.analyse() == -1)
  {
    std::cout << "Duplicate found!!\n";
    return 1;
  }
  CFG cfgGraph(root);
  cfgGraph.buildIrNodeAST(root);
  cfgGraph.buildCFG();
  cfgGraph.printBlocks();
  // ST.printSTtree();
  cfgGraph.generate_tree();

  // cfgGraph.genByteCode(); // disable for testing IR on Cambridge files.
  // it.interpret(); // disable for testing IR on Cambridge files.

  return 0;
}

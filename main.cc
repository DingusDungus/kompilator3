#include <iostream>
#include "parser.tab.hh"
#include "symbolTable.h"
#include "semanticAnalyser.h"

extern Node *root;
extern FILE *yyin;

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
  semanticAnalyser SA(ST, root);
  if (SA.analyse() == -1)
  {
    std::cout << "Duplicate found!!\n";
    return 1;
  }

  return 0;
}
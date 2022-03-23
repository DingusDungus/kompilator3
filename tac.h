#ifndef TAC_H
#define TAC_H

#include <iostream>
#include <vector>
#include "symbolTable.h"

symbolTable extern SYMBOL_TABLE;

class tac
{
public:
    std::string op, lhs, rhs, result;
    bool lhsSet;
    virtual void dump()
    {
        std::cout << result << ":=" << lhs << op << rhs << std::endl;
        std::string tacVal = "( " + lhs + " " + op + " " + rhs + " )";
        variable* var = new variable(result,tacVal);
        SYMBOL_TABLE.putTemps(result,var);
    }
};

class expression : public tac
{
public:
    expression(std::string _op, std::string _y, std::string _z, std::string _result)
    {
        op =_op;
        lhs = _y;
        rhs = _z;
        result = _result;
    }
};

class parameter : public tac
{
public:
    parameter(std::string _p)
    {
        op = "param";
        lhs = "";
        rhs = _p;
        result = "";
    }
    void dump() override 
    {
        std::cout << op << " " << rhs << std::endl;
    }
};
class methodCallTac : public tac
{
public:
    methodCallTac(std::string _f, std::string _n, std::string _result)
    {
        op = "call";
        lhs = _f;
        rhs = _n;
        result = _result;
    }
    void dump() override 
    {
        std::cout << result << ":=" << op << " " << lhs << " " << rhs << std::endl;
    }
};

class jump : public tac
{
public:
    jump(std::string _label)
    {
        op = "goto";
        result = _label;
    }
};

class condJump : public tac
{
public:
    condJump(std::string _op, std::string _x, std::string _result)
    {
        op = _op;
        lhs = _x;
        result = _result;
    }
};

#endif

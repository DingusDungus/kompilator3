#ifndef TAC_H
#define TAC_H

#include <iostream>
#include <vector>

class tac
{
public:
    std::string op, lhs, rhs, result;
    bool lhsSet;
    void dump()
    {
        std::cout << result << ":=" << lhs << op << rhs << std::endl;
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

class methodCall : public tac
{
public:
    methodCall(std::string _f, std::string _n, std::string _result)
    {
        op = "call";
        lhs = _f;
        rhs = _n;
        result = _result;
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

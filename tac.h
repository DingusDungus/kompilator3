#ifndef TAC_H
#define TAC_H

#include <iostream>
#include <vector>
#include "symbolTable.h"

symbolTable extern SYMBOL_TABLE;

struct byteCode
{
    byteCode() {}
    byteCode(int index, std::string instruction) : index(index), instruction(instruction) {}
    int index;
    std::string instruction;
};

class tac
{
public:
    std::string op, lhs, rhs, result;
    std::vector<byteCode *> bytecodes;
    bool lhsSet;
    virtual void dump()
    {
        std::cout << result << ":=" << lhs << op << rhs << std::endl;
        std::string tacVal = "( " + lhs + " " + op + " " + rhs + " )";
        variable *var = new variable(result, tacVal);
        SYMBOL_TABLE.putTemps(result, var);
    }
    bool isNumber(const std::string &str)
    {
        for (char const &c : str)
        {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }
    std::string decideOp()
    {
        if (op == "AddOP")
        {
            return "iadd ";
        }
        else if (op == "SubOP")
        {
            return "isub ";
        }
        else if (op == "MultOP")
        {
            return "imul ";
        }
        else if (op == "DivOP")
        {
            return "idiv ";
        }
        else if (op == "AndOP")
        {
            return "iand ";
        }
        else if (op == "OrOP")
        {
            return "ior ";
        }
        else if (op == "NotOP")
        {
            return "inot ";
        }
        else if (op == "LesserOp")
        {
            return "ilt ";
        }
        else if (op == "GreaterOp")
        {
            return "igt ";
        }
        else if (op == "EqualsOp")
        {
            return "ieq ";
        }
        else
        {
            return "";
        }
    }
    std::string decideIn()
    {
        if (result == "print")
        {
            return "print";
        }
        else if (result == "ifElse")
        {
            return "";
        }
        else if (result == "while")
        {
            return "";
        }
        else if (result == "stop")
        {
            return "stop";
        }
        else
        {
            return "istore";
        }
    }

    int decideId(std::string name)
    {
        if (name == "AddOP")
        {
            return 3;
        }
        else if (name == "SubOP")
        {
            return 4;
        }
        else if (name == "MultOP")
        {
            return 5;
        }
        else if (name == "DivOP")
        {
            return 6;
        }
        else if (name == "AndOP")
        {
            return 7;
        }
        else if (name == "OrOP")
        {
            return 8;
        }
        else if (name == "NotOP")
        {
            return 9;
        }
        else if (name == "LesserOp")
        {
            return 10;
        }
        else if (name == "GreaterOp")
        {
            return 11;
        }
        else if (name == "EqualsOp")
        {
            return 12;
        }
        else if (name == "print")
        {
            return 13;
        }
        else if (name == "ifElse")
        {
            return 14;
        }
        else if (name == "while")
        {
            return 15;
        }
        else if (name == "stop")
        {
            return 16;
        }
        else
        {
            return 17; // istore
        }
    }

    std::string generate_code()
    {
        if (lhs.length() > 0)
        {
            if (isNumber(lhs) || lhs == "true" || lhs == "false")
            {
                if (lhs == "true")
                {
                    lhs = "1";
                }
                else if (lhs == "false")
                {
                    lhs = "0";
                }
                byteCode *newByteCode = new byteCode(1, "iconst " + lhs);
                bytecodes.push_back(newByteCode);
            }
            else
            {
                byteCode *newByteCode = new byteCode(0, "iload " + rhs);
                bytecodes.push_back(newByteCode);
            }
        }
        if (rhs.length() > 0)
        {
            if (isNumber(rhs) || rhs == "true" || rhs == "false")
            {
                if (rhs == "true")
                {
                    rhs = "1";
                }
                else if (rhs == "false")
                {
                    rhs = "0";
                }
                byteCode *newByteCode = new byteCode(2, "iconst " + rhs);
                bytecodes.push_back(newByteCode);
            }
            else
            {
                byteCode *newByteCode = new byteCode(0, "iload " + rhs);
                bytecodes.push_back(newByteCode);
            }
        }
        if (op.length() > 0)
        {
            byteCode *newByteCode = new byteCode(decideId(rhs), (decideOp() + rhs));
            bytecodes.push_back(newByteCode);
        }
        if (result.length() > 0)
        {
            std::string instruction = decideIn();
            if (instruction == "print" || instruction == "stop")
            {
                byteCode *newByteCode = new byteCode(decideId(result), instruction);
                bytecodes.push_back(newByteCode);
            }
            else
            {
                byteCode *newByteCode = new byteCode(17, ("istore " + result));
                bytecodes.push_back(newByteCode);
            }
        }
        std::string codeString;
        for (int i = 0; i < bytecodes.size(); i++)
        {
            codeString += (std::to_string(bytecodes[i]->index) + " " + bytecodes[i]->instruction + "\n"); 
        }
        return codeString;
    }
};

class expression : public tac
{
public:
    expression(std::string _op, std::string _y, std::string _z, std::string _result)
    {
        op = _op;
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

#include "interpreter.h"

interpreter::interpreter()
{
    lhs = 0;
    rhs = 0;
    temp = 0;
    isLhs = true;
}

void interpreter::parseLine(std::string line)
{
    using namespace std;
    std::istringstream iss(line);
    std::string s;
    while (std::getline(iss, s, ' '))
    {
        lineVector.push_back(s);
    }
}

bool interpreter::parse(std::string line)
{
    parseLine(line);
    int id = std::stoi(lineVector[0]);
    if (id == 0)
    {
        load();
    }
    else if (id == 1)
    {
        iconst();
    }
    else if (id == 2)
    {
    }
    else if (id == 3)
    {
        addOp();
    }
    else if (id == 4)
    {
        subOp();
    }
    else if (id == 5)
    {
        mulOp();
    }
    else if (id == 6)
    {
        divOp();
    }
    else if (id == 7)
    {
        andOp();
    }
    else if (id == 8)
    {
        orOp();
    }
    else if (id == 9)
    {
        notOp();
    }
    else if (id == 10)
    {
        ltOp();
    }
    else if (id == 11)
    {
        gtOp();
    }
    else if (id == 12)
    {
        eqOp();
    }
    else if (id == 13)
    {
        print();
    }
    else if (id == 16)
    {
        return false;
    }
    else if (id == 17)
    {
        store();
    }
    lineVector.clear();
    return true;
}

int interpreter::addOp()
{
    temp = lhs + rhs;
}
int interpreter::subOp()
{
    temp = lhs - rhs;
}
int interpreter::mulOp()
{
    temp = lhs * rhs;
}
int interpreter::divOp()
{
    temp = lhs / rhs;
}
int interpreter::andOp()
{
    if (lhs == 1 && rhs == 1)
    {
        temp = 1;
    }
    else
    {
        temp = 0;
    }
}
int interpreter::orOp()
{
    if (lhs == 1 || rhs == 1)
    {
        temp = 1;
    }
    else
    {
        temp = 0;
    }
}
int interpreter::notOp()
{
    if (lhs == 1)
    {
        temp = 0;
    }
    else
    {
        temp = 1;
    }
}
int interpreter::ltOp()
{
    if (lhs < rhs)
    {
        temp = 1;
    }
    else
    {
        temp = 0;
    }
}
int interpreter::gtOp()
{
    if (lhs > rhs)
    {
        temp = 1;
    }
    else
    {
        temp = 0;
    }
}
int interpreter::eqOp()
{
    if (lhs == rhs)
    {
        temp = 1;
    }
    else
    {
        temp = 0;
    }
}
void interpreter::print()
{
    std::cout << temp << std::endl;
}
void interpreter::load()
{
    if (lineVector[2] != "")
    {
        pop();
    }
}
void interpreter::store()
{
    put();
}
void interpreter::stop()
{
}
void interpreter::iconst()
{
    if (isLhs)
    {
        lhs = std::stoi(lineVector[2]);
        isLhs = false;
    }
    else
    {
        rhs = std::stoi(lineVector[2]);
        isLhs = true;
    }
}

stackEntry *interpreter::searchStack(std::string name)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (name == entries[i]->varName)
        {
            return entries[i];
        }
    }
    return nullptr;
}

void interpreter::pop()
{
    if (isLhs)
    {
        stackEntry *entry = entries[entries.size() - 1];
        lhs = entry->value;
        entries.pop_back();
        isLhs = false;
    }
    else
    {
        stackEntry *entry = entries[entries.size() - 1];
        rhs = entry->value;
        entries.pop_back();
        isLhs = true;
    }
}
void interpreter::put()
{
    if (lineVector[2].length() > 0)
    {
        stackEntry *var = searchStack(lineVector[2]);
        if (var != nullptr)
        {
            std::cout << temp << "\n";
            var->value = temp;
        }
        else
        {
            var = new stackEntry(lineVector[2], temp);
            entries.push_back(var);
        }
    }
}

void interpreter::interpret()
{
    std::string line;
    byteCode.open("byteCode.o");
    if (!byteCode.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    while (std::getline(byteCode, line))
    {
        if (!parse(line))
        {
            break;
        }
        isLhs = true;
    }
    byteCode.close();
}
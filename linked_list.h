#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

struct entry
{
    double value;
    std::string name;
    entry *next;
    entry *before;
    entry() : value(0), name(""), next(nullptr) {}
    entry(std::string name, double value) : name(name), value(value), next(nullptr), before(nullptr) {}
};

class linked_list
{
private:
    entry *start;
    entry *walker;

public:
    linked_list();
    void put(std::string name, double value);
    entry *pop(std::string name);
    void printEntries();
};

#endif
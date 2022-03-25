#include "linked_list.h"

linked_list::linked_list()
{
    start = nullptr;
    walker = nullptr;
}

void linked_list::put(std::string name, double value)
{
    entry *newEntry = new entry(name, value);
    if (start == nullptr)
    {
        start = newEntry;
    }
    else
    {
        newEntry->next = start;
        start->before = newEntry;
        start = newEntry;
    }
}

entry *linked_list::pop(std::string name)
{
    if (start == nullptr)
    {
        return nullptr;
    }
    else
    {
        walker = start;
        while (walker != nullptr)
        {
            if (walker->name == name)
            {
                if (walker != start)
                {
                    walker->before->next = walker->next;
                    if (walker->next != nullptr)
                    {
                        walker->next = walker->before->next;
                    }
                }
                else
                {
                    start = walker->next;
                }
                return walker;
            }
            walker = walker->next;
        }
    }
    return nullptr;
}

void linked_list::printEntries()
{
    walker = start;
    while (walker != nullptr)
    {
        std::cout << "Entry: " << walker->name << " value: " << std::to_string(walker->value) << std::endl;  
    }
}

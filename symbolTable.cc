#include "symbolTable.h"
#include "scope.h"
#include <string>

void symbolTable::stBuilderRec(Node *walker, Node *parent)
{
    for (auto next = walker->children.begin(); next != walker->children.end(); next++)
    {
        if ((*next)->type == "VarDeclaration" || (*next)->type == "TypeIdentifier")
        {
            variable *newRecord = new variable();
            auto child = (*next)->children.begin();
            if ((*child)->type != "identifierType")
            {
                newRecord->type = (*child)->value;
            }
            else
            {
                auto temp = (*child)->children.begin();
                newRecord->type = (*temp)->value;
            }
            child++;
            newRecord->id = (*child)->value;
            put(newRecord->id, newRecord);
            declared(newRecord->id);

            // std::cout << "Record: type: " << newRecord->type << " id: " << newRecord->id << std::endl;
        }
        else if ((*next)->type == "MainClass")
        {
            Class *newRecord = new Class();
            auto child = (*next)->children.begin();
            newRecord->id = (*child)->value;
            newRecord->type = "MainClass";
            put(newRecord->id, newRecord);
            // std::cout << "Record: type: " << newRecord->type << " id: " << newRecord->id << std::endl;
            declared(newRecord->id);
            enterScope();
            current->scopeRecord = newRecord;
            stBuilderRec((*next), walker);
            exitScope();
        }
        else if ((*next)->type == "PublicMainMethod")
        {
            method *newRecord = new method();
            auto child = (*next)->children.begin();
            newRecord->id = "main";
            newRecord->type = "PublicMainMethod";
            newRecord->parameters.insert({(*child)->value, "String"});
            put(newRecord->id, newRecord);
            // std::cout << "Record: type: " << newRecord->type << " id: " << newRecord->id << std::endl;
            declared(newRecord->id);
            enterScope();
            current->scopeRecord = newRecord;
            stBuilderRec((*next), walker);
            exitScope();
        }
        else if ((*next)->type == "ClassDeclaration")
        {
            Class *newRecord = new Class();
            auto child = (*next)->children.begin();
            newRecord->id = (*child)->value;
            newRecord->type = "Class";
            put(newRecord->id, newRecord);
            // std::cout << "Record: type: " << newRecord->type << " id: " << newRecord->id << std::endl;
            declared(newRecord->id);
            enterScope();
            current->scopeRecord = newRecord;
            stBuilderRec((*next), walker);
            exitScope();
        }
        else if ((*next)->type == "MethodDeclaration")
        {
            method *newRecord = new method();
            auto child = (*next)->children.begin();
            auto baseChild = child;
            while ((*child)->children.empty() == false)
            {
                child = (*child)->children.begin();
                if ((*child)->children.empty())
                {
                    break;
                }
            }
            newRecord->type = (*child)->value;
            child = baseChild;
            child++;
            newRecord->id = (*child)->value;
            put(newRecord->id, newRecord);
            // std::cout << "Record: type: " << newRecord->type << " id: " << newRecord->id << std::endl;
            declared(newRecord->id);
            enterScope();
            child++;
            addParameters((*child), newRecord);
            current->scopeRecord = newRecord;
            stBuilderRec((*next), walker);
            exitScope();
        }
        else
        {
            stBuilderRec((*next), walker);
        }
    }
}

void symbolTable::addParameters(Node *ptr, method *scopeRecord)
{
    if (ptr->type == "TypeIdentifier")
    {
        auto child = ptr->children.begin();
        std::string type = (*child)->value;
        child++;
        std::string id = (*child)->value;
        scopeRecord->parameters.insert({id, type});
    }

    for (auto i = ptr->children.begin(); i != ptr->children.end(); i++)
    {
        addParameters((*i), scopeRecord);
    }
}

void symbolTable::stBuilder()
{
    Node *walker = nodeRoot;
    stBuilderRec(walker, walker);
}

symbolTable::symbolTable()
{
    root = new scope();
    current = root;
}

void symbolTable::enterScope() { current = current->nextChild(); }
void symbolTable::exitScope() { current = current->Parent(); }

void symbolTable::init(Node *root)
{
    nodeRoot = root;
    stBuilder();
}

void symbolTable::put(std::string key, record *item)
{
    current->put(key, item);
}

record *symbolTable::lookup(std::string key)
{
    return current->lookup(key);
}

void symbolTable::printSTtree()
{
    std::cout << "\n\n----- Symbol Table start -----" << std::endl;
    root->printRecords();
    std::cout << "----- Symbol Table end -----\n\n"
              << std::endl;
}

void symbolTable::resetScopes()
{
    root->resetScope();
    current = root;
}

scope *symbolTable::operator[](int index)
{
    if (index < current->children.size())
    {
        return current->children[index];
    }
    else
    {
        return nullptr;
    }
}

scope *symbolTable::getCurrent()
{
    return current;
}

int symbolTable::size()
{
    return current->children.size();
}

void symbolTable::declared(std::string key)
{
    if (current->declarationCount.count(key) == 0)
    {
        current->declarationCount.insert({key, 1});
    }
    else
    {
        auto var = current->declarationCount.find(key);
        current->declarationCount[var->first] = 2;
    }
}

bool symbolTable::duplicatesFound()
{
    scope *ptr = root;
    bool returnBool = false;
    for (auto i = ptr->declarationCount.begin(); i != ptr->declarationCount.end(); i++)
    {
        if (i->second > 1)
        {
            std::cout << "Error: Duplicate declaration " << i->first << " found in " << ptr->scopeRecord->type << " " << ptr->scopeRecord->id << "!\n";
            returnBool = true;
        }
    }
    for (int i = 0; i < ptr->children.size(); i++)
    {
        returnBool = duplicatesFoundRec(ptr->children[i]);
    }

    return returnBool;
}
bool symbolTable::duplicatesFoundRec(scope *ptr)
{
    bool returnBool = false;
    for (auto i = ptr->declarationCount.begin(); i != ptr->declarationCount.end(); i++)
    {
        if (i->second > 1)
        {
            std::cout << "Error: Duplicate declaration " << i->first << " found in " << ptr->scopeRecord->type << " " << ptr->scopeRecord->id << "!\n";
            returnBool = true;
        }
    }
    for (int i = 0; i < ptr->children.size(); i++)
    {
        returnBool = duplicatesFoundRec(ptr->children[i]);
    }
    return returnBool;
}

bool symbolTable::testType(Node *ptr, std::string type)
{
    if (expressionCheckRecNode(ptr))
    {
        return true;
    }
    // debug below
    // std::cout << "(debug) EXPElements: ";
    // for (int i = 0; i < expressionElements.size(); i++)
    // {
        // std::cout << expressionElements[i] << " ";
    // }
    // std::cout << "\n";
    // debug above
    if (expressionElements.size() > 0)
    {
        for (int i = 0; i < expressionElements.size(); i++)
        {
            if (type != expressionElements[i])
            {
                std::cout << "Error; row: " << ptr->value << ", Expression in "
                          << current->scopeRecord->type
                          << " "
                          << current->scopeRecord->id
                          << " consists of differing types, got: "
                          << expressionElements[i]
                          << ", expected: "
                          << type
                          << std::endl;
                expressionElements.clear();
                return true;
            }
        }
    }
    expressionElements.clear();
    return false;
}
bool symbolTable::testType(Node *ptr)
{
    if (expressionCheckRecNode(ptr))
    {
        return true;
    }
    if (expressionElements.size() > 0)
    {
        // debug below
        // std::cout << "(debug) EXPElements: ";
        // for (int i = 0; i < expressionElements.size(); i++)
        // {
            // std::cout << expressionElements[i] << " ";
        // }
        // std::cout << "\n";
        // debug above
        std::string type = expressionElements[0];
        for (int i = 0; i < expressionElements.size(); i++)
        {
            if (type != expressionElements[i])
            {
                std::cout << "Error; row: " << ptr->value << ", Expression in "
                          << current->scopeRecord->type
                          << " "
                          << current->scopeRecord->id
                          << " consists of differing types, got: "
                          << expressionElements[i]
                          << ", expected: "
                          << type
                          << std::endl;
                expressionElements.clear();
                return true;
            }
        }
    }
    expressionElements.clear();
    return false;
}
bool symbolTable::testTypeIdentifier(Node *ptr)
{
    auto child = ptr->children.begin();
    record *varRecord = current->lookup((*child)->value);
    child++;
    if (expressionCheckRecNode((*child)))
    {
        return true;
    }
    // for (int i = 0; i < expressionElements.size(); i++)
    // {
    // std::cout << expressionElements[i] << " ";
    // }
    // std::cout << "\n";
    if (expressionElements.size() > 0)
    {
        std::string type = varRecord->type;
        for (int i = 0; i < expressionElements.size(); i++)
        {
            if (type != expressionElements[i])
            {
                std::cout << "Error; row: " << ptr->value << ", assignStatement in "
                          << current->scopeRecord->type
                          << " "
                          << current->scopeRecord->id
                          << " expression doesn't match identifier type\n";
                expressionElements.clear();
                return true;
            }
        }
        expressionElements.clear();
        return false;
    }
}

method *symbolTable::methodLookup(std::string className, std::string methodName)
{
    method *methodRecord = nullptr;
    scope *walker = nullptr;
    if (className != "this")
    {
        // std::cout << "MethLookUp: className: " << className << " methodName: " << methodName << std::endl;
        std::string realClassName = "";
        bool classFound = false;
        record *objectRecord = nullptr;
        // take class name if it is actually a object identifier
        // and look up the actual class it is an object of
        // and put into object into classRecord.
        objectRecord = lookup(className);
        if (objectRecord != nullptr)
        {
            // std::cout << "(debug) realClassName: " << objectRecord->type << std::endl;
            realClassName = objectRecord->type;
            if (realClassName != "Class")
            {
                // std::cout << "(debug) Fetched object: "
                          // << className
                          // << " actual class type: "
                          // << realClassName
                          // << std::endl;
                // std::cout << "(debug) Changed className to : " << realClassName << std::endl;
                className = realClassName;
            }
        }
        for (int i = 0; i < root->children.size(); i++)
        {
            // std::cout << "scopeId: " << root->children[i]->scopeRecord->id << std::endl;
            if (root->children[i]->scopeRecord->id == className)
            {
                walker = root->children[i];
                classFound = true;
                // std::cout << "(debug) Class '"
                          // << className
                          // << "' found!!"
                          // << std::endl;
                methodRecord = (method *)walker->lookup(methodName);
                if (methodRecord != nullptr)
                {
                    // std::cout << "(debug) Method '"
                              // << methodRecord->id
                              // << "' found!!"
                              // << std::endl;
                }
                break;
            }
        }
        if (!classFound)
        {
            std::cout << "Error; class: " << className << " was not found!\n";
            return nullptr;
        }
    }
    else
    {
        methodRecord = (method *)current->lookup(methodName);
    }
    return methodRecord;
}

std::string symbolTable::getType(Node *ptr)
{
    std::string type = "";
    if (ptr->type == "Identifier")
    {
        record *identifierVal = lookup(ptr->value);
        if (identifierVal != nullptr)
        {
            return identifierVal->type;
        }
        else
        {
            std::cout << "Error: variable " << ptr->value << " is not declared\n";
            return "";
        }
    }
    else if (ptr->type == "IntegerLiteral")
    {
        return "int";
    }
    else if (ptr->type == "BooleanExpression")
    {
        return "boolean";
    }
    else if (ptr->type == "ThisExpression")
    {
        return current->scopeRecord->id;
    }
    else if (ptr->type == "intArray")
    {
        return "int";
    }
    for (auto i = ptr->children.begin(); i != ptr->children.end(); i++)
    {
        type = getType((*i));
    }
    return type;
}

std::vector<std::string> symbolTable::getParams(Node *ptr, std::vector<std::string> params)
{
    // if not empty
    if (ptr->children.empty() == false)
    {
        auto expressionList = ptr->children.begin();
        for (auto i = (*expressionList)->children.begin(); i != (*expressionList)->children.end(); i++)
        {
            params.push_back(getType((*i)));
        }
    }
    return params;
}

bool symbolTable::expressionCheck()
{
    current = root;
    Node *nodePtr = nodeRoot;
    resetScopes();
    bool returnBool;
    return expressionCheckRec(nodePtr);
}

bool symbolTable::testMethodCallParams(Node *ptr)
{
    if (ptr->children.empty() == false)
    {
        auto child = ptr->children.begin();
        child++;
        child++;
        if ((*child)->children.empty() == false)
        {
            auto expressionList = (*child)->children.begin();
            for (auto i = (*expressionList)->children.begin(); i != (*expressionList)->children.end(); i++)
            {
                if (testType((*i)))
                {
                    std::cout << "Error; methodcall parameter expression has differing types!\n";
                    return true;
                }
            }
        }
    }
    return false;
}

bool symbolTable::varIsBool(Node *ptr)
{
    Node *id = (*ptr->children.begin());
    record *idRecord = lookup(id->value);
    if (idRecord != nullptr && !(idRecord->type == "boolean"))
    {
        return false;
    }
    else if (idRecord == nullptr)
    {
        return false;
    }
}

bool symbolTable::isBoolChildren(Node *ptr)
{
    for (auto i = ptr->children.begin(); i != ptr->children.end(); i++)
    {
        if (!((*i)->type == "AndOP" || (*i)->type == "OrOP" || (*i)->type == "NotOP" || (*i)->type == "GreaterOP" || (*i)->type == "LesserOP" || (*i)->type == "EqualsOP" || (*i)->type == "BooleanExpression" || (*i)->type == "IdentifierExpression" || (*i)->type == "Expression"))
        {
            return false;
        }
        if ((*i)->type == "IdentifierExpression")
        {
            Node *id = (*(*i)->children.begin());
            record *idRecord = lookup(id->value);
            if (idRecord != nullptr && !(idRecord->type == "boolean"))
            {
                return false;
            }
            else if (idRecord == nullptr)
            {
                std::cout << "Error; " << id->value << " is undefined in boolean expression!\n";
                return false;
            }
        }
        else if ((*i)->type == "Expression" && (*i)->value == "")
        {
            // std::cout << "Here! " << (*i)->value << "\n";
            return isBoolChildren(*i);
        }
    }
    return true;
}

bool symbolTable::testBoolEquals(Node *ptr)
{
    for (auto i = ptr->children.begin(); i != ptr->children.end(); i++)
    {
        if ((*i)->type == "IdentifierExpression")
        {
            Node *id = (*(*i)->children.begin());
            record *idRecord = lookup(id->value);
            if (idRecord != nullptr && !(idRecord->type == "boolean"))
            {
                return false;
            }
            else if (idRecord == nullptr)
            {
                std::cout << "Error; " << id->value << " is undefined in boolean expression!\n";
                return false;
            }
        }
        else if ((*i)->type == "BooleanExpression" || (*i)->value == "AndOP" || (*i)->value == "OrOP" || (*i)->value == "NotOP")
        {
            // do nothing
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool symbolTable::equalsTestType(Node *ptr)
{
    bool isBooleanExpression = testBoolEquals(ptr);
    if (!isBooleanExpression)
    {
        if (!testType(ptr, "int"))
        {
            return false;
        }
        return true;
    }
    return false;
}

bool symbolTable::isExpression(Node *ptr)
{
    if (ptr->type == "AndOP" || ptr->type == "OrOP" || ptr->type == "LesserOP" || ptr->type == "GreaterOP" || ptr->type == "EqualsOP" || ptr->type == "OrOP" || ptr->type == "AddOP" || ptr->type == "SubOP" || ptr->type == "OrOP" || ptr->type == "MultOP" || ptr->type == "DivOP" || ptr->type == "ArrayIndexAccessExpression" || ptr->type == "newIntArray" || ptr->type == "newIdentifier" || ptr->type == "NotOP")
    {
        return true;
    }
    return false;
}

bool symbolTable::expressionCheckRec(Node *nodePtr)
{
    bool returnBool;
    for (auto next = nodePtr->children.begin(); next != nodePtr->children.end(); next++)
    {
        if (isExpression((*next)))
        {
            if (isBoolChildren((*next)))
            {
                return false;
            }
            if (testType((*next)))
            {
                returnBool = true;
            }
            else if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            expressionElements.clear();
        }
        else if ((*next)->type == "IF_ElseStatement" || (*next)->type == "WhileStatement")
        {
            std::string statementType = (*next)->type;
            if (testBoolExpression((*next)))
            {
                std::cout << "Error; row: "
                          << (*next)->value
                          << ", boolean expression incorrect"
                          << " in: "
                          << statementType
                          << std::endl;
                returnBool = true;
            }
            else if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
        }
        else if ((*next)->type == "AssignStatement")
        {
            auto child = (*next)->children.begin();
            child++;
            if (testType((*next)))
            {
                std::cout << "Error; row: " << (*next)->value << ", assign-statement incorrect\n";
                returnBool = true;
            }
            else if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            expressionElements.clear();
        }
        else if ((*next)->type == "ArrayIndexAssignStatement")
        {
            if (testType((*next), "int"))
            {
                std::cout << "Error; row: " << (*next)->value << ", array index assigned non int value\n";
                returnBool = true;
            }
            auto child = (*next)->children.begin();
            child++;
            expressionCheckRecNode((*child));
            if (expressionElements.size() > 0)
            {
                std::string type = "int";
                for (int i = 0; i < expressionElements.size(); i++)
                {
                    if (type != expressionElements[i])
                    {
                        std::cout << "Error: Assign statement in "
                                  << current->scopeRecord->type
                                  << " "
                                  << current->scopeRecord->id
                                  << " expression doesn't match identifier type\n";
                        returnBool = true;
                    }
                }
                expressionElements.clear();
            }
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
        }
        else if ((*next)->type == "SystemOutPrintStatement")
        {
            auto child = (*next)->children.begin();
            if ((*child)->type != "MethodCall")
            {
                if (testType((*next), "int"))
                {
                    std::cout << "Error; row: " << (*next)->value << ", system.out.print call has non-integer parameter\n";
                    returnBool = true;
                }
                else if (expressionCheckRec((*next)))
                {
                    returnBool = true;
                }
            }
            else
            {
                Node *methodCallNode = (*child);
                auto mChild = methodCallNode->children.begin();
                auto mChildBase = mChild;
                std::string className;
                method *methodRecord = nullptr;
                if ((*mChild)->value == "this")
                {
                    className = (*mChild)->value;
                    // std::cout << "(SOP) className this: " << className << std::endl;
                }
                else
                {
                    // std::cout << "(debug) start while" << std::endl;
                    while ((*mChild)->children.empty() == false)
                    {
                        mChild = (*mChild)->children.begin();
                        if ((*mChild)->children.empty())
                        {
                            // std::cout << "(debug) going to break" << std::endl;
                        }
                    }
                    // std::cout << "Before className assign" << std::endl;
                    className = (*mChild)->value;
                    // std::cout << "(SOP) className else: " << className << std::endl;
                }
                mChild = mChildBase;
                mChild++;
                methodRecord = methodLookup(className, (*mChild)->value);
                if (methodRecord == nullptr)
                {
                    std::cout << "Error; row: " << (*next)->value << ", " << (*mChild)->value << " does not exist!\n";
                    returnBool = true;
                }
                else
                {
                    mChild++;
                    std::vector<std::string> params1;
                    std::vector<std::string> params2;
                    for (auto i = methodRecord->parameters.begin();
                         i != methodRecord->parameters.end(); i++)
                    {
                        params1.push_back(i->second);
                    }
                    params2 = getParams((*mChild), params2);
                    if (params1.size() != params2.size())
                    {
                        std::cout << "Error; row: " << (*next)->value << ", Invalid number of parameters in methodcall!\n";
                        returnBool = true;
                    }
                    for (int i = 0; i < params1.size(); i++)
                    {
                        if (params2[i] != params1[i])
                        {
                            std::cout << "Error; row: " << (*next)->value << ", Invalid parameter in methodcall!\n";
                            returnBool = true;
                        }
                    }
                    if (testMethodCallParams((*child)))
                    {
                        returnBool = true;
                    }
                    // check it returns INT for system.out
                    if ("int" != methodRecord->type)
                    {
                        std::cout << "Error; row: " << (*next)->value << ", return type is different from variable!\n";
                        returnBool = true;
                    }
                }
            }
        }
        else if ((*next)->type == "dotlength")
        {
            Node *walker = nodePtr;
            while (walker->children.empty() == false)
            {
                walker = (*walker->children.begin());
            }
            record *arrayVar = lookup(walker->value);
            if (arrayVar == nullptr)
            {
                std::cout << "Error; row: " << (*next)->value << " not defined!\n";
                returnBool = true;
            }
            if (arrayVar->type == "intArray")
            {
                std::cout << "Error; row: " << (*next)->value << ", calling dot-length on non array variable\n";
                returnBool = true;
            }
            else if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
        }
        else if ((*next)->type == "MainClass")
        {
            enterScope();
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            exitScope();
        }
        else if ((*next)->type == "PublicMainMethod")
        {
            enterScope();
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            exitScope();
        }
        else if ((*next)->type == "ClassDeclaration")
        {
            enterScope();
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            exitScope();
        }
        else if ((*next)->type == "MethodDeclaration")
        {
            enterScope();
            auto child = (*next)->children.begin();
            auto endChild = (*next)->children.end();
            endChild--;
            while ((*endChild)->children.empty() == false)
            {
                endChild = (*endChild)->children.begin();
            }
            std::string returnType = (*endChild)->value;
            std::string targetType = (*child)->value;
            if ((*child)->children.empty() == false)
            {
                auto targetChild = (*child)->children.begin();
                targetType = (*targetChild)->value;
            }
            child++;
            std::string methodName = (*child)->value;
            record *base = lookup(methodName);
            method *targetMethod = (method *)base;
            record *targetReturn = lookup(returnType);
            std::string returnId = ""; // for error reporting
            if (targetReturn)
            {
                returnType = targetReturn->type;
                returnId = (*endChild)->value;
            }
            else
            {
                returnType = (*endChild)->type;
                returnType = getTypeLiteralExpression(returnType);
                if (returnType == "n/a")
                {
                    returnId = (*endChild)->value;
                    auto params = targetMethod->parameters;
                    returnType = "Undefined identifier";
                    for (auto i = params.begin(); i != params.end(); ++i)
                    {
                        if (returnId == i->first)
                        {
                            returnType = i->second;
                        }
                    }
                }
            }
            if (targetType != returnType)
            {
                std::cout << "Error: Return type in method: "
                          << methodName << " '"
                          << targetType << "' "
                          << "doesn't match type being returned: "
                          << returnId << " '"
                          << returnType << "'"
                          << std::endl;
                returnBool = true;
            }
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
            exitScope();
        }
        else
        {
            if (expressionCheckRec((*next)))
            {
                returnBool = true;
            }
        }
    }
    return returnBool;
}

std::string symbolTable::getTypeLiteralExpression(std::string literal)
{
    if (literal == "IntegerLiteral")
    {
        return "int";
    }
    else if (literal == "BooleanExpression")
    {
        return "boolean";
    }
    return "n/a";
}

bool symbolTable::testBoolExpression(Node *ptr)
{
    bool returnBool = false;
    if (ptr->value == "AndOP" || ptr->value == "OrOP" || ptr->value == "NotOP")
    {
        if (ptr->value == "NotOP" && ptr->parent_node->type == "MethodCall")
        {
            auto meth = ptr->parent_node->children.begin();
            meth++;
            std::string methodName = (*meth)->value;
            record *methodRecord = lookup(methodName);
            // std::cout << "methodName: " << methodRecord->id << std::endl;
            // std::cout << "methodReturnType: " << methodRecord->type << std::endl;
            if (methodRecord->type != "boolean")
            {
                std::cout << "Error; "
                          << ptr->value
                          << " contains"
                          << "expression of type "
                          << methodRecord->type
                          << " expected type of boolean"
                          << std::endl;
                returnBool = true;
            }
        }
        else if (!isBoolChildren(ptr))
        {
            std::cout << "Error; "
                      << ptr->value
                      << " does not contain expression of type boolean!"
                      << std::endl;
            returnBool = true;
        }
    }
    else if (ptr->value == "GreaterOP" || ptr->value == "LesserOP")
    {
        if (testType(ptr, "int"))
        {
            returnBool = true;
        }
    }
    else if (ptr->value == "EqualsOP")
    {
        if (equalsTestType(ptr))
        {
            returnBool = true;
        }
    }
    for (auto i = ptr->children.begin(); i != ptr->children.end(); i++)
    {
        if (testBoolExpression((*i)))
        {
            returnBool = true;
        }
    }
    return returnBool;
}

bool symbolTable::expressionCheckRecNode(Node *nodePtr)
{
    if (nodePtr->type == "Identifier")
    {
        record *identifierVal = lookup(nodePtr->value);
        if (identifierVal != nullptr)
        {
            if (identifierVal->type == "Class")
            {
                expressionElements.push_back(nodePtr->value);
            }
            else if (identifierVal->type != "intArray")
            {
                expressionElements.push_back(identifierVal->type);
            }
            else
            {
                expressionElements.push_back("int");
            }
        }
        else
        {
            std::cout << "Error: variable " << nodePtr->value << " is not declared\n";
            return true;
        }
    }
    else if (nodePtr->type == "dotlength")
    {
        // check children for type of identifier
        Node *walker = nodePtr;
        while (walker->children.empty() == false)
        {
            walker = (*walker->children.begin());
        }
        record *arrayVar = lookup(walker->value);
        if (arrayVar != nullptr)
        {
            // std::cout << arrayVar->id << " " << arrayVar->type << "\n";
            if (arrayVar->type == "intArray")
            {
                expressionElements.push_back("int");
                return false;
            }
            else
            {
                std::cout << "Error; Calling length operator on non array"
                          << ", got: "
                          << arrayVar->type
                          << " variable: "
                          << arrayVar->id
                          << std::endl;
                return true;
            }
        }
        else
        {
            std::cout << "Error; "
                      << walker->value
                      << " doesnt exists"
                      << std::endl;
            return true;
        }
    }
    else if (nodePtr->type == "IntegerLiteral")
    {
        expressionElements.push_back("int");
    }
    else if (nodePtr->type == "BooleanExpression")
    {
        expressionElements.push_back("boolean");
    }
    else if (nodePtr->type == "MethodCall")
    {
        auto mChild = nodePtr->children.begin();
        auto mChildBase = mChild;
        std::string className;
        method *methodRecord = nullptr;
        if ((*mChild)->value == "this")
        {
            className = (*mChild)->value;
            // std::cout << "(secondary) className this: " << className << std::endl;
        }
        else
        {
            // std::cout << "(debug) start while" << std::endl;
            while ((*mChild)->children.empty() == false)
            {
                mChild = (*mChild)->children.begin();
                if ((*mChild)->children.empty())
                {
                    // std::cout << "(debug) reached while loop end" << std::endl;
                }
            }
            // std::cout << "Before className assign" << std::endl;
            className = (*mChild)->value;
            // std::cout << "(secondary) className else: " << className << std::endl;
        }
        mChild = mChildBase;
        mChild++;
        /* debug */ if (className == "")
        {
            // std::cout << "!!HERE!!" << std::endl;
            // std::cout << "value: " << (*mChild)->value << std::endl;
            // std::cout << "id: " << (*mChild)->id << std::endl;
        }
        methodRecord = methodLookup(className, (*mChild)->value);
        if (methodRecord == nullptr)
        {
            std::cout << "Error; " << (*mChild)->value << " does not exist!\n";
            return true;
        }
        expressionElements.push_back(methodRecord->type);
        return false;
    }
    else if (nodePtr->type == "intArray")
    {
        expressionElements.push_back("int");
    }
    for (auto i = nodePtr->children.begin(); i != nodePtr->children.end(); i++)
    {
        if (expressionCheckRecNode((*i)))
        {
            return true;
        }
    }
    return false;
}

bool symbolTable::typeCheck()
{
    return expressionCheck();
}

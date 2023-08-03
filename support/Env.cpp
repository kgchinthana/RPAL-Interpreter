#include "Env.h"

// Constructor for the class Env, taking an integer id as a parameter.
Env::Env(int id)
{
    this->id = id; // Assign the provided id to the member variable 'id' of the current instance.
}

Env::~Env()
{
}

// Method to assign a parent environment to the current environment.
// 'parent' is a pointer to an object of type Env, representing the parent environment.
void Env::assignParent(Env *parent)
{
    this->parent = parent;
}

// Method to perform a symbol lookup in the environment's symbol table.
// 'str' is a string representing the name of the symbol to be looked up.
// The method returns a pointer to an object of type Ctrl (probably a controller) if the symbol is found, or nullptr if not found.
Ctrl *Env::lookup(string str)
{
    map<string, Ctrl *>::iterator it;
    it = symbolTable.find(str);
    if (symbolTable.end() == it)
        return parent->lookup(str);
    else
        return (*it).second;
}

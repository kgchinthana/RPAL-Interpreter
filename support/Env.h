#ifndef ENV_H_
#define ENV_H_

#include <map>
#include <string>
#include "Ctrl.h"

using namespace std;

class Env {
public:
    int id;
    Env *parent;
    void assignParent(Env* );
    Ctrl* lookup(string);
    Env(int);
    virtual ~Env();
    map<string, Ctrl *> symbolTable;
};

#endif /* ENV_H_ */

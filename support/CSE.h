#ifndef CSEM_H_
#define CSE_H_

#include <vector>
#include <stack>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "Env.h"

using namespace std;

static vector <string> inbuiltFuncVector =
        {"Order","Print","Isinteger","Istruthvalue","Isstring",
         "Istuple","Isfunction","Isdummy","Stem","Stern",
         "Conc","Conc2","ItoS","Null"};

class CSE {

    void flattenDeltaThen(Node*, Ctrl *, vector<Ctrl *> *);
    void flattenDeltaElse(Node*, Ctrl *, vector<Ctrl *> *);
    void flattenLAMBDA(Node*, Ctrl *, vector<Ctrl *> *);
    void flattenTernary(Node*, Ctrl *, vector<Ctrl *> *);
    void flattenTree(Node*, Ctrl *, vector<Ctrl *> *);
    void init(Node*);
    void applyBinaryOPR(int);
    void applyThisRator(Ctrl*);
    void printCS();
    bool checkInbuilt(string);
    void escapePrintStr(string);
    void rule411(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void rule12(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void rule13(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void rule10(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleNeg(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleEnv(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleTau(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleBeta(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleName(Ctrl*, Ctrl*, Ctrl*, Env*, int);
    void handleGAMMA(Ctrl*, Ctrl*, Ctrl*, Env*, int);

    Node* top;
    vector<Ctrl *> *deltas;
    Ctrl *rootDelta;
    int numEnvironment;
    vector<Ctrl *> control;
    stack<Ctrl *> execStack;
    Env *currEnvironment;
    Env *tempEnvironment;
    Env *PE;
    stack<Env*> environmentStack;
    map<int, Env *> environmentMap;

public:
    CSE();
    CSE(Node*);
    virtual ~CSE();
    Env* primitiveEnv;
    void run(Node*);
    Env* createNewEnvironment();
    void execCSE();

};


#endif /* CSE_H_ */

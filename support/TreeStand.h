#ifndef TREESTAND_H_
#define TREESTAND_H_

#include "LexicalAnalyser.h"
#include "Node.h"

using namespace std;

class TreeStand {
    void standardize(Node*);
    void standardizeLET(Node*);
    void standardizeWHERE(Node*);
    void standardizeWITHIN(Node*);
    void standardizeREC(Node*);
    void standardizeFCNFORM(Node*);
    void standardizeLAMBDA(Node*);
    void standardizeAND(Node*);
    void standardizeAT(Node*);

public:
    TreeStand(Node* topNode);
    virtual ~TreeStand();
};

#endif /* TREESTAND_H_ */

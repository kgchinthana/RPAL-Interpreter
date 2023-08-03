#include "TreeStand.h"

// Constructor: Initializes the TreeStand with a given topNode and standardizes it.
TreeStand::TreeStand(Node *topNode)
{
    standardize(topNode);
}

// Destructor: Doesn't perform any specific cleanup in this case.
TreeStand::~TreeStand()
{
}

// Function to standardize the LET case of the tree.
void TreeStand::standardizeLET(Node *topNode)
{
    // Function local variables
    Node *p;
    // Check if the subtree is not already in standard form (BINDING node expected).
    if (Node::BINDING != topNode->childNode->type)
    {
        printf("Subtree not standardised: case LET");
    }

    // Perform the standardization: Change the topNode and its child node types and strings.
    topNode->type = Node::GAMMA;
    topNode->nodeString = "gamma";
    topNode->childNode->type = Node::LAMBDA;
    topNode->childNode->nodeString = "lambda";
    p = topNode->childNode->siblingNode;
    topNode->childNode->siblingNode = topNode->childNode->childNode->siblingNode;
    topNode->childNode->childNode->siblingNode = p;
}
// Function to standardize the WHERE case of the tree.
void TreeStand::standardizeWHERE(Node *topNode)
{

    Node *p, *x, *e;

    // Check if the subtree is not already in standard form (BINDING node expected).
    if (Node::BINDING != topNode->childNode->siblingNode->type)
    {

        printf("Subtree not standardised: case WHERE");
    }

    // Perform the standardization: Change the topNode and its child node types and strings.
    topNode->type = Node::GAMMA;
    topNode->nodeString = "gamma";
    p = topNode->childNode;
    x = topNode->childNode->siblingNode->childNode;
    e = topNode->childNode->siblingNode->childNode->siblingNode;
    p->siblingNode = NULL;
    x->siblingNode = NULL;
    topNode->childNode = new Node();
    topNode->childNode->type = Node::LAMBDA;
    topNode->childNode->nodeString = "lambda";
    topNode->childNode->siblingNode = e;
    topNode->childNode->childNode = x;
    x->siblingNode = p;
}
// Function to standardize the WITHIN case of the tree.
void TreeStand::standardizeWITHIN(Node *topNode)
{

    Node *x1, *x2, *e1;
    // Check if the subtree is not already in standard form (BINDING nodes expected).
    if (Node::BINDING != topNode->childNode->type || Node::BINDING != topNode->childNode->siblingNode->type)
    {

        printf("Subtree not standardised: case WITHIN");
    }
    // Perform the standardization: Rearrange the nodes and types.
    topNode->type = Node::BINDING;
    x1 = topNode->childNode->childNode;
    x2 = topNode->childNode->siblingNode->childNode;
    e1 = x1->siblingNode;
    x1->siblingNode = x2->siblingNode;
    x2->siblingNode = new Node();
    x2->siblingNode->type = Node::GAMMA;
    x2->siblingNode->nodeString = "gamma";
    x2->siblingNode->childNode = new Node();
    x2->siblingNode->childNode->type = Node::LAMBDA;
    x2->siblingNode->childNode->nodeString = "lambda";
    x2->siblingNode->childNode->siblingNode = e1; // the new lambda's right
    x2->siblingNode->childNode->childNode = x1;   // the new lambda's left has now x1 who's first sibling is e2
    topNode->childNode = x2;
}
// Function to standardize the REC case of the tree.
void TreeStand::standardizeREC(Node *topNode)
{

    Node *x, *e, *temp;
    // Check if the subtree is not already in standard form (BINDING node expected).
    if (Node::BINDING != topNode->childNode->type)
    {

        printf("Subtree not standardised: case REC");
    }

    topNode->type = Node::BINDING;
    x = topNode->childNode->childNode;
    e = topNode->childNode->childNode->siblingNode;
    x->siblingNode = NULL;
    topNode->childNode = x; // removing the reference to the old = node
    temp = new Node();
    temp->type = Node::GAMMA;
    temp->nodeString = "gamma";
    x->siblingNode = temp;
    temp->childNode = new Node();
    temp->childNode->type = Node::YSTAR;
    temp->childNode->nodeString = "<Y*>";
    temp->childNode->siblingNode = new Node();
    temp->childNode->siblingNode->type = Node::LAMBDA;
    temp->childNode->siblingNode->nodeString = "lambda";
    temp->childNode->siblingNode->childNode = new Node();
    temp->childNode->siblingNode->childNode->type = x->type;
    temp->childNode->siblingNode->childNode->nodeString = x->nodeString;
    temp->childNode->siblingNode->childNode->siblingNode = e;
}
// Function to standardize the FCN_FORM case of the tree.
void TreeStand::standardizeFCNFORM(Node *topNode)
{

    Node *temp, *newNode;
    topNode->type = Node::BINDING;
    topNode->nodeString = "=";
    temp = topNode->childNode;

    while (temp->siblingNode->siblingNode != NULL)
    {

        newNode = new Node();
        newNode->type = Node::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = newNode->childNode;
    }
}
// Function to standardize the LAMBDA case of the tree.
void TreeStand::standardizeLAMBDA(Node *topNode)
{

    Node *temp, *newNode;
    temp = topNode->childNode;

    while (temp->siblingNode->siblingNode != NULL)
    {

        newNode = new Node();
        newNode->type = Node::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = temp->siblingNode;
    }
}
// Function to standardize the AND case of the tree.
void TreeStand::standardizeAND(Node *topNode)
{

    Node *temp;
    Node *tauNode, *commaNode, **currentTau, **currentComma;
    topNode->type = Node::BINDING;
    topNode->nodeString = "=";
    temp = topNode->childNode;
    tauNode = new Node();
    commaNode = new Node();
    tauNode->type = Node::TAU;
    tauNode->nodeString = "tau";
    tauNode->childNode = NULL;
    tauNode->siblingNode = NULL;
    commaNode->type = Node::COMMA;
    commaNode->nodeString = ",";
    commaNode->childNode = NULL;
    commaNode->siblingNode = NULL;
    topNode->childNode = commaNode;
    topNode->childNode->siblingNode = tauNode;
    currentTau = &(tauNode->childNode);
    currentComma = &(commaNode->childNode);

    while (temp != NULL)
    {

        *currentTau = temp->childNode->siblingNode;
        temp->childNode->siblingNode = NULL;
        *currentComma = temp->childNode;
        temp = temp->siblingNode;
        currentComma = &((*currentComma)->siblingNode); // Can your code BE any more UGLY?
        currentTau = &((*currentTau)->siblingNode);
    }
}
// Function to standardize the AT case of the tree.
void TreeStand::standardizeAT(Node *topNode)
{

    Node *E1, *N, *E2;
    E1 = topNode->childNode;
    N = E1->siblingNode;
    E2 = N->siblingNode;
    topNode->type = Node::GAMMA;
    topNode->nodeString = "gamma";
    topNode->childNode = new Node();
    topNode->childNode->type = Node::GAMMA;
    topNode->childNode->nodeString = "gamma";
    topNode->childNode->siblingNode = E2;
    topNode->childNode->childNode = N;
    N->siblingNode = NULL;
    topNode->childNode->childNode->siblingNode = E1;
    E1->siblingNode = NULL;
}

void TreeStand::standardize(Node *topNode)
{

    Node *p, *e, *e1, *e2, *x, *x1, *x2, *n, *temp, *new_temp;
    string origString = topNode->nodeString;
    if (topNode->childNode != NULL)
        standardize(topNode->childNode);
    if (topNode->siblingNode != NULL)
        standardize(topNode->siblingNode);

    switch (topNode->type)
    {
    case Node::LET:
        standardizeLET(topNode);
        break;
    case Node::WHERE:
        standardizeWHERE(topNode);
        break;
    case Node::WITHIN:
    {
        standardizeWITHIN(topNode);
        break;
    }
    case Node::REC:
    {
        standardizeREC(topNode);
        break;
    }
    case Node::FCN_FORM:
    {
        standardizeFCNFORM(topNode);
        break;
    }
    case Node::LAMBDA:
        standardizeLAMBDA(topNode);
        break;
    case Node::AND:
        standardizeAND(topNode);
        break;
    case Node::AT:
        standardizeAT(topNode);
        break;
    default:
        break;
    }
}

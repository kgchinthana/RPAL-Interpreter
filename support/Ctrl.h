#include "LexicalAnalyser.h"
#include "Node.h"

#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>

class Ctrl{

public:
  void addCtrl(Node* node, int , string, vector<string> *, Ctrl*, int);
  enum Type{
    ENV = 1 ,
    DELTA = 2 ,
    NAME = 3 ,
    LAMBDA = 4 ,
    GAMMA = 5 ,
    AUG = 6 ,
    BETA = 7 ,
    OR = 8 ,
    AND_LOGICAL = 9 ,
    NOT = 10 ,
    GR = 11 ,
    GE = 12 ,
    LS = 13 ,
    LE = 14 ,
    EQ = 15 ,
    NE = 16 ,
    ADD = 17 ,
    SUBTRACT = 18 ,
    NEG = 19 ,
    MULTIPLY = 20 ,
    DIVIDE = 21 ,
    EXP = 22 ,
    AT = 23 ,
    TRUE = 24 ,
    FALSE = 25 ,
    NIL = 26 ,
    DUMMY = 27 ,
    YSTAR = 28 ,
    ETA = 29 ,
    TAU = 30 ,
    STRING = 31 ,
    INTEGER = 32 ,
    TUPLE = 33
    };
  string toStr();
  Ctrl(Ctrl *ctrl);
  Ctrl(Ctrl::Type type, int index);
  Ctrl(Ctrl::Type type, vector<string> *variables, Ctrl *del_ptr, int delta_index);
  Ctrl(Ctrl::Type type, int index, bool b);
  Ctrl(string var_value, Ctrl::Type type );
  Ctrl(Ctrl::Type type, string value);
  Ctrl();
  Ctrl(Ctrl::Type type);
  vector<Ctrl *> *ctrlStruct;
  vector<string> variables;
  vector<Ctrl *> ctrlTuples;
  int associatedENV;
  Type type;
  int index;
  string ctrlVal;
  Ctrl *delta;

};

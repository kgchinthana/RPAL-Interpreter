#ifndef PARSER_H_
#define PARSER_H_

#include "LexicalAnalyser.h"
#include <stack>
#include "TreeStand.h"
#include "CSE.h"

using namespace std;

class Parser {

public:
	Parser (LexicalAnalyser*);
    void evaluateProgram();
	virtual ~Parser();

private:
    LexicalAnalyser* lex;
    stack <Node*> treeStack;
    Token *nextToken;

	void E();
	void Ew();
	void T();
	void Ta();
	void Tc();
	void B();
	void Bt();
	void Bs();
	void Bp();
	void A();
	void At();
	void Af();
	void Ap();
	void R();
	void Rn();
	void D();
	void Da();
	void Dr();
	void Db();
	void Vb();
	void Vl();

	void parse();
	void standardize(Node*);
    void buildTree(string, int);
    void buildTree(string, int, int);
    void treePrettyPrint(Node*, int);
    void read(string);
	bool isKeyword(string);
	string to_s(Node*);

};

#endif /* PARSER_H_ */

#ifndef LEXICAL_ANALYSER_H_
#define LEXICAL_ANALYSER_H_

#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "Token.h"

using namespace std;

class LexicalAnalyser {

public:
	LexicalAnalyser(ifstream*);
	virtual ~LexicalAnalyser();
	Token* getNxtToken();
	void lexerReset();

private:
    ifstream* file;
    int lineCount, charCount;
    string tokenIdentifier();
    string tokenInteger();
    string tokenStrings();
    string tokenSpaces();
    string tokenComment();
    string tokenOperator();
    string tokenPunctuation();

	bool isPunctuation(char);
	bool isOperatorSymbol(char);
	bool isSpaces(char);
	bool isEOL(char);
	bool isEOF(char);
	bool isCommentChar(char);
	bool isKeyword(string);

};

#endif /* LEXICAL_ANALYSER_H_ */

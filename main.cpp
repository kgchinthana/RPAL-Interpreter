#include <cstdio>
#include <fstream>
#include "support/LexicalAnalyser.h"
#include "support/Parser.h"

using namespace std;

ifstream fileRead;

int main (int argc, char *argv[]){

    // only two args possible
    //      main
    //      filename
	if (argc >= 2) {

	    char* filename = argv[argc-1];

	    fileRead.open(filename);

        // error checking
	    if (!fileRead.good()){
	        printf ("File \"%s\" not found\n", filename);
	        return 0;
	    }

        // evaluate program
        auto rpalLexer = new LexicalAnalyser(&fileRead);
        auto rpalParser = new Parser(rpalLexer);
        rpalParser->evaluateProgram();

		fileRead.close();

	}

    // if argument count is abnormal
    else {
        cout << "usage : ./rpal20 <filename>" << endl;
    }

}

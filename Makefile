all:	
	g++ -std=c++11 support/Token.cpp support/TreeStand.cpp support/LexicalAnalyser.cpp support/Ctrl.cpp support/Parser.cpp support/Env.cpp support/CSE.cpp main.cpp -o rpal20
cl: 
	rm -f main;

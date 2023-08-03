#include "Parser.h"

// Constructor: Initializes the Parser with a given LexicalAnalyser.
Parser::Parser(LexicalAnalyser *lexr)
{
	this->lex = lexr;
	nextToken = NULL;
}

// Destructor: Cleans up the Parser.
Parser::~Parser()
{
}

// Entry point for parsing the program.
void Parser::evaluateProgram()
{

	parse(); // Parse the input code.

	// After parsing, perform some post-parsing operations on the generated tree.
	auto ts = new TreeStand(treeStack.top());
	auto cse = new CSE();
	cse->run(treeStack.top());
}

void Parser::parse()
{

	// Skip spaces and comments by reading the next token until it's not TOK_DELETE.
	do
	{

		nextToken = lex->getNxtToken();

	} while (nextToken->tokType == TOK_DELETE);
	// Begin the actual parsing by calling the 'E' rule.
	E();
	// Check if the parsing is complete.
	if (!treeStack.empty() && treeStack.size() != 1)
	{

		printf("Error: Stack not empty at the end of parsing\n");
		exit(0);
	}
	else if (lex->getNxtToken()->tokType != TOK_EOF)
	{

		printf("Error: Parsing finished input still remaining\n");
		exit(0);
	}
}

// Helper function to check if a given string is a keyword.
bool Parser::isKeyword(string val)
{
	// The function checks if the given string matches any predefined keywords.
	if (val == "in" || val == "where" || val == "." || val == "aug" || val == "and" || val == "or" || val == "&" || val == "not" || val == "gr" || val == "ge" || val == "ls" || val == "le" || val == "eq" || val == "ne" || val == "+" || val == "-" || val == "*" || val == "/" || val == "**" || val == "@" || val == "within" || val == "rec" || val == "let" || val == "fn")
		return true;
	else
		return false;
}
// Helper function to read the next token and check if it matches the expected token.
void Parser::read(string tokStr)
{

	if (!(nextToken->tokValue == tokStr))
	{

		printf("At L:C::%d:%d Expected '%s', received '%s'\n", nextToken->lineCount, nextToken->chrCount,
			   tokStr.c_str(), nextToken->tokValue.c_str());
		exit(0);
	}

	// Skip spaces and comments by reading the next token until it's not TOK_DELETE.
	do
	{

		nextToken = lex->getNxtToken();

	} while (nextToken->tokType == TOK_DELETE);
}

void Parser::buildTree(string nodeStr, int numChildNodes, int type)
{
	// Create a new node with the provided information.
	int finalSize = treeStack.size() - numChildNodes + 1;
	Node *newNode = new Node();
	Node *tempNode = new Node();
	newNode->nodeString = nodeStr;
	newNode->type = type;

	// Handle different cases for the number of child nodes.
	if (numChildNodes == 0)
	{
		// No child nodes, simply push the new node onto the stack.
		treeStack.push(newNode);
		return;
	}

	if (treeStack.empty())
	{
		// Stack is empty, but there should be child nodes to pop.
		return;
	}
	else
	{
		// Pop the required number of child nodes from the stack and link them with the new node.
		while ((numChildNodes - 1) > 0)
		{

			if (!treeStack.empty())
			{

				tempNode = treeStack.top();
				treeStack.pop();

				if (treeStack.size() != 0)
				{
					treeStack.top()->siblingNode = tempNode;
				}
				else if (treeStack.size() == 0)
				{ // Stack cannot be empty here. We have one more element to pop before we can build/push the requested tree
					printf("Parse Error: Empty Stack\n");
					exit(0);
				}

				numChildNodes--;
			}
			else
			{
				// Stack size is less than numChildNodes. Abort.
				return;
			}
		}

		// Now, pop the last child node from the stack and link it as a child of the new node.
		tempNode = treeStack.top();
		newNode->childNode = tempNode;
		treeStack.pop();
	}
	// Finally, push the new node onto the stack.
	treeStack.push(newNode);
}

// Function to convert a node type to a string representation.
string Parser::to_s(Node *node)
{
	// This function converts different node types to their corresponding string representations.

	string str;
	switch (node->type)
	{

	case Node::IDENTIFIER:
		return "<ID:" + node->nodeString + ">";

	case Node::INTEGER:
		return "<INT:" + node->nodeString + ">";

	case Node::STRING:
		return "<STR:" + node->nodeString + ">";

	default:
		return node->nodeString;
	}
}

void Parser::treePrettyPrint(Node *topNode, int numDots)
{
	// This function prints the AST in a pretty format by adding indentation with dots based on the node's level.
	int numDots1 = numDots;

	while (numDots1 > 0)
	{

		printf(".");
		numDots1--;
	}

	printf("%s\n", to_s(topNode).c_str());

	if (topNode->childNode != NULL)
	{
		treePrettyPrint(topNode->childNode, numDots + 1);
	}

	if (topNode->siblingNode != NULL)
	{
		treePrettyPrint(topNode->siblingNode, numDots);
	}
}

/* E   -> ’let’ D ’in’ E     => ’let’
	   -> ’fn’ Vb+ ’.’ E     => ’lambda’
	   ->  Ew;
*/

// Other functions implementing grammar rules such as 'E', 'Ew', 'T', 'Ta', 'Tc', etc.
// The implementation of the grammar rules 'E', 'Ew', 'T', 'Ta', 'Tc', etc. is done recursively.
void Parser::E()
{

	if (nextToken->tokValue == "let")
	{

		read("let");
		D();
		read("in");
		E();
		buildTree("let", 2, Node::LET);
	}
	else if (nextToken->tokValue == "fn")
	{

		read("fn");
		int n = 0;
		do
		{
			Vb();
			n++;
		} while (nextToken->tokValue == "(" || nextToken->tokType == TOK_IDENTIFIER);
		read(".");
		E();
		buildTree("lambda", n + 1, Node::LAMBDA);
	}
	else
	{
		Ew();
	}
}

/* Ew  -> T ’where’ Dr   => ’where’
	   -> T;
*/
void Parser::Ew()
{

	T();

	if (nextToken->tokValue == "where")
	{

		read("where");
		Dr();
		buildTree("where", 2, Node::WHERE);
	}
}

/* T   -> Ta ( ’,’ Ta )+   => ’tau’
	   -> Ta ;
*/
void Parser::T()
{

	int n = 0;

	Ta();

	if (nextToken->tokValue == ",")
	{

		do
		{

			read(",");
			Ta();
			n++;

		} while (nextToken->tokValue == ",");

		buildTree("tau", n + 1, Node::TAU);
	}
}

/* Ta  -> Ta ’aug’ Tc    => ’aug’
	   -> Tc ;
 */
void Parser::Ta()
{

	Tc();

	while (nextToken->tokValue == "aug")
	{

		read("aug");
		Tc();
		buildTree("aug", 2, Node::AUG);
	}
}

/* Tc  -> B ’->’ Tc ’|’ Tc     => '->'
	   -> B ;
*/
void Parser::Tc(){

	B();
	if (nextToken->tokValue == "->")
	{
		read("->");
		Tc();
		read("|");
		Tc();
		buildTree("->", 3, Node::TERNARY);
	}
}

/* B   -> B ’or’ Bt   => ’or’
	   -> Bt ;
*/
void Parser::B()
{

	Bt();

	while (nextToken->tokValue == "or")
	{

		read("or");
		Bt();
		buildTree("or", 2, Node::OR);
	}
}

/*  Bt  -> Bt ’&’ Bs   => ’&’
		-> Bs ;
*/
void Parser::Bt()
{

	Bs();

	while (nextToken->tokValue == "&")
	{

		read("&");
		Bs();
		buildTree("&", 2, Node::AND_LOGICAL);
	}
}

/* Bs  -> ’not’ Bp  => ’not’
	   -> Bp ;
*/
void Parser::Bs()
{

	if (nextToken->tokValue == "not")
	{

		read("not");
		Bp();
		buildTree("not", 1, Node::NOT);
	}
	else
	{
		Bp();
	}
}

/* Bp  -> A (’gr’ | ’>’ ) A   => ’gr’
	   -> A (’ge’ | ’>=’) A   => ’ge’
	   -> A (’ls’ | ’<’ ) A   => ’ls’
	   -> A (’le’ | ’<=’) A   => ’le’
	   -> A ’eq’ A            => ’eq’
	   -> A ’ne’ A            => ’ne’
	   -> A ;
*/
void Parser::Bp()
{

	A();

	if (nextToken->tokValue == "gr" || nextToken->tokValue == ">")
	{

		if (nextToken->tokValue == "gr")
			read("gr");
		else if (nextToken->tokValue == ">")
			read(">");

		A();

		buildTree("gr", 2, Node::GR);
	}
	else if (nextToken->tokValue == "ge" || nextToken->tokValue == ">=")
	{

		if (nextToken->tokValue == "ge")
			read("ge");
		else if (nextToken->tokValue == ">=")
			read(">=");

		A();

		buildTree("ge", 2, Node::GE);
	}
	else if (nextToken->tokValue == "ls" || nextToken->tokValue == "<")
	{

		if (nextToken->tokValue == "ls")
			read("ls");
		else if (nextToken->tokValue == "<")
			read("<");

		A();

		buildTree("ls", 2, Node::LS);
	}
	else if (nextToken->tokValue == "le" || nextToken->tokValue == "<=")
	{

		if (nextToken->tokValue == "le")
			read("le");
		else if (nextToken->tokValue == "<=")
			read("<=");

		A();

		buildTree("le", 2, Node::LE);
	}
	else if (nextToken->tokValue == "eq")
	{

		read("eq");
		A();
		buildTree("eq", 2, Node::EQ);
	}
	else if (nextToken->tokValue == "ne")
	{

		read("ne");
		A();
		buildTree("ne", 2, Node::NE);
	}
}

/* A   -> A ’+’ At      => ’+’
	   -> A ’-’ At      => ’-’
	   ->   ’+’ At
	   ->   ’-’ At      => ’neg’
	   -> At ;
*/
void Parser::A()
{

	string treeStr;

	if (nextToken->tokValue == "+")
	{

		read("+");
		At();
	}
	else if (nextToken->tokValue == "-")
	{

		read("-");
		At();
		buildTree("neg", 1, Node::NEG);
	}
	else
	{
		At();
	}

	while (nextToken->tokValue == "+" || nextToken->tokValue == "-")
	{

		if (nextToken->tokValue == "+")
		{
			read("+");
			treeStr = "+";
		}
		else
		{
			read("-");
			treeStr = "-";
		}

		At();

		buildTree(treeStr, 2, treeStr == "+" ? Node::ADD : Node::SUBTRACT);
	}
}

/* At  -> At ’*’ Af      => ’*’
	   -> At ’/’ Af      => ’/’
	   -> Af ;
*/
void Parser::At()
{

	string treeStr;

	Af();

	while (nextToken->tokValue == "*" || nextToken->tokValue == "/")
	{

		if (nextToken->tokValue == "*")
		{
			read("*");
			treeStr = "*";
		}
		else
		{
			read("/");
			treeStr = "/";
		}

		Af();

		buildTree(treeStr, 2, treeStr == "*" ? Node::MULTIPLY : Node::DIVIDE);
	}
}

/* Af  -> Ap ’**’ Af    => ’**’
	   -> Ap ;
*/
void Parser::Af()
{

	Ap();

	if (nextToken->tokValue == "**")
	{
		read("**");
		Af();
		buildTree("**", 2, Node::EXPONENTIAL);
	}
}

/* Ap  -> Ap ’@’ ’<IDENTIFIER>’ R    => ’@’
	   -> R ;
*/
void Parser::Ap()
{

	R();

	while (nextToken->tokValue == "@")
	{

		read("@");
		buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
		read(nextToken->tokValue);

		R();

		buildTree("@", 3, Node::AT);
	}
}

/* R   -> R Rn   => ’gamma’
	   -> Rn ;
*/
void Parser::R()
{

	Rn();

	while ((TOK_IDENTIFIER == nextToken->tokType || TOK_INTEGER == nextToken->tokType || TOK_STRING == nextToken->tokType || "(" == nextToken->tokValue || "false" == nextToken->tokValue || "true" == nextToken->tokValue || "nil" == nextToken->tokValue || "dummy" == nextToken->tokValue) && !isKeyword(nextToken->tokValue))
	{

		Rn();
		buildTree("gamma", 2, Node::GAMMA);
	}
}

/* Rn  -> ’<IDENTIFIER>’
	   -> ’<INTEGER>’
	   -> ’<STRING>’
	   -> ’true’             => ’true’
	   -> ’false’            => ’false’
	   -> ’nil’              => ’nil’
	   -> ’(’ E ’)’
	   -> ’dummy’            => ’dummy’ ;
*/
void Parser::Rn()
{

	if ("(" == nextToken->tokValue)
	{

		read("(");
		E();
		read(")");
	}

	else if (TOK_IDENTIFIER == nextToken->tokType || TOK_INTEGER == nextToken->tokType || TOK_STRING == nextToken->tokType)
	{

		if ("true" == nextToken->tokValue)
		{

			read("true");
			buildTree("<true>", 0, Node::TRUE);
		}
		else if ("false" == nextToken->tokValue)
		{

			read("false");
			buildTree("<false>", 0, Node::FALSE);
		}
		else if ("nil" == nextToken->tokValue)
		{

			read("nil");
			buildTree("<nil>", 0, Node::NIL);
		}
		else if ("dummy" == nextToken->tokValue)
		{

			read("dummy");
			buildTree("<dummy>", 0, Node::DUMMY);
		}
		else if (TOK_IDENTIFIER == nextToken->tokType)
		{

			buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
			read(nextToken->tokValue);
		}
		else if (TOK_STRING == nextToken->tokType)
		{

			buildTree(nextToken->tokValue, 0, Node::STRING);
			read(nextToken->tokValue);
		}
		else if (TOK_INTEGER == nextToken->tokType)
		{

			buildTree(nextToken->tokValue, 0, Node::INTEGER);
			read(nextToken->tokValue);
		}
	}
}

/* D  -> Da ’within’ D   => ’within’
	  -> Da ;
  */
void Parser::D()
{

	Da();

	if (nextToken->tokValue == "within")
	{

		read("within");
		D();
		buildTree("within", 2, Node::WITHIN);
	}
}
/*     Da  -> Dr ( ’and’ Dr )+   => ’and’
		   -> Dr ;
*/
void Parser::Da()
{

	int n = 0;

	Dr();

	while (nextToken->tokValue == "and")
	{

		read("and");
		Dr();
		n++;
	}

	if (n > 0)
		buildTree("and", n + 1, Node::AND);
}

/*      Dr  -> ’rec’ Db  => ’rec’
			-> Db ;
*/
void Parser::Dr()
{

	if (nextToken->tokValue == "rec")
	{

		read("rec");
		Db();
		buildTree("rec", 1, Node::REC);
	}
	else
	{
		Db();
	}
}

/* Db  -> Vl ’=’ E                    => ’=’
	   -> ’<IDENTIFIER>’ Vb+ ’=’ E    => ’fcn_form’
	   -> ’(’ D ’)’ ;
*/
void Parser::Db()
{

	if (nextToken->tokValue == "(")
	{

		read("(");
		D();
		read(")");
	}
	else if (nextToken->tokType == TOK_IDENTIFIER)
	{

		// Since identifier type is common here, read it here now and consider it for build tree later.
		buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
		read(nextToken->tokValue);

		if (nextToken->tokValue == "," || nextToken->tokValue == "=")
		{

			Vl();
			read("=");
			E();
			buildTree("=", 2, Node::BINDING);
		}
		else
		{

			int n = 0;
			do
			{
				Vb();
				n++;
			} while (nextToken->tokValue == "(" || nextToken->tokType == TOK_IDENTIFIER);
			read("=");
			E();

			buildTree("function_form", n + 2, Node::FCN_FORM);
		}
	}
}

/*    Vb -> ’<IDENTIFIER>’
		 -> ’(’ Vl ’)’
		 -> ’(’ ’)’         => ’()’;
*/
void Parser::Vb()
{

	if (nextToken->tokType == TOK_IDENTIFIER)
	{

		buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
		read(nextToken->tokValue);
	}
	else if (nextToken->tokValue == "(")
	{

		read("(");

		if (nextToken->tokValue == ")")
		{
			read(")");
			buildTree("()", 0, Node::PARANTHESES);
		}

		else if (nextToken->tokType == TOK_IDENTIFIER)
		{
			// Before getting into Vl, an identifier must be read
			// Vl expects its caller to do this.
			buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
			read(nextToken->tokValue);
			Vl();
			read(")");
		}
	}
}

//    Vl   -> ’<IDENTIFIER>’ list ’,’ => ’,’?;
void Parser::Vl()
{

	int n = 0;

	while (nextToken->tokValue == ",")
	{

		read(",");

		if (nextToken->tokType == TOK_IDENTIFIER)
		{
			buildTree(nextToken->tokValue, 0, Node::IDENTIFIER);
			read(nextToken->tokValue);
			n++;
		}
		else
		{
			printf("ERROR In Vl()\n");
		}
	}

	if (n > 0)
		buildTree(",", n + 1, Node::COMMA);
}

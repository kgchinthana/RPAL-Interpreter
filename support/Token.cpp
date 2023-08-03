#include "Token.h"

Token::Token()
{
	// Initialize the 'tokType' member variable to 0 (default value for an integer).
	tokType = 0;
	// Initialize the 'tokValue' member variable to an empty string (default value for a string).
	tokValue = "";
	// Initialize the 'lineCount' member variable to 0 (default value for an integer).
	lineCount = 0;
	// Initialize the 'chrCount' member variable to 0 (default value for an integer).
	chrCount = 0;
}
// Destructor for the Token class.
Token::~Token()
{
	// The destructor doesn't have any specific implementation here.
	// Since there are no dynamically allocated resources, there is no need for explicit cleanup.
	// The compiler-generated destructor will handle any necessary cleanup automatically.
}

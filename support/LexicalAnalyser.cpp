#include "LexicalAnalyser.h"

using namespace std;

// Constructor: Takes a pointer to an input file stream (source) and initializes lineCount and charCount.
LexicalAnalyser::LexicalAnalyser(std::ifstream *source)
{
    file = source; // change to buffer
    lineCount = 1; // Initialize line count to 1
    charCount = 1; // Initialize character count to 1
}
// Destructor: No specific cleanup required, so it's empty.
LexicalAnalyser::~LexicalAnalyser()
{
}
// Resets the file stream to the beginning.
void LexicalAnalyser::lexerReset()
{
    file->clear();
    file->seekg(0, ios::beg);
}
// Checks if a character is a punctuation symbol.
bool LexicalAnalyser::isPunctuation(char c)
{
    if ('(' == c || ')' == c || ';' == c || ',' == c)
        return true;
    else
        return false;
}
// Checks if a character is an operator symbol.
bool LexicalAnalyser::isOperatorSymbol(char c)
{
    // List of recognized operator symbols.
    if ('+' == c || '-' == c || '*' == c || '<' == c || '>' == c || '*' == c || '&' == c || '.' == c || '@' == c || '/' == c || ':' == c || '=' == c || '~' == c || '|' == c || '$' == c || '!' == c || '#' == c || '%' == c || '^' == c || '_' == c || '[' == c || ']' == c || '{' == c || '}' == c || '"' == c || '`' == c || '?' == c)
        return true;
    else
        return false;
}

// Checks if a character is a comment character (in this case, character '/').
bool LexicalAnalyser::isCommentChar(char c)
{
    if (39 == c) // char is '/'
        return true;
    else
        return false;
}

// Tokenizes an identifier.
string LexicalAnalyser::tokenIdentifier()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    do
    {
        file->get(nextChar);
        charCount++;
        tokStr += nextChar;
        nextPeek = file->peek();
    } while (!(EOF == nextPeek) && (isalpha(nextPeek) || isdigit(nextPeek) || (95 == nextPeek)));
    return tokStr;
}

// Tokenizes an integer.
string LexicalAnalyser::tokenInteger()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    do
    {
        file->get(nextChar);
        charCount++;
        tokStr += nextChar;
        nextPeek = file->peek();
    } while (EOF != nextPeek && isdigit(nextPeek));
    return tokStr;
}
// Tokenizes a string literal (enclosed in single quotes ').
string LexicalAnalyser::tokenStrings()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    do
    {
        file->get(nextChar);
        charCount++;
        tokStr += nextChar;
        nextPeek = file->peek();
        if (nextChar == '\\')
        {
            if ((nextPeek == 't' || nextPeek == 'n' || nextPeek == '\\' || nextPeek == '\''))
            {
                continue; // Valid escape sequence
            }
            else
            {
                printf("Invalid escape sequence\n");
                exit(0);
            }
        }
    } while (EOF != nextPeek && 39 != nextPeek);
    file->get(nextChar);
    tokStr += nextChar;
    return tokStr;
}
// Tokenizes consecutive spaces or tabs.
string LexicalAnalyser::tokenSpaces()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    do
    {
        file->get(nextChar);
        charCount++;
        if (10 == nextChar || 13 == nextChar)
        {
            charCount = 1;
            lineCount++;
        }
        tokStr += nextChar;
        nextPeek = file->peek();
    } while (EOF != nextPeek && isspace(nextPeek));
    return tokStr;
}
// Tokenizes comments starting with two consecutive slashes '//'.
string LexicalAnalyser::tokenComment()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    file->get(nextChar);
    charCount++;
    tokStr += nextChar;
    nextPeek = file->peek();
    if (47 == nextPeek)
    {
        file->get(nextChar);
        tokStr += nextChar;
        do
        {
            file->get(nextChar);
            tokStr += nextChar;
            nextPeek = file->peek();
        } while (EOF != nextPeek && !(10 == nextPeek || 13 == nextPeek));
        return tokStr;
    }
    else
    {
        return tokStr;
    }
}
// Tokenizes consecutive operator symbols.
string LexicalAnalyser::tokenOperator()
{
    string tokStr = "";
    char nextPeek;
    char nextChar;
    do
    {
        file->get(nextChar);
        charCount++;
        tokStr += nextChar;
        nextPeek = file->peek();
    } while (EOF != nextPeek && isOperatorSymbol(nextPeek));
    return tokStr;
}

Token *LexicalAnalyser::getNxtToken()
{
    Token *tok = new Token();
    string tokenizedLex;
    int nextChar;
    char readNext;
    nextChar = file->peek();

    if (isalpha(nextChar))
    {

        tokenizedLex = tokenIdentifier();
        tok->tokType = TOK_IDENTIFIER;
    }
    else if (isdigit(nextChar))
    {

        tokenizedLex = tokenInteger();
        tok->tokType = TOK_INTEGER;
    }
    else if (39 == nextChar)
    {

        tokenizedLex = tokenStrings();
        tok->tokType = TOK_STRING;
    }
    else if (isspace(nextChar))
    {

        tokenizedLex = tokenSpaces();
        tok->tokType = TOK_DELETE;
    }
    else if (isPunctuation(nextChar))
    {

        file->get(readNext);
        charCount++;
        tokenizedLex = readNext;
        tok->tokType = TOK_PUNCTUATION;
    }
    else if (47 == nextChar)
    {

        tokenizedLex = tokenComment();
        if (!tokenizedLex.compare("/"))
        {
            tok->tokType = TOK_OPERATOR;
        }
        else
        {
            tok->tokType = TOK_DELETE;
        }
    }
    else if (isOperatorSymbol(nextChar))
    {

        tokenizedLex = tokenOperator();
        tok->tokType = TOK_OPERATOR;
    }
    else if (EOF == nextChar)
    {

        tok->tokType = TOK_EOF;
    }

    tok->tokValue = tokenizedLex;
    tok->chrCount = charCount;
    tok->lineCount = lineCount;

    return tok;
}
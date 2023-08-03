Problem Description
You are required to implement a lexical analyzer and a parser for the RPAL language. Refer
RPAL_Lex.pdf for the lexical rules and RPAL_Grammar.pdf for the grammar details.
You should not use ‘lex’. ‘yacc’ or any such tool.
Output of the parser should be the Abstract Syntax Tree (AST) for the given input program. Then
you need to implement an algorithm to convert the Abstract Syntax Tree (AST) in to Standardize
Tree (ST) and implement CSE machine.
Your program should be able to read an input file which contains a RPAL program.
Output of your program should match the output of “rpal.exe“ for the relevant program.
You must use C/C++ or Java for this project.

Input and Output Requirements

Your program should execute using the following

For c/c++:
$ ./rpal20 file_name
For java:
$ java rpal20 file_name

Where file_name is the name of the file that has the RPAL program as the input.

Input Format

Eg: Here is one input file
let Sum(A) = Psum (A,Order A )
where rec Psum (T,N) = N eq 0 -> 0
 | Psum(T,N-1)+T N
in Print ( Sum (1,2,3,4,5) )

Output Format

Output of the above program is:
15

Executing the Program for C/C++:
 You should create an executable file named rpal20. Provide a makefile to build your
project. Make sure your program can be run with the following sequence.

 > tar xvf <submission_file>.tar
 >make
 > ./rpal20 rpal_test_programs/rpal_01 > output.01
 > diff output.01 rpal_test_programs/output01.test
 > ./rpal20 rpal_test_programs/rpal_02 > output.02
 > diff output.02 rpal_test_programs/output02.test
 …
Executing the Program for java:
You should create rpal20.java as the main class. Provide a makefile to build you project.
Make sure your program can be run with the following sequence.
 > tar xvf <submission_file>.tar
 >make
 > java rpal20 rpal_test_programs/rpal_01 > output.01
 > diff output.01 rpal_test_programs/output01.test
 > java rpal20 rpal_test_programs/rpal_02 > output.02
 > diff output.02 rpal_test_programs/output02.test


RPAL’s Phrase Structure Grammar:
# Expressions ############################################
E -> ’let’ D ’in’ E => ’let’
-> ’fn’ Vb+ ’.’ E => ’lambda’
-> Ew;
Ew -> T ’where’ Dr => ’where’
-> T;
# Tuple Expressions ######################################
T -> Ta ( ’,’ Ta )+ => ’tau’
-> Ta ;
Ta -> Ta ’aug’ Tc => ’aug’
-> Tc ;
Tc -> B ’->’ Tc ’|’ Tc => ’->’
-> B ;
# Boolean Expressions ####################################
B ->B’or’ Bt => ’or’
-> Bt ;
Bt -> Bt ’&’ Bs => ’&’
-> Bs ;
Bs -> ’not’ Bp => ’not’
-> Bp ;
Bp -> A (’gr’ | ’>’ ) A => ’gr’
-> A (’ge’ | ’>=’) A => ’ge’
-> A (’ls’ | ’<’ ) A => ’ls’
-> A (’le’ | ’<=’) A => ’le’
-> A ’eq’ A => ’eq’
-> A ’ne’ A => ’ne’
-> A ;
# Arithmetic Expressions #################################
A ->A’+’ At => ’+’
-> A ’-’ At => ’-’
-> ’+’ At
-> ’-’ At => ’neg’
-> At ;
At -> At ’*’ Af => ’*’
-> At ’/’ Af => ’/’
-> Af ;
Af -> Ap ’**’ Af => ’**’
-> Ap ;
Ap -> Ap ’@’ ’<IDENTIFIER>’ R => ’@’
-> R ;
# Rators And Rands #######################################
R ->RRn => ’gamma’
-> Rn ;
Rn -> ’<IDENTIFIER>’
-> ’<INTEGER>’
-> ’<STRING>’
-> ’true’ => ’true’
-> ’false’ => ’false’
-> ’nil’ => ’nil’
-> ’(’ E ’)’
-> ’dummy’ => ’dummy’ ;
# Definitions ############################################
D -> Da ’within’ D => ’within’
-> Da ;
Da -> Dr ( ’and’ Dr )+ => ’and’
-> Dr ;
Dr -> ’rec’ Db => ’rec’
-> Db ;
Db -> Vl ’=’ E => ’=’
-> ’<IDENTIFIER>’ Vb+ ’=’ E => ’fcn_form’
-> ’(’ D ’)’ ;
# Variables ##############################################
Vb -> ’<IDENTIFIER>’
-> ’(’ Vl ’)’
-> ’(’ ’)’ => ’()’;
Vl -> ’<IDENTIFIER>’ list ’,’ => ’,’?;



RPAL’s Phrase Structure Grammar:
# Expressions ############################################
E -> ’let’ D ’in’ E => ’let’
-> ’fn’ Vb+ ’.’ E => ’lambda’
-> Ew;
Ew -> T ’where’ Dr => ’where’
-> T;
# Tuple Expressions ######################################
T -> Ta ( ’,’ Ta )+ => ’tau’
-> Ta ;
Ta -> Ta ’aug’ Tc => ’aug’
-> Tc ;
Tc -> B ’->’ Tc ’|’ Tc => ’->’
-> B ;
# Boolean Expressions ####################################
B ->B’or’ Bt => ’or’
-> Bt ;
Bt -> Bt ’&’ Bs => ’&’
-> Bs ;
Bs -> ’not’ Bp => ’not’
-> Bp ;
Bp -> A (’gr’ | ’>’ ) A => ’gr’
-> A (’ge’ | ’>=’) A => ’ge’
-> A (’ls’ | ’<’ ) A => ’ls’
-> A (’le’ | ’<=’) A => ’le’
-> A ’eq’ A => ’eq’
-> A ’ne’ A => ’ne’
-> A ;
# Arithmetic Expressions #################################
A ->A’+’ At => ’+’
-> A ’-’ At => ’-’
-> ’+’ At
-> ’-’ At => ’neg’
-> At ;
At -> At ’*’ Af => ’*’
-> At ’/’ Af => ’/’
-> Af ;
Af -> Ap ’**’ Af => ’**’
-> Ap ;
Ap -> Ap ’@’ ’<IDENTIFIER>’ R => ’@’
-> R ;
# Rators And Rands #######################################
R ->RRn => ’gamma’
-> Rn ;
Rn -> ’<IDENTIFIER>’
-> ’<INTEGER>’
-> ’<STRING>’
-> ’true’ => ’true’
-> ’false’ => ’false’
-> ’nil’ => ’nil’
-> ’(’ E ’)’
-> ’dummy’ => ’dummy’ ;
# Definitions ############################################
D -> Da ’within’ D => ’within’
-> Da ;
Da -> Dr ( ’and’ Dr )+ => ’and’
-> Dr ;
Dr -> ’rec’ Db => ’rec’
-> Db ;
Db -> Vl ’=’ E => ’=’
-> ’<IDENTIFIER>’ Vb+ ’=’ E => ’fcn_form’
-> ’(’ D ’)’ ;
# Variables ##############################################
Vb -> ’<IDENTIFIER>’
-> ’(’ Vl ’)’
-> ’(’ ’)’ => ’()’;
Vl -> ’<IDENTIFIER>’ list ’,’ => ’,’?;

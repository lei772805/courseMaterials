#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <iostream>
#include <string>

/* meaningless comment*/
using namespace std;
class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
*/
enum tokenEnumType { 

    // Keywords
    letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd, 
    printKwd, readKwd, writeKwd, 
    integerKwd, floatKwd, booleanKwd, stringKwd, 
    trueKwd, falseKwd, 

    headKwd, tailKwd, nullKwd, 
    mapKwd, filterKwd, foldKwd, 
    zipKwd,

    // Constants
    intConst, floatConst, stringConst, 

    // Names
    variableName ,
    
    // Punctuation
    leftParen, rightParen, 
    leftSquare, rightSquare, leftAngle, rightAngle,
    colon, colonColon, comma, semiColon, 
    equalsSign, 

    plusSign, star, dash, forwardSlash, percent, 
    backSlash, arrow, 
    equalsEquals, 
    lessThanEquals, greaterThanEquals, notEquals, 
    plusPlus, dotDot, 
    tupleOp,

    endOfFile ,
    lexicalError
} ;
typedef enum tokenEnumType terminal_t ;

// Below you need to write your class declarations for Token and Scanner.
class Token {
	public:
		terminal_t terminal;
		string lexeme;
		Token* next;
		Token();
		Token(const Token&);
		Token(string, terminal_t&, Token*);
		
};



class Scanner{
	public:
		Token *scan (const char*);
	
};	

#endif /* SCANNER_H */

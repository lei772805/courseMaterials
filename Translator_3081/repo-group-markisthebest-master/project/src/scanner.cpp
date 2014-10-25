
#include "readInput.h"
#include "scanner.h"
#include "regex.h"
#include <regex.h>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
Token* HEAD = NULL;
Token* TAIL;
Token* LAST;
Token* temp;
int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
								  regex_t *lineComment,
                                  const char *text);
                                  

regex_t regexArray[55] = {
	*makeRegex ("^[\n\t\r ]+"),
    *makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/"),
    *makeRegex ("^//[^\n]*\n"),
	*makeRegex ("^let"), 
	*makeRegex ("^in"),
	*makeRegex ("^end"),
	*makeRegex ("^if"),
    *makeRegex ("^then"),
	*makeRegex ("^else"),
	*makeRegex ("^print"),
	*makeRegex ("^read"),
	*makeRegex ("^write"),
	*makeRegex ("^Integer"),
	*makeRegex ("^Float"),
	*makeRegex ("^Boolean"),
	*makeRegex ("^String"),
	*makeRegex ("^true"),
	*makeRegex ("^false"),
	*makeRegex("^head"),
	*makeRegex("^tail"),
	*makeRegex("^null"),	
	*makeRegex("^map"),
	*makeRegex("^filter"),
	*makeRegex("^fold"),
	*makeRegex("^zip"),
	*makeRegex("^[-]?[0-9]+"),
	*makeRegex("^[+|-]?[0-9]*\\.[0-9]+"),
	*makeRegex("^\"([^\"\\\\]|\\\\.)*\""),
	*makeRegex("^([a-zA-Z_][a-zA-Z0-9_]*)"),	
	*makeRegex("^\\("),	
	*makeRegex("^\\)"),
	*makeRegex("^\\["),
	*makeRegex("^\\]"),
	*makeRegex("^<"),
	*makeRegex("^>"),	
	*makeRegex("^\\:"),
	*makeRegex("^\\:\\:"),
	*makeRegex("^\\,"),
	*makeRegex("^\\;"),
    *makeRegex("^\\="),
	*makeRegex("^\\+"),
	*makeRegex("^\\*"),
	*makeRegex("^\\-"),
    *makeRegex("^\\/"),
	*makeRegex("^\\%"),
    *makeRegex("^\\\\"),
	*makeRegex("^->"),
	*makeRegex("^\\=\\="),
	*makeRegex("^<="),
	*makeRegex("^>="),
	*makeRegex("^\\!\\="),
	*makeRegex("^\\+\\+"),
	*makeRegex("^\\.\\."),
	*makeRegex("^\\#[1-9]+[0-9]*"),
	*makeRegex("^\0")
	};

Token* Scanner::scan(const char* arg){
	/* The Token LAST keeps track of the last token in the linked-list,
	 * which is the endOfFile token. Token HEAD is the token that's the 
	 * head of the linked-list, and this token will never change. Token
	 * TAIL is the token before the endOfFile token, or the last token of
	 * the file, that's not the endOfFile token. From the start, we only 
	 * have the the Token LAST, which is also HEAD and TAIL at this point
	 * since this list is currently empty. */
	LAST = new Token;
	LAST->terminal = endOfFile;
	LAST->lexeme = "\0";
	LAST->next = NULL;	
	HEAD = LAST;
	TAIL = HEAD;
	const char *text = arg;
	string lex; //keeps track the lexeme from the input text.
	terminal_t terminal; //keeps track of the type of the current string.
	
	/*This is all the terminal and their regex's that's used by the scanner. */
	
	

	
	int numMatchedChars = 0;
	/* Removes all comments and spaces from the beginning of the text.*/
	numMatchedChars = consumeWhiteSpaceAndComments (&regexArray[0], &regexArray[1], 
													&regexArray[2], text); 
	text = text + numMatchedChars;
	
	int maxNumMatchedChars = 0;
	
	while(text[0] != '\0' ){
		maxNumMatchedChars = 0; 
		/*If none of the terminals matches the input, then it has 
		 * to be a lexicalError, so that's initialized as the terminal. */
		terminal = lexicalError; 
		
		int i;
		for(i = 3; i < sizeof(regexArray)/sizeof(regexArray[0]); i++) {
			numMatchedChars = matchRegex(&regexArray[i], text);
			if(numMatchedChars > maxNumMatchedChars){
			   maxNumMatchedChars = numMatchedChars;
			   terminal = static_cast<terminal_t>(i-3); //assume that it is the current terminal, if found a longer match, this will be changed.
			   lex = text; //this turns the input text to a string type so we can use string methods.
			   lex = lex.substr(0, maxNumMatchedChars); //this keeps tracks of the lexeme or the current word we are matching.
			}
		}
		
	    if(terminal == lexicalError){
			/*If the terminal is a lexicalError, then we can say that the
			 * length of the string is a character. */
			lex = text;
			lex = lex.substr(0,1); 
			/*This is creates the linked-list. If HEAD is still LAST, means
			 * that we have not yet put in anything into the list, therefore
			 * we insert terminal and lexeme for HEAD, and assign TAIL to point
			 * to HEAD since that's the first and also the last Token. */
			if(HEAD == LAST) {
				temp = LAST;
				HEAD = new Token;
				HEAD->terminal = terminal;
				HEAD->lexeme = lex;	
				HEAD->next = temp;
				TAIL = HEAD;
			}
			/*If HEAD is not LAST meaning list is not empty, then we can make
			 * a new node/Token, and insert this between TAIL and LAST then 
			 * re-assign TAIL to be that new Token. */
			else {
				Token* node = new Token;
				temp = LAST;
				TAIL->next = node;
				node->lexeme = lex;
				node->terminal = terminal;
				node->next = temp;
				TAIL = node;
			}
			text = text + 1;
		}
		else{
			if(HEAD == LAST) {
				temp = LAST;
				HEAD = new Token;
				HEAD->terminal = terminal;
				HEAD->lexeme = lex;	
				HEAD->next = temp;
				TAIL = HEAD;
			}
			else {
				Token* node = new Token;
				temp = LAST;
				TAIL->next = node;
				node->lexeme = lex;
				node->terminal = terminal;
				node->next = temp;
				TAIL = node;
			}
			text = text + maxNumMatchedChars;
			
		}
		
		/*Remove any spaces or comments until the next character. */ 
		numMatchedChars = consumeWhiteSpaceAndComments(&regexArray[0], &regexArray[1], &regexArray[2], text);
		text = text + numMatchedChars;
	    	
	}
	

	return HEAD;
	 
}
//Token::Token(const Token&)
int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
								  regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}

Token::Token() {
	terminal = lexicalError;
	lexeme = "empty string";
	next = NULL;	
}

Token::Token(const Token& arg) {
	terminal = arg.terminal;
	lexeme = arg.lexeme;
	next = arg.next;
}

Token::Token(string lex, terminal_t& term, Token* ptr){
	terminal = term;
	lexeme = lex;
	next = ptr;
}























	
	

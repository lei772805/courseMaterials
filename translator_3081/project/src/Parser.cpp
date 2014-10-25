/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk

   This algorithms is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work comes from that
   of Based on algorithms described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: March 4, 2014.
*/


#include <stdlib.h>
#include "Parser.h"
#include "scanner.h"
#include "ExtToken.h"
#include "AST.h"

#include <assert.h>

using namespace std ;

Parser::Parser ( ) { } 

/*! This contains the parse methods for our parser. From last iteration we
 * were able to make a linkedlist of tokens of our text. From that, we 
 * were able to parse the linkedlist from beginning to end and form the 
 * AST.
 * 
 * For each of these method, they parse different productions of the 
 * functional language kix. The method parseXXXX parses a class of XXXX 
 * into different class sturctures of the possible inputs to create a 
 * class which is a subclass of XXXX. 
 * 
 * For example, parseExprList has two possible different productions,
 * one which only contains an Expr, another is a list of Expr. For this 
 * parse method, we parse it by Expr first, since we know the first token
 * of an ExprList string has to start with an Expr, and save that into
 * a ParseResult structure. After that, we check if the new token is a 
 * comma. If the next token is a comma, then we for sure know that this 
 * is not a single Expr we are trying to parse, so we parse the ExprList
 * and save that in other ParseResult variable and make an AST of that 
 * with the two ParseResult variables by making a new AST with the proper
 * subclass constructor. */

/*!This function use to initialize a new scanner of a text string, and
 * initialize currToken to the list of tokens that we created. */ 
void Parser::initialize (const char *text) {
	Scanner *s = new Scanner();
	tokens = extendTokenList (this, s->scan(text));
	assert(tokens!=NULL);
	currToken = tokens;
}

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        Scanner *s = new Scanner() ;
        tokens = extendTokenList ( this, s->scan (text) ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

// Program DONE -
ParseResult Parser::parseProgram () {
    ParseResult pr;
	pr.ast = NULL;
    // Program ::= DeclList
    ParseResult dl = parseDeclList() ;
    pr.ast = new startProgram(dynamic_cast<DeclList*>(dl.ast));

    // verify that ast field is a DeclList pointer
    match(endOfFile) ;

    return pr ;
}

// DeclList DONE -
ParseResult Parser::parseDeclList () {
    ParseResult pr ;
    // DeclList ::= Decl
    ParseResult decl = parseDecl() ;
    assert(decl.ast != NULL);
    // verify that ast field is a Declt pointer

    if ( nextIs(variableName) ) {
        // DeclList ::= Decl DeclList
       ParseResult DL = parseDeclList() ;
       pr.ast = new multiDeclList(dynamic_cast<Decl*>(decl.ast), dynamic_cast<DeclList*>(DL.ast));
        // verify that ast field is a DeclList pointer
    }
    else {
		//emptyList
		pr.ast = new declDeclList(dynamic_cast<Decl*>(decl.ast));
    }

    return pr ;
}

// Decl DONE -
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl ::= variableName '::' TypeExpr '=' Expr ';'
	
    match(variableName) ;
	string var = prevToken->lexeme;
    match(colonColon) ;

    ParseResult TE = parseTypeExpr() ;
    	
    // verify that ast field is a TypeExpr pointer
    match(equalsSign) ;
	ParseResult expr = parseExpr(0) ;
	
	pr.ast = new normDecl(var, dynamic_cast<TypeExpr*>(TE.ast), dynamic_cast<Expr*>(expr.ast));
    match(semiColon) ;
	assert(pr.ast);
	
    return pr ;
}

// TypeExpr DONE -
ParseResult Parser::parseTypeExpr () {
    ParseResult pr ;
    if ( attemptMatch(integerKwd) ) { 
        // TypeExpr ::= 'Integer' DONE -
        pr.ast = new ConstTypeExpr( prevToken->lexeme);
    }

    else if ( attemptMatch(floatKwd) ) {
        // TypeExpr ::= 'Float' DONE -
        pr.ast = new ConstTypeExpr( prevToken->lexeme);
    }

    else if ( attemptMatch(stringKwd) ) {
        // TypeExpr ::= 'String' DONE -
        pr.ast = new ConstTypeExpr(prevToken->lexeme);
    }

    else if ( attemptMatch(booleanKwd) ) {
        // TypeExpr ::= 'Boolean' DONE -
        pr.ast = new ConstTypeExpr(prevToken->lexeme);
    }

    else if ( attemptMatch(leftSquare) ) {
        // TypeExpr ::= '[' TypeExpr ']' DONE -
		
        ParseResult TE = parseTypeExpr() ;
        pr.ast = new BracketTypeExpr(dynamic_cast<TypeExpr*>(TE.ast));

        match(rightSquare) ;

    }

    else if ( attemptMatch(leftParen) ) {
        if ( nextIs(variableName) ) {
            // TypeExpr ::= '(' TypeBindingList ')' '->' TypeExpr DONE -

            ParseResult BL = parseTypeBindingList() ;
			
            match(rightParen) ;
            match(arrow) ;

            ParseResult TE = parseTypeExpr() ;
            pr.ast = new BindListTypeExpr(dynamic_cast<TypeBindingList*>(BL.ast), 
										dynamic_cast<TypeExpr*>(TE.ast));

        } else {
            // TypeExpr ::= '(' TypeExprList ')'  DONE -
            ParseResult TEL = parseTypeExprList() ;
            pr.ast = new listTypeExpr(dynamic_cast<TypeExprList*>(TEL.ast));
            match(rightParen) ;
        }
    }
    else {
        throw ( makeErrorMsg ( currToken->terminal ) ) ;
    }

    return pr ;
}

// TypeExprList DONE
ParseResult Parser::parseTypeExprList () {
    ParseResult pr ;
    // TypeExprList ::= TypeExpr -
    ParseResult TE = parseTypeExpr() ;
    
    if ( attemptMatch(comma) ) {
        // TypeExprList ::= TypeExpr ',' TypeExprList
        ParseResult TEL = parseTypeExprList () ;
        pr.ast = new multiTypeExprList(dynamic_cast<TypeExpr*>(TE.ast),
										dynamic_cast<TypeExprList*>(TEL.ast));

    }
    else {
       //no TypeExprs follow the one matched previously in this function. 
		pr.ast = new oneTypeExprList( dynamic_cast<TypeExpr*>(TE.ast));
    }    
    
    return pr ;
}

// TypeBindingList DONE-
ParseResult Parser::parseTypeBindingList () {
    ParseResult pr ;
    // TypeBindingList ::= TypeBinding -

    ParseResult TB = parseTypeBinding () ;

    if ( attemptMatch(comma) ) {
        // TypeBindingList ::= TypeBinding ',' TypeBindingList
        ParseResult TBL = parseTypeBindingList () ;
        pr.ast = new multiTypeBindingList(dynamic_cast<TypeBinding*>(TB.ast),
										dynamic_cast<TypeBindingList*>(TBL.ast));

    }
    else {
		pr.ast = new oneTypeBindingList( dynamic_cast<TypeBinding*>(TB.ast));
    }
    
    return pr ;
}

// TypeBinding DONE -
ParseResult Parser::parseTypeBinding () {
    ParseResult pr ;
    // TypeBinding ::= variableName '::' TypeExpr
    match(variableName) ;
    string var = prevToken->lexeme;
    match(colonColon) ;
	
    ParseResult TE = parseTypeExpr () ;
	pr.ast = new TypedeclTypeBinding(var, dynamic_cast<TypeExpr*>(TE.ast));
    return pr ;
}

// OptionalExprList DONE
ParseResult Parser::parseOptionalExprList () {
    ParseResult pr ;
    if ( nextIs(rightSquare) || nextIs(rightParen) ) {
        // OptionalExprList ::= <<empty>>
        pr.ast = new listOptionalExprList(NULL);
    } else {
        // OptionalExprList ::= ExprList
        ParseResult EL = parseExprList() ;
        pr.ast = new listOptionalExprList(dynamic_cast<ExprList*>(EL.ast));

    }
    return pr ;
}

// ExprList  DONE
ParseResult Parser::parseExprList () {
    ParseResult pr ;
    // ExprList ::= Expr

    ParseResult expr = parseExpr(0) ;

    if ( attemptMatch(comma) ) {
        // ExprList ::= Expr ',' ExprList
        ParseResult EL = parseExprList() ;
		pr.ast = new multiExprList(dynamic_cast<Expr*>(expr.ast), dynamic_cast<ExprList*>(EL.ast));
    }
    else {
		//empty listExprs
		pr.ast = new oneExprList(dynamic_cast<Expr*>(expr.ast));
    }
    
    return pr ;
}

// Expr-
ParseResult Parser::parseExpr (int rbp) {
    /*! Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }

    return left ;
}

/*
 * parse methods for productions
 * -----------------------------
 */

// Expr ::= variableName DONE-
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    
    return pr ;
}

// Expr ::= intConst DONE-
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    
    return pr ;
}

// Expr ::= floatConst DONE-
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    
    return pr ;
}

// Expr ::= stringConst DONE-
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    
    return pr ;
}

// Expr ::= trueKwd DONE -
ParseResult Parser::parseTrueKwd ( ) {
    ParseResult pr ;
    match ( trueKwd ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    
    return pr ;
}

// Expr ::= falseKwd DONE-
ParseResult Parser::parseFalseKwd ( ) {
    ParseResult pr ;
    match ( falseKwd ) ;
    pr.ast = new ConstExpr(prevToken->lexeme);
    return pr ;
}


ParseResult Parser::parseNestedOrTupleExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    
    // Expr ::= '(' Expr ')'  DONE
   ParseResult e1 = parseExpr(0) ; 

    if ( attemptMatch(comma) ) { 
        // Expr ::= '(' Expr ',' ExprList ')'  DONE
        ParseResult el = parseExprList() ;
        pr.ast = new multipleExpr(dynamic_cast<Expr*>(e1.ast), dynamic_cast<ExprList*>(el.ast));
    }
    else {
		//no ExprList to parse
		pr.ast = new singleExpr(dynamic_cast<Expr*>(e1.ast)); 
    }

    match(rightParen) ;

    return pr ;
}

// Expr ::= '[' ExprList ']'  Original documentation
// Expr ::= '[' OptionalExprList ']'  Original implementation

// Expr ::= '[' OptionalExprList ']'  before adding TypeExpr to literals
//       |  '[' Expr '..' Expr ']'     

// Expr ::= '[' '<' TypeExpr '>' OptionalExprList ']'  after TypeExpr
//       |  '[' Expr '..' Expr ']'     

ParseResult Parser::parseBraketListExpr ( ) {
    ParseResult pr ;
    match ( leftSquare ) ;

    if ( attemptMatch(leftAngle) ) {

        ParseResult TE = parseTypeExpr() ;
        match(rightAngle) ;

        ParseResult OEL = parseOptionalExprList() ; 
        pr.ast = new longExpr(dynamic_cast<TypeExpr*>(TE.ast), dynamic_cast<OptionalExprList*>(OEL.ast));

    }
    else {
		ParseResult e1 = parseExpr(0); 
        match(dotDot) ; 
        ParseResult e2 = parseExpr(0); 
        pr.ast = new rangeExpr(dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast));

    }
    match(rightSquare) ;
    return pr ;
}

// Expr ::= variableName '(' OptionnalExprList ')'  DONE
ParseResult Parser::parseFunctionCall ( ParseResult left ) {
    // parser has already matched variableName on left.ast


    ParseResult pr ;
	ConstExpr* varName = dynamic_cast<ConstExpr*>(left.ast);
	string vn = varName->unparse();
    match ( leftParen ) ;

    ParseResult OEL = parseOptionalExprList() ; 

    match(rightParen) ;
    pr.ast = new funcExpr(vn, dynamic_cast<OptionalExprList*>(OEL.ast));

    return pr ;
}

// Expr ::= Expr '+' Expr  DONE-
ParseResult Parser::parseAddition ( ParseResult left ) {
    // parser has already matched left expression 
	
    ParseResult pr ;
    match ( plusSign ) ;
    ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, plusSign));
    
    return pr ;

}

// Expr ::= Expr '*' Expr  DONE-
ParseResult Parser::parseMultiplication ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    match ( star ) ;
	ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, star));

    return pr ;
}
// Expr ::= Expr '-' Expr  DONE-
ParseResult Parser::parseSubtraction ( ParseResult left ) {
    // parser has already matched left expression 
	ParseResult pr ;
    match (dash) ;
	ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, dash));

    return pr ;
  
}
// Expr ::= Expr '/' Expr  DONE-
ParseResult Parser::parseDivision ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( forwardSlash ) ;
	ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, forwardSlash));

    return pr ;
}
// Expr ::= Expr '%' Expr  DONE-
ParseResult Parser::parseModulus ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    match ( percent ) ;
	ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, percent));
    return pr ;
}

// Expr ::= Expr '==' Expr  DONE
// Expr ::= Expr '<' Expr
// Expr ::= Expr '>' Expr
// Expr ::= Expr '>=' Expr
// Expr ::= Expr '<=' Expr
// Expr ::= Expr '!=' Expr
/*! Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
	terminal_t term = currToken->terminal;
	assert(term == equalsEquals ||
			term == leftAngle ||
			term == rightAngle ||
			term == lessThanEquals ||
			term == greaterThanEquals ||
			term == notEquals);
    nextToken( ) ;
    // just advance token, since examining it in parseExpr led 
    // to this method being called.
    
    ParseResult right = parseExpr(prevToken->lbp());
    pr.ast = new BinOpExpr(dynamic_cast<Expr*>(left.ast),
							dynamic_cast<Expr*>(right.ast), term);

    return pr ;
}

// Expr ::= Expr ':' Expr  DONE-
ParseResult Parser::parseConsExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    match(colon) ; 
	ParseResult right = parseExpr(prevToken->lbp());

    Expr *leftExpr = dynamic_cast<Expr*>(left.ast);
    Expr *rightExpr = dynamic_cast<Expr*>(right.ast);
    pr.ast = dynamic_cast<Node*>(new BinOpExpr (leftExpr, rightExpr, colon));

    return pr ;
}

// Expr ::= Expr '++' Expr  DONE
ParseResult Parser::parseAppendExpr ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match(plusPlus) ; 
    ParseResult right = parseExpr( prevToken->lbp() ); 
    pr.ast = new BinOpExpr(dynamic_cast<Expr*>(left.ast),
							dynamic_cast<Expr*>(right.ast), plusPlus);

    return pr ;
}
/*
// Expr ::= '[' Expr '..' Expr ']'  DONE
ParseResult Parser::parseDotDotExpr ( ParseResult left ) {
    // parser has already matched left expression 
    Expr *e1 = dynamic_cast<Expr *>(left.ast) ;
    if ( ! e1 ) throw ( (string) "Bad cast of first Expr in ListRange" ) ;

    ParseResult pr ;

    match(dotDot) ; 

    ParseResult prExpr = parseExpr( prevToken->lbp() ); 

    Expr *e2 = dynamic_cast<Expr *>(prExpr.ast) ;
    if ( ! e2 ) throw ( (string) "Bad cast of second Expr in ListRange" ) ;
    throw ((string) "IN LIST RANGE");
    pr.ast = new ListRange (e1, e2) ;
    return pr ;
}
*/

// Expr ::= 'let' DeclList 'in' Expr 'end'  DONE-
ParseResult Parser::parseLetExpr ( ) {
    ParseResult pr ;
    match(letKwd) ; 
    ParseResult DL = parseDeclList() ;
    
    match(inKwd) ; 
    ParseResult expr = parseExpr(0); 
    
    match(endKwd) ; 
    pr.ast = new LetExpr(dynamic_cast<DeclList*>(DL.ast), dynamic_cast<Expr*>(expr.ast));

    return pr ;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr  DONE -
ParseResult Parser::parseIfThenElse ( ) {
    ParseResult pr ;
    match(ifKwd) ;
    ParseResult e1 = parseExpr(0) ;

    match(thenKwd) ;
    ParseResult e2 = parseExpr(0) ;
    
    match(elseKwd);
    ParseResult e3 = parseExpr(0) ;
	pr.ast = new ifExpr(dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast),
						dynamic_cast<Expr*>(e3.ast));
	
    return pr ;
}

// Expr ::= 'print' '(' Expr ')'  DONE-
ParseResult Parser::parsePrintExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(printKwd) ;
    match(leftParen) ;
	
    ParseResult expr = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new oneArgExpr(kwd, dynamic_cast<Expr*>(expr.ast));
	
    return pr ;

}

// Expr ::= 'write' '(' Expr, Expr ')'  -
ParseResult Parser::parseWriteExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(writeKwd) ;
    match(leftParen) ;
    ParseResult e1 = parseExpr(0) ;

    match(comma) ;
    ParseResult e2 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new twoArgExpr(kwd, dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast));
    return pr ;
}

// Expr ::= 'read' '<' TypeExpr '>' '(' Expr ')'
ParseResult Parser::parseReadExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(readKwd) ;
    match(leftAngle) ;
    ParseResult TE = parseTypeExpr() ;

    match(rightAngle) ;
    match(leftParen) ;
    ParseResult E = parseExpr(0) ;

    match(rightParen) ;
    pr.ast = new readExpr(kwd, dynamic_cast<TypeExpr*>(TE.ast),
							dynamic_cast<Expr*>(E.ast));

    return pr ;
}

// Expr ::= 'head' '(' Expr ')'
ParseResult Parser::parseHeadExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(headKwd) ;
    match(leftParen) ;

    ParseResult e1 = parseExpr(0) ;

    match(rightParen) ;
    pr.ast = new oneArgExpr(kwd, dynamic_cast<Expr*>(e1.ast));
   
    return pr ;
}

// Expr ::= 'tail' '(' Expr ')'
ParseResult Parser::parseTailExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(tailKwd) ;
    match(leftParen) ;

    ParseResult e1 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new oneArgExpr(kwd, dynamic_cast<Expr*>(e1.ast));
   
    return pr ;
}

// Expr ::= 'null' '(' Expr ')'
ParseResult Parser::parseNullExpr ( ) {
    ParseResult pr ;
    string kwd = currToken->lexeme;
    match(nullKwd) ;
    match(leftParen) ;

    ParseResult e1 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new oneArgExpr(kwd, dynamic_cast<Expr*>(e1.ast));
    
    return pr ;
}

// Expr ::= 'map' '(' Expr ',' Expr ')'-
ParseResult Parser::parseMapExpr ( ) {
    ParseResult pr ;
	string kwd = currToken->lexeme;
    match(mapKwd) ;
    match(leftParen) ;
    ParseResult e1 = parseExpr(0) ;

    match(comma) ;
    ParseResult e2 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new twoArgExpr(kwd, dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast));
   
    return pr ;
}

// Expr ::= 'filter' '(' Expr ',' Expr ')'-
ParseResult Parser::parseFilterExpr ( ) {
    ParseResult pr ;
	string kwd = currToken->lexeme;
    match(filterKwd) ;
    match(leftParen) ;
    ParseResult e1 = parseExpr(0) ;

    match(comma) ;
    ParseResult e2 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new twoArgExpr(kwd, dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast));		
    
    return pr ;
}

// Expr ::= 'fold' '(' Expr ',' Expr ',' Expr ')'
ParseResult Parser::parseFoldExpr ( ) {
    ParseResult pr ;
	string kwd = currToken->lexeme;
    match(foldKwd) ;
    match(leftParen) ;
    ParseResult e1 = parseExpr(0) ;

    match(comma) ;
    ParseResult e2 = parseExpr(0) ;

    match(comma) ;
    ParseResult e3 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new foldExpr(kwd, dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast), dynamic_cast<Expr*>(e3.ast));
   
    return pr ;
}

// Expr ::= 'zip' '(' Expr ',' Expr ')'-
ParseResult Parser::parseZipExpr ( ) {
    ParseResult pr ;
	string kwd = currToken->lexeme;
    match(zipKwd) ;
    match(leftParen) ;

    ParseResult e1 = parseExpr(0) ;

    match(comma) ;

    ParseResult e2 = parseExpr(0) ;

    match(rightParen) ;
    pr.ast = new twoArgExpr(kwd, dynamic_cast<Expr*>(e1.ast), dynamic_cast<Expr*>(e2.ast));

    return pr ;
}

// Expr ::= '\' TypeBindingsList 'arrow' Expr-
ParseResult Parser::parseLambdaExpr ( ) {
    ParseResult pr ;
    match(backSlash) ;

    ParseResult TBL = parseTypeBindingList() ;

    match(arrow) ;

    ParseResult E = parseExpr(0) ;
    pr.ast = new lambdaExpr(dynamic_cast<TypeBindingList*>(TBL.ast), dynamic_cast<Expr*>(E.ast));

    return pr ;
}

// Expr ::= tupleOp '(' Expr ')'
ParseResult Parser::parseProjectionExpr () {
    ParseResult pr ;
	string kwd = currToken->lexeme;
    match(tupleOp) ;
    
    ExtToken *op = prevToken ;

    match(leftParen) ;
    ParseResult e1 = parseExpr(0) ;

    match(rightParen) ;
	pr.ast = new oneArgExpr(kwd, dynamic_cast<Expr*>(e1.ast));
	
    return pr ;
}


// Helper function used by the parser.

void Parser::match (terminal_t tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (terminal_t tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (terminal_t tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( terminal_t terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( terminal_t terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( terminal_t terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}


#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <string>
//#include "i1_assessment_tests.h"

using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You will need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */
    

    // Here you should place tests for these other functions.
    // See i1_assessment_tests.h and your CxxTest lab for examples
    

    /* Remember, you have to write a test for each of the terminal
     * types in the language - each terminal is specified in the 
     * tokenEnumType in scanner.h
     */
     bool sameTerminals (Token *tks, int numTerms, terminal_t *ts) {
			Token *currentToken = tks ;
			int termIndex = 0 ;
			while (currentToken != NULL && termIndex < numTerms ) {
				if (currentToken->terminal != ts[termIndex]) {
					return false ;
				}
				else {
					++ termIndex ;
					currentToken = currentToken->next ;
				}
			}
			return true ;
		}

	void test_terminal_letKwd(){
		Token *tks = s->scan("let");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { letKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}
	
	void test_terminal_inKwd(){
		Token *tks = s->scan("in");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { inKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_endKwd(){
		Token *tks = s->scan("end");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { endKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_ifKwd(){
		Token *tks = s->scan("if");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { ifKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}
	
	void test_terminal_thenKwd(){
		Token *tks = s->scan("then");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { thenKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_elseKwd(){
		Token *tks = s->scan("else");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { elseKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}	

	void test_terminal_printKwd(){
		Token *tks = s->scan("print");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { printKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_readKwd(){
		Token *tks = s->scan("read");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { readKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_writeKwd(){
		Token *tks = s->scan("write");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { writeKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_integerKwd(){
		Token *tks = s->scan("Integer");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { integerKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_floatKwd(){
		Token *tks = s->scan("Float");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { floatKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_booleanKwd(){
		Token *tks = s->scan("Boolean");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { booleanKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_stringKwd(){
		Token *tks = s->scan("String");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { stringKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_trueKwd(){
		Token *tks = s->scan("true");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { trueKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_falseKwd(){
		Token *tks = s->scan("false");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { falseKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_headKwd(){
		Token *tks = s->scan("head");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { headKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_tailKwd(){
		Token *tks = s->scan("tail");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { tailKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_nullKwd(){
		Token *tks = s->scan("null");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { nullKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_mapKwd(){
		Token *tks = s->scan("map");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { mapKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_filterKwd(){
		Token *tks = s->scan("filter");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { filterKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_foldKwd(){
		Token *tks = s->scan("fold");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { foldKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_zipKwd(){
		Token *tks = s->scan("zip");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { zipKwd, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_intConst(){
		Token *tks = s->scan("23");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { intConst, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_floatConst(){
		Token *tks = s->scan("2.13");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { floatConst, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_stringConst(){
		Token *tks = s->scan("\"string\" \"string2\"");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { stringConst, stringConst, endOfFile };
		TS_ASSERT( sameTerminals (tks, 3, ts));
	}

	void test_terminal_variableName(){
		Token *tks = s->scan("variable");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { variableName, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_leftParen(){
		Token *tks = s->scan("(");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { leftParen, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_rightParen(){
		Token *tks = s->scan(")");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { rightParen, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_leftSquare(){
		Token *tks = s->scan("[");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { leftSquare, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_rightSquare(){
		Token *tks = s->scan("]");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { rightSquare, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_leftAngle(){
		Token *tks = s->scan("<");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { leftAngle, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_rightAngle(){
		Token *tks = s->scan(">");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { rightAngle, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_colon(){
		Token *tks = s->scan(":");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { colon, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_colonColon(){
		Token *tks = s->scan("::");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { colonColon, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_comma(){
		Token *tks = s->scan(",");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { comma, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_semiColon(){
		Token *tks = s->scan(";");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { semiColon, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_equalsSign(){
		Token *tks = s->scan("=");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { equalsSign, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_plusSign(){
		Token *tks = s->scan("+");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { plusSign, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_star(){
		Token *tks = s->scan("*");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { star, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_dash(){
		Token *tks = s->scan("-");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { dash, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_forwardSlash(){
		Token *tks = s->scan("/");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { forwardSlash, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_percent(){
		Token *tks = s->scan("%");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { percent, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_backSlash(){
		Token *tks = s->scan("\\");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { backSlash, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_arrow(){
		Token *tks = s->scan("->");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { arrow, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_equalsEquals(){
		Token *tks = s->scan("==");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { equalsEquals, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_lessThanEquals(){
		Token *tks = s->scan("<=");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { lessThanEquals, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_greaterThanEquals(){
		Token *tks = s->scan(">=");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { greaterThanEquals, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_notEquals(){
		Token *tks = s->scan("!=");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { notEquals, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_plusPlus(){
		Token *tks = s->scan("++");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { plusPlus, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_dotDot(){
		Token *tks = s->scan("..");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { dotDot, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}

	void test_terminal_tupleOp(){
		Token *tks = s->scan("#30");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { tupleOp, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	}


	void test_terminal_lexicalError(){
		Token *tks = s->scan("$");
		TS_ASSERT(tks != NULL);
		terminal_t ts[] = { lexicalError, endOfFile };
		TS_ASSERT( sameTerminals (tks, 2, ts));
	
	}

} ;






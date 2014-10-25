#include <cxxtest/TestSuite.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "readInput.h"
#include "scanner.h"
#include "ExtToken.h"
#include "Parser.h"
#include "ParseResult.h"
// One of the above has likely included the .h file with your
// definition of the Node class.  It is not named here so that
// you can name your files as you see fit.

using namespace std ;

class CodeGenTestSuite : public CxxTest::TestSuite 
{
public:
    int DEBUG ;
    Parser *p ;
    void test_setup_code ( ) {
        DEBUG = 0 ;
        p = new Parser() ;
    }

    void codeGen_tests ( const char *filename, int returnValue, 
                         string expectedOutput ) {

        string file = filename ;

        // 1. Test that the file can be parsed.
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;

        if (DEBUG) {
        TS_TRACE ("==================================================") ;
        TS_TRACE ("==================================================") ;
        TS_TRACE ( (string) "\n" + text + "\n" ) ;
        }

        ParseResult pr = p->parse ( text ) ;
        TSM_ASSERT ( file + " failed to parse.", pr.ok ) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT ( file + " failed to generate an AST.", pr.ast != NULL );

        // 3. Verify that the "unparsing" is non-empty.
        string code = pr.ast->cppCode() ;
        TSM_ASSERT ( file + " failed to generate non-empty cppCode.", 
                     code.length() > 0 ) ;

        // 4. Write C++ code to a file
        string cppFile = (string) "../samples/" + 
            file.substr (0, file.length()-3 ) + "cpp" ;

        ofstream cppOut( cppFile.c_str() );
        cppOut << pr.ast->cppCode() << endl ;

        // 5. Compile the code.
        string compileCommand = "cd ../samples; g++ " + cppFile ;
        int rc = system (compileCommand.c_str() ) ;
        TSM_ASSERT_EQUALS ( file + " compiled with errors", rc, 0 ) ;

        // 6. Run the code.
        // The 'system' command does not reliably return the return value 
        // when it is not zero.  Thus we save the value to a file.
        system ("cd ../samples; rm -f rc; rm -f output" );
        system ("cd ../samples; ./a.out > output; echo $? > rc") ;

        ifstream rcStream("../samples/rc") ;
        rcStream >> rc ;

        char *outputC = readInputFromFile ("../samples/output");
        string output(outputC);

        stringstream ss1 ;
        ss1 << "rc is " << rc << endl 
            << "output is: " << endl << output << endl ;
        if (DEBUG) {
        TS_TRACE ( ss1.str() ) ;
        }
        stringstream ss2 ;
        ss2 << file << " incorrect return value of " << rc 
            << " doesn't match expected value of " << returnValue ;
        TSM_ASSERT_EQUALS ( ss2.str(), rc, returnValue ) ;
        stringstream ss3 ;
        ss3 << file << " output of " << endl << output << endl 
            << " doesn't match expected output of " << expectedOutput ;
        TSM_ASSERT_EQUALS ( ss3.str(), output, expectedOutput ) ;
    }

    void test_codeGen_simple_1 ( ) {
        codeGen_tests ("../samples/simple_1.kix", 3, "") ;
    }

	 void test_codeGen_simple_2 ( ) {
        codeGen_tests ("../samples/simple_2.kix", 20, "") ;
    }
    
     void test_codeGen_simple_3 ( ) {
        codeGen_tests ("../samples/simple_3.kix", 9, "") ;
    }
    
     void test_codeGen_simple_4 ( ) {
        codeGen_tests ("../samples/simple_4.kix", 27, "") ;
    }
    
    void test_codeGen_print_int_1 ( ) {
        string expected = (string)
            "3\n" ;
        codeGen_tests ("../samples/print_int_1.kix", 0, expected) ;
    }
    
    void test_codeGen_print_int_2 ( ) {
        string expected = (string)
            "20\n" ;
        codeGen_tests ("../samples/print_int_2.kix", 0, expected) ;
    }
    
    void test_codeGen_read_int ( ) {
        string expected = (string)
            "35\n" ;
        codeGen_tests ("../samples/read_int.kix", 0, expected) ;
    }
    
    void test_codeGen_lists_1 ( ) {
        string expected = (string)
            "[ 1, 2, 3 ]\n" ;
        codeGen_tests ("../samples/lists_1.kix", 0, expected) ;
    }
	
	void test_codeGen_lists_2 ( ) {
        string expected = (string)
            "[ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]\n" ;
        codeGen_tests ("../samples/lists_2.kix", 0, expected) ;
    }
    
    void test_codeGen_lists_3 ( ) {
        string expected = (string)
            "[ 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 ]\n" ;
        codeGen_tests ("../samples/lists_3.kix", 0, expected) ;
    }
    
     void test_codeGen_squareFile ( ) {
        string expected = (string)
            "[ 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 ]\n" ;
        codeGen_tests ("../samples/squareFile.kix", 0, expected) ;
    }
    
    void test_codeGen_squaresMap ( ) {
        string expected = (string)
            "[ 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 ]\n" ;
        codeGen_tests ("../samples/squareMap.kix", 0, expected) ;
    }
    
    void test_codeGen_vecSum ( ) {
        string expected = (string)
            "55\n" ;
        codeGen_tests ("../samples/vecSum.kix", 0, expected) ;
    }
    
    void test_codeGen_evenSquares ( ) {
        string expected = (string)
            "[ 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 ]\n" +
            "[ 4, 16, 36, 64, 100 ]\n" ;
        codeGen_tests ("../samples/evenSquares.kix", 0, expected) ;
    }
    
    void test_codeGen_pairs ( ) {
        string expected = (string)
            "[ ( 1, 11 ), ( 2, 12 ), ( 3, 13 ), ( 4, 14 ), ( 5, 15 ), " +
            "( 6, 16 ), ( 7, 17 ), ( 8, 18 ), ( 9, 19 ), ( 10, 20 ) ]\n" ;
        codeGen_tests ("../samples/pairs.kix", 0, expected) ;
    }
    
} ;

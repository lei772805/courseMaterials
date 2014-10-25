#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "Parser.h"
#include "regex.h"
#include "AST.h"
#include <regex.h>

/*!Our test cases were implemented by the order that we did implement our 
 * classes basing of 13_assessment_tests.h. However, these test cases are 
 * not in the order that we tested them, and some of the input are random
 * input that we thought off our head the moment that we were writing the
 * tests. Some of the test cases are base of the sample kix codes given.
 * Each of these test cases tests each of the classes that we created and that
 * are possible to be tested. 
 * 
 * For these tests, we initialize a string(text), then we parse it by a 
 * method that we know that this text can be parsed by. Then we recreated 
 * a class out of the AST that was created by the parse, which is result.
 * Then we assert that result exist, and then unparse it to assert that it
 * matches with the input text.*/
using namespace std ;

class ParserTestSuite : public CxxTest::TestSuite 
{
	public:
		Scanner *s ;
		Parser *p ;
		void setUp ( ) {
			s = new Scanner() ;
			p = new Parser() ;
		}

		void tearDown() {
			delete s;
			delete p;
		}

	void test_optExprList() {
		const char* text = "add (x, y)";
		p->initialize(text);
		ParseResult pr = p->parseOptionalExprList();
		listOptionalExprList* result = dynamic_cast<listOptionalExprList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_expr_list() {
		const char* text = "a, b, c";
		p->initialize(text);
		ParseResult pr = p->parseExprList();
		multiExprList* result = dynamic_cast<multiExprList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_TypeBinding() {
		const char *text = "args::Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeBinding();
		TypedeclTypeBinding *result = dynamic_cast<TypedeclTypeBinding*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Decl() {
		const char *text = "args :: Integer = 1 + 1;";
		p->initialize(text);
		ParseResult pr = p->parseDecl();
		normDecl *result = dynamic_cast<normDecl*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Int_const_TE() {
		const char *text = "Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		ConstTypeExpr *result = dynamic_cast<ConstTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}

	void test_BL_TE() {
		const char *text = "( args::Integer ) -> Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		BindListTypeExpr *result = dynamic_cast<BindListTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Ft_const_TE() {
		const char *text = "Float";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		ConstTypeExpr *result = dynamic_cast<ConstTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Str_const_TE() {
		const char *text = "String";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		ConstTypeExpr *result = dynamic_cast<ConstTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Bool_const_TE() {
		const char *text = "Boolean";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		ConstTypeExpr *result = dynamic_cast<ConstTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_Brack_TE() {
		const char *text = "[Integer]";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		BracketTypeExpr *result = dynamic_cast<BracketTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_single_TEL() {
		const char *text = "Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeExprList();
		oneTypeExprList *result = dynamic_cast<oneTypeExprList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_multiple_TEL() {
		const char *text = "Integer, Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeExprList();
		multiTypeExprList *result = dynamic_cast<multiTypeExprList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_list_TE() {
		const char *text = "(Integer)";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		listTypeExpr *result = dynamic_cast<listTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_int_const() {
		const char *text = "1";
		p->initialize(text);
		ParseResult pr = p->parseIntConst();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_varName() {
		const char *text = "hello";
		p->initialize(text);
		ParseResult pr = p->parseVariableName();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}

	void test_floatConst() {
		const char *text = "2.45";
		p->initialize(text);
		ParseResult pr = p->parseFloatConst();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_strConst() {
		const char *text = "\"random string\"";
		p->initialize(text);
		ParseResult pr = p->parseStringConst();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_truekwd() {
		const char *text = "true";
		p->initialize(text);
		ParseResult pr = p->parseTrueKwd();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_falsekwd() {
		const char *text = "false";
		p->initialize(text);
		ParseResult pr = p->parseFalseKwd();
		ConstExpr *result = dynamic_cast<ConstExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_addition() {
		const char *text = "1 + 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseAddition(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), "1 + 1");
	}
	
	void test_subtract() {
		const char *text = "1 - 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseSubtraction(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), "1 - 1");
	}
	
	void test_multi() {
		const char *text = "5 * 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseMultiplication(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_divide() {
		const char *text = "5 / 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseDivision(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_mod() {
		const char *text = "5 % 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseModulus(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_colon_expr() {
		const char *text = "4 : 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseConsExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_equalsequals_expr() {
		const char *text = "1 == 1";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_greater_expr() {
		const char *text = "4 > 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_lessThan_expr() {
		const char *text = "4 < 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_lessOrEqual_expr() {
		const char *text = "4 <= 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_greaterOrEqual_expr() {
		const char *text = "4 >= 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_notEqual_expr() {
		const char *text = "4 != 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseRelationalExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_plusPLus_expr() {
		const char *text = "4 ++ 5";
		p->initialize(text);
		ParseResult left = p->parseIntConst();
		ParseResult pr = p->parseAppendExpr(left);
		BinOpExpr *result = dynamic_cast<BinOpExpr*>(pr.ast);
		TS_ASSERT (result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_singleTypebindlist() {
		const char* text = "args::[String]";
		p->initialize(text);
		ParseResult pr = p->parseTypeBindingList();
		oneTypeBindingList* result = dynamic_cast<oneTypeBindingList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_multipleTypebindlist() {
		const char* text = "args::[String], args::[Integer]";
		p->initialize(text);
		ParseResult pr = p->parseTypeBindingList();
		multiTypeBindingList* result = dynamic_cast<multiTypeBindingList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_typeExprListBind() {
		const char* text = "( args::[String] ) -> Integer";
		p->initialize(text);
		ParseResult pr = p->parseTypeExpr();
		BindListTypeExpr* result = dynamic_cast<BindListTypeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
		
	}
	
	void test_decl() {
		const char* text = "main :: ( args::[String] ) -> Integer = 1 + 2;";
		p->initialize(text);
		ParseResult pr = p->parseDecl();
		normDecl* result = dynamic_cast<normDecl*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
		
	void test_singleDeclList() {
		const char* text = "main :: ( args::[String] ) -> Integer = 1 + 2;";
		p->initialize(text);
		ParseResult pr = p->parseDeclList();
		declDeclList* result = dynamic_cast<declDeclList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_multipleDeclList() {
		const char* text = "main :: ( args::[String] ) -> Integer = 1 + 2; main :: ( args::[String] ) -> Integer = 1 + 2;";
		p->initialize(text);
		ParseResult pr = p->parseDeclList();
		multiDeclList* result = dynamic_cast<multiDeclList*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_program() {
		const char* text = "main :: ( args::[String] ) -> Integer = 1 + 2;";
		p->initialize(text);
		ParseResult pr = p->parseProgram();
		startProgram* result = dynamic_cast<startProgram*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_let() {
		const char* text = "let x :: Integer = 4; y :: Integer = 5; in x * y end";
		p->initialize(text);
		ParseResult pr = p->parseLetExpr();
		LetExpr* result = dynamic_cast<LetExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_fold() {
		const char* text = "fold ( 1, 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseFoldExpr();
		foldExpr* result = dynamic_cast<foldExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_print() {
		const char* text = "print ( 1 )";
		p->initialize(text);
		ParseResult pr = p->parsePrintExpr();
		oneArgExpr* result = dynamic_cast<oneArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_head() {
		const char* text = "head ( 1 )";
		p->initialize(text);
		ParseResult pr = p->parseHeadExpr();
		oneArgExpr* result = dynamic_cast<oneArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_tail() {
		const char* text = "tail ( 1 )";
		p->initialize(text);
		ParseResult pr = p->parseTailExpr();
		oneArgExpr* result = dynamic_cast<oneArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_null() {
		const char* text = "null ( 1 )";
		p->initialize(text);
		ParseResult pr = p->parseNullExpr();
		oneArgExpr* result = dynamic_cast<oneArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}

	void test_singleExpression() {
		const char* text = "( 1 )";
		p->initialize(text);
		ParseResult pr = p->parseNestedOrTupleExpr();
		singleExpr* result = dynamic_cast<singleExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_multipleExpression() {
		const char* text = "( 1, 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseNestedOrTupleExpr();
		multipleExpr* result = dynamic_cast<multipleExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_tupleOp() {
		const char* text = "#1 ( 1 )";
		p->initialize(text);
		ParseResult pr = p->parseProjectionExpr();
		oneArgExpr* result = dynamic_cast<oneArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
		
	void test_write() {
		const char* text = "write ( 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseWriteExpr();
		twoArgExpr* result = dynamic_cast<twoArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_map() {
		const char* text = "map ( 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseMapExpr();
		twoArgExpr* result = dynamic_cast<twoArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_filter() {
		const char* text = "filter ( 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseFilterExpr();
		twoArgExpr* result = dynamic_cast<twoArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_zip() {
		const char* text = "zip ( 1, 1 )";
		p->initialize(text);
		ParseResult pr = p->parseZipExpr();
		twoArgExpr* result = dynamic_cast<twoArgExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_longExpr() {
		const char* text = "[ <Integer> 1, 2 ]";
		p->initialize(text);
		ParseResult pr = p->parseBraketListExpr();
		longExpr* result = dynamic_cast<longExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_rangeExpr(){
		const char* text = "[1 .. 2]";
		p->initialize(text);
		ParseResult pr = p->parseBraketListExpr();
		rangeExpr* result = dynamic_cast<rangeExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
	
	void test_ifThen(){
		const char* text = "if 1 then 2 else 3";
		p->initialize(text);
		ParseResult pr = p->parseIfThenElse();
		ifExpr* result = dynamic_cast<ifExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}
		
	void test_readExpr(){
		const char* text = "read <Integer> ( 3 )";
		p->initialize(text);
		ParseResult pr = p->parseReadExpr();
		readExpr* result = dynamic_cast<readExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
		
	}
		
	void test_lambdaExpr(){
		const char* text = "\\\ args::[String] -> 3";
		p->initialize(text);
		ParseResult pr = p->parseLambdaExpr();
		lambdaExpr* result = dynamic_cast<lambdaExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
		
	}
		
	void test_funcExpr(){
		const char* text = "args (1, 2)";
		p->initialize(text);
		ParseResult left = p->parseVariableName();
		ParseResult pr = p->parseFunctionCall(left);
		funcExpr* result = dynamic_cast<funcExpr*>(pr.ast);
		TS_ASSERT(result);
		TS_ASSERT_EQUALS(result->unparse(), text);
	}

} ;





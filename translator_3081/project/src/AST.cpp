#include "AST.h"
#include "ExtToken.h"

using namespace std ;

// Root
// ------------------------------------------------------------

/*!This files holds all the unparse methods for all the classes
 * in the AST.h file.*/


string Node :: unparse(){}
string startProgram::unparse(){return DL->unparse();}
string multiDeclList::unparse() {return D->unparse() + " " + DL->unparse();}
string declDeclList::unparse() {return decl->unparse();}
string normDecl::unparse(){return varName + " :: " + TE->unparse() + " = " + E->unparse() + ";";}
string listOptionalExprList::unparse(){return EL->unparse();}
string multiTypeExprList::unparse(){return TE->unparse() + ", " + TEL->unparse();}
string oneTypeExprList::unparse() {return TE->unparse();}
string listTypeExpr::unparse() {return "(" + TEL->unparse() + ")";}
string BindListTypeExpr::unparse(){return "( " +TBL->unparse() + " )" + " -> " + TE->unparse();}
string BracketTypeExpr::unparse() {return "[" + TE->unparse() + "]";}
string ConstTypeExpr::unparse() {return type;}
string multiTypeBindingList::unparse() {return TB->unparse() + ", " + TBL->unparse();}
string oneTypeBindingList::unparse() {return TB->unparse();}
string TypedeclTypeBinding::unparse(){return varName + "::" + TE->unparse();}
string oneExprList::unparse(){return E->unparse();}
string multiExprList::unparse(){return E->unparse() + ", " + EL->unparse();}
string rangeExpr::unparse(){return "[" + E1->unparse() + " .. " + E2->unparse() + "]";}
string ifExpr::unparse(){return "if " + E1->unparse() + " then " + E2->unparse() + " else " + E3->unparse();}
string LetExpr::unparse() {return "let " + DL->unparse() + " in " + E->unparse() + " end";}
string funcExpr::unparse() {return varName + " (" + OEL->unparse() + ")";}
string oneArgExpr::unparse(){return kwd + " ( " + E->unparse() + " )";}
string twoArgExpr::unparse(){return kwd + " ( " + E1->unparse() + ", " + E2->unparse() + " )";}
string lambdaExpr::unparse(){return "\\\ " + TBL->unparse() + " -> " + E->unparse();}
string longExpr::unparse(){return "[ <" + TE->unparse() + "> " + OEL->unparse() + " ]";}
string readExpr::unparse(){return varName + " <" + TE->unparse() + "> ( " + E->unparse() + " )";}
string foldExpr::unparse(){return kwd + " ( " + E1->unparse() + ", " + E2->unparse() + ", " + E3->unparse() + " )";}
string singleExpr::unparse(){return "( " + E1->unparse() + " )";}
string multipleExpr::unparse(){return "( " + E1->unparse() + ", " + EL->unparse() + " )";}
string ConstExpr::unparse(){return constString;}
string BinOpExpr::unparse() {
			/*! In this unparse, what we return base on the terminal type 
			 * we got from our parse method.*/
	switch(op) {
		case dash:
			return left->unparse() + " - " + right->unparse();
		case plusSign: 
			return left->unparse() + " + " + right->unparse();
		case star:
			return left->unparse() + " * " + right->unparse();
		case forwardSlash:
			return left->unparse() + " / " + right->unparse();
		case percent:
			return left->unparse() + " % " + right->unparse();
		case colon:
			return left->unparse() + " : " + right->unparse();
		case plusPlus:
			return left->unparse() + " ++ " + right->unparse();
		case equalsEquals:
			return left->unparse() + " == " + right->unparse();
		case leftAngle:
			return left->unparse() + " < " + right->unparse();
		case rightAngle:
			return left->unparse() + " > " + right->unparse();
		case greaterThanEquals:
			return left->unparse() + " >= " + right->unparse();
		case lessThanEquals:
			return left->unparse() + " <= " + right->unparse();
		case notEquals:
			return left->unparse() + " != " + right->unparse();
		default:
			return "";
	}
}





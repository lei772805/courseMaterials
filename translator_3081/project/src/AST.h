#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 
#include <list>

#include "scanner.h"

/*! AST.h contains all the declaration of all the classes. The classes
 * themselves are pretty explanatory. All non-terminal classes inherit
 * from the Node class. Each of the non-terminal has their own subclass
 * base on their production, and they inherit from non-terminal classes.
 * All of the classes also have an unparse method that unpares the nodes
 * from the AST back to the kix production form. All abstract classes 
 * excluding Node is a subclass of Node.*/
/*!This is Node, the root of the AST, and the base class of all classes 
 * in this file, the super class. */
class Node {
	/*!The unparse method is a virtual method that is used in all classes
	 * and is declared as pure virtual method. Also includes a desturctor.*/
public:
    virtual std::string unparse ( ) = 0 ;
    virtual std::string cppCode () {return "unimplemented!";} //-TODO iter4
    virtual std::string cppCode(int i) {return "unimplemented!";} 
    virtual ~Node() { }
} ;

/*!An abstract class of Program */
class Program: public Node {};
/*!An abstract class of DeclList */
class DeclList: public Node {};
/*!An abstract class of Decl */
class Decl: public Node {};
/*!An abstract class of OptionalExprList */
class OptionalExprList: public Node {};
/*!An abstract class of TypeExprList */
class TypeExprList: public Node {};
/*!An abstract class of TypeExpr */
class TypeExpr: public Node {};
/*!An abstract class of TypeBindingList */
class TypeBindingList: public Node {};
/*!An abstract class of TypeBinding */
class TypeBinding: public Node {};
/*!An abstract class of ExprList */
class ExprList: public Node {};
/*!An abstract class of Expr */
class Expr: public Node {};


/*!This is a subclass of Program, which has a list of declarations. It's
 * a symbol of a program.*/
class startProgram: public Program {
	/*! This constructor takes a DeclList
	 * 	The unparse method returns the production
	 * 	\param DL a list of declaration */
	public:
		startProgram(DeclList* _DL): DL(_DL){}
		std::string unparse();
		std::string cppCode(){return "#include \"../samples/common_header.h\" \nusing namespace std; \n" + DL->cppCode();}
	private:
		DeclList* DL;
};

/*!This is a subclass of DeclList, which use for a program has more than one 
 * declaration. */
class multiDeclList: public DeclList {
	/*! This constructor takes in a Decl and DeclList
	 * 	The unparse method returns the production
	 * 	\param D a declaration
	 * 	\param DL a list of declarations */
	public:
		multiDeclList(Decl* _D, DeclList* _DL): D(_D), DL(_DL){}
		std::string unparse();
		std::string cppCode() {return D->cppCode() + DL->cppCode();}
	private:
		Decl* D;
		DeclList* DL;
};

/*!This is a subclass of DeclList, which use for a program that has one
 * declaration. */
class declDeclList: public DeclList {
	/*! This constructor takes in a Decl
	 * 	The unparse method returns the production
	 * 	\param decl a declaration */
	public:
		declDeclList(Decl* _decl): decl(_decl) {}
		std::string unparse();
		std::string cppCode(){return decl->cppCode();}
	private:
		Decl* decl;
};

/*!This is a subclass of TypeExpr, which when an expression type is a 
 * binding type */
class BindListTypeExpr: public TypeExpr {
	/*! This constructor takes in a TypeBindingList and TypeExpr
	 * 	The unparse method returns the production
	 * 	\param TBL a list of binding types
	 * 	\param TE an expression type */
	public:
		BindListTypeExpr(TypeBindingList* _TBL, TypeExpr* _TE): TBL(_TBL), TE(_TE) {}
		TypeExpr* getTE() {return TE;}
		TypeBindingList* getTBL() {return TBL;}
		std::string unparse();
		std::string cppCode() {return TBL->cppCode();}
	private:
		TypeBindingList* TBL;
		TypeExpr* TE;
};

/*!This is a subclass of TypeExpr, which puts brackets around an 
 * expression type */
class BracketTypeExpr: public TypeExpr {
	/*! This constructor takes in a TypeExpr
	 * 	The unparse method returns the production
	 * 	\param TE an expression type*/
	public:
		BracketTypeExpr( TypeExpr* _TE) : TE(_TE){}
		std::string unparse();
		std::string cppCode() {return "MyList< " + TE->cppCode() + " >";}
		TypeExpr* getTE() {return TE;}
	private:
		TypeExpr* TE;
};


/*!This is a subclass of Expr, which contains all of the possible constants like Integer, Float ,etc.*/
class ConstExpr: public Expr {
	/*! This constructor takes in a string which could be a constant or keyword
	 * 	The unparse method returns the production
	 * 	\param constString is a constant or keyword*/
	public:
		ConstExpr(std::string _s) : constString(_s){}
		std::string unparse();
		std::string cppCode(){return constString;}
	private:
		std::string constString;
};


/*! This is a subclass of Decl, which is a declaration of a variable. */
class normDecl: public Decl {
	/*! This constructor takes in a variableName, TypeExpr and Expr
	 * 	The unparse method returns the production
	 * 	\param varName the variable name
	 * 	\param TE the type of the variable
	 * 	\param E the expression  */
	public:
		normDecl(string _varName, TypeExpr* _TE, Expr* _E): varName(_varName), TE(_TE), E(_E){}
		std::string unparse();
		std::string cppCode() {
			BindListTypeExpr* BLTE = dynamic_cast<BindListTypeExpr*>(TE);
			BracketTypeExpr* BTE = dynamic_cast<BracketTypeExpr*>(TE);
			if(varName == "main") {
				return "int " + varName +" ( int argc, char **argv ) {\n " +
				"MyList<string> " + "args" + " = mkArgs ( argc, argv) ;"
				 + "\n return " + E->cppCode() + " ;\n}";
			}
			
			else if(varName == "mkSquares"){
				return "MyList<int> " + varName + "(MyList<int> ns)" + "{\n return" + E->cppCode() + " ;\n}\n\n";
		
			}
			else if(BLTE != NULL) {
				return BLTE->getTE()->cppCode() + " " + varName + " (" +BLTE->getTBL()->cppCode() + 
				") {\n return" + E->cppCode() + " ;\n}\n\n";
			}
			else if(BTE != NULL) {
				return BTE->cppCode() + varName + " = " + E->cppCode() + " ;\n";
			}
			else {
				return TE->cppCode() + " " + varName + " = " + E->cppCode() + " ;\n";
			}
		}
	private:
		string varName;
		TypeExpr* TE;
		Expr* E;
};

/*!This is a subclass of OptionalExprList, which only contains a list of
 * expressions */
class listOptionalExprList: public OptionalExprList {
	/*! This constructor takes in a ExprList
	 * 	The unparse method returns the production
	 * 	\param EL a list of expression */
	public:
		listOptionalExprList(ExprList* _EL): EL(_EL){}
		std::string unparse();
		std::string cppCode(int i) {
		
			if(EL != NULL){
				return EL->cppCode(i);
			}
			else{
				return "";
			}
		}
	private:
		ExprList* EL;
		
};

/*!This is a subclass of TypeExprList, which when there is a list of 
 * expression being parsed */
class multiTypeExprList: public TypeExprList {
	/*! This constructor takes in a TypeExpr and TypeExprList
	 * 	The unparse method returns the production
	 * 	\param TE an expression type
	 * 	\param TEL a list of expressions*/
	public:
		multiTypeExprList(TypeExpr* _TE, TypeExprList* _TEL): TE(_TE), TEL(_TEL){}
		std::string unparse();
		std::string cppCode() {return TE->cppCode() + ", " + TEL->cppCode();}
	private:
		TypeExpr* TE;
		TypeExprList* TEL;
};

/*!This is a subclass of TypeExprList, which when there is only one
 * expression being parsed */
class oneTypeExprList: public TypeExprList {
	/*! This constructor takes in a TypeExpr
	 * 	The unparse method returns the production
	 * 	\param TE the type of expression */
	public:
		oneTypeExprList(TypeExpr* _TE): TE(_TE){}
		std::string unparse();
		std::string cppCode() {return TE->cppCode();}
	private:
		TypeExpr* TE;
};

/*!This is a subclass of TypeExpr, which when there is more than one 
 * TypeExpr being parsed */
class listTypeExpr: public TypeExpr {
	/*! This constructor takes in a TypeExprList
	 * 	The unparse method returns the production
	 * 	\param TEL a list of expression types*/
	public:
		listTypeExpr( TypeExprList* _TEL) : TEL(_TEL){}
		std::string unparse();
	private:
		TypeExprList* TEL;
};

/*!This is a subclass of TypeExpr, which only contains a variable that
 * holds a constant string, such as types */
class ConstTypeExpr: public TypeExpr {
	/*! This constructor takes in a string variable "type"
	 * 	The unparse method returns the production
	 * 	\param type a string variable to hold a lexeme */
	public:
		ConstTypeExpr( string _type) : type(_type){}
		std::string unparse();
		std::string cppCode() {
			if(type == "String") {return "string";}
			else if( type == "Integer") {return "int";}
			else if(type == "Float") {return "double";}
			else return "bool";
		}
	private:
		string type;
};

/*!This is a subclass of TypeBindingList, which when there are more than 
 * one binding type to be parsed */
class multiTypeBindingList: public TypeBindingList {
	/*! This constructor takes in a TypeBinding and TypeBindingList
	 * 	The unparse method returns the production
	 * 	\param TB a binding type
	 * 	\param TBL a list of binding types*/
	public:
		multiTypeBindingList(TypeBinding* _TB, TypeBindingList* _TBL): TB(_TB), TBL(_TBL) {}
		std::string unparse();
		std::string cppCode() {return TB->cppCode() + ", " + TBL->cppCode();}
	private:
		TypeBinding* TB;
		TypeBindingList* TBL;
};

/*!This is a subclass of TypeBindingList, which when there is only one 
 * binding type needs to be parsed */
class oneTypeBindingList: public TypeBindingList {
	/*! This constructor takes in a TypeBinding
	 * 	The unparse method returns the production
	 * 	\param TB a binding type */
	public:
		oneTypeBindingList(TypeBinding* _TB): TB(_TB){}
		std::string unparse();
		std::string cppCode() {return TB->cppCode();}
	private:
		TypeBinding* TB;
};

/*!This is a subclass of TypeBinding, which binds a type to a variable */
class TypedeclTypeBinding: public TypeBinding {
	/*! This constructor takes in a variableName and TypeExpr
	 * 	The unparse method returns the production
	 * 	\param varName a variable name
	 * 	\param TE an expression type */
	public:
		TypedeclTypeBinding(string _varName, TypeExpr* _TE): varName(_varName), TE(_TE) {}
		std::string unparse();
		std::string cppCode(){return TE->cppCode() + " " + varName;}
	private:
		string varName;
		TypeExpr* TE;
};

/*!This is a subclass of ExprList, which only has one Expr to parse */
class oneExprList: public ExprList {
	/*! This constructor takes in an Expr
	 * 	The unparse method returns the production
	 * 	\param E an expression */
	public:
		oneExprList(Expr* _E): E(_E){}
		std::string unparse();
		std::string cppCode(int i) {
		if(i==1){
			return "temp.push_back(" + E->cppCode() + ");\n";
		}
		else{
			return E->cppCode();
		}
			}
	private:
		Expr* E;
};

/*!This is a subclass of ExprList, which when there are more than one 
 * expression to parse */
class multiExprList: public ExprList {
	/*! This constructor takes in a Expr and ExprList
	 * 	The unparse method returns the production
	 * 	\param E an expression
	 * 	\param EL a list of expressions */
	public:
		multiExprList(Expr* _E, ExprList* _EL): E(_E), EL(_EL){}
		std::string unparse();
		std::string cppCode(int i) {
			if(i == 1){
				return "temp.push_back(" + E->cppCode() + ");\n" + EL->cppCode(i);
			}
			else{
				return E->cppCode() + ", " + EL->cppCode(i);
			}
		}
	private:
		Expr* E;
		ExprList* EL;
};

/*!This is a subclass of Expr, which creates a range of two other expressions */
class rangeExpr: public Expr {
	/*! This constructor takes in two Exp
	 * 	The unparse method returns the production
	 * 	\param E1 an expression
	 * 	\param E2 another expression */
	public:
		rangeExpr(Expr* _E1, Expr* _E2): E1(_E1), E2(_E2){}
		std::string unparse();

		std::string cppCode() {return "({ MyList<int> temp(" + E1->cppCode() + ", " + E2->cppCode() + "); temp; })";}
	private:
		Expr* E1;
		Expr* E2;
};

/*!This is a subclass of Expr, which when an expression is an if statement */
class ifExpr: public Expr {
	/*! This constructor takes in three Expr
	 * 	The unparse method returns the production
	 * 	\param E1 an expression
	 * 	\param E2 a second expression
	 * 	\param E3 the third expression */
	public:
		ifExpr(Expr* _E1, Expr* _E2, Expr* _E3): E1(_E1), E2(_E2), E3(_E3){}
		std::string unparse();
		std::string cppCode(){return "( " + E1->cppCode() + " ? " + E2->cppCode() + " : " + E3->cppCode() + " )";}
	private:
		Expr* E1;
		Expr* E2;
		Expr* E3;
};

/*!This is a subclass of Expr, which when an expression is an let statement */
class LetExpr: public Expr {
	/*! This constructor takes in a DeclList and Expr
	 * 	The unparse method returns the production
	 * 	\param DL a list of declarations
	 * 	\param E an expression */
	public:
		LetExpr(DeclList* _DL, Expr* _E): DL(_DL), E(_E){}
		std::string unparse();
		std::string cppCode() {return "({" + DL->cppCode() + E->cppCode() + "; })";}
	private:
		DeclList* DL;
		Expr* E;
};
/*! This is a subclass of Expr for the func structure */
class funcExpr: public Expr {
	/*! This constructor takes in a string and an OptionalExprList
	 * 	The unparse method returns the production
	 * 	\param varName is a string
	 * 	\param OEL is an optional expression list*/
	public:
		funcExpr(string _varName, OptionalExprList* _OEL): varName(_varName), OEL(_OEL){}
		std::string unparse();
		std::string cppCode() {return "( " + varName + " (" + OEL->cppCode(0) + " ) )";}
	private:
		string varName;
		OptionalExprList* OEL;
};
/*! This is a subclass of Expr for the single arguement structure */
class oneArgExpr: public Expr {
	/*! This constructor takes in a string and a Expr
	 * 	The unparse method returns the production
	 * 	\param kwd is a string
	 * 	\param E is an expression*/
	public:
		oneArgExpr(string _kwd, Expr* _E): kwd(_kwd), E(_E){}
		std::string unparse();
		std::string cppCode() {
			if(kwd == "print") {
				return "({ cout << " + E->cppCode() + " << endl ; cout.good() ? 0 : 1 ; })";
			}
			else if(kwd == "null"){
				return "(" + E->cppCode() + ".null())";
			}
			else if(kwd == "head"){
				return "(" + E->cppCode() + ".head())";
			}
			else if(kwd == "tail"){
				return "(" + E->cppCode() + ".tail())";
			}
		}
	private:
		string kwd;
		Expr* E;
};
/*! This is a subclass of Expr for the two arguments structure */
class twoArgExpr: public Expr {
	/*! This constructor takes in a string and two Expr
	 * 	The unparse method returns the production
	 * 	\param kwd is a string
	 * 	\param E1 is an expression
	 * 	\param E2 is an expression*/
	public:
		twoArgExpr(string _kwd, Expr* _E1, Expr* _E2): kwd(_kwd), E1(_E1), E2(_E2){}
		std::string unparse();
		std::string cppCode(){return "(" + E2->cppCode() + "." + kwd + "(" + E1->cppCode() + "))" ;}
	private:
		string kwd;
		Expr* E1;
		Expr* E2;
};
/*! This is a subclass of Expr for the lambda structure */
class lambdaExpr: public Expr {
	/*! This constructor takes in a TypeBindingList and a Expr
	 * 	The unparse method returns the production
	 * 	\param TBL is a list of typebindings
	 * 	\param E is an expression*/
	public:
		lambdaExpr(TypeBindingList* _TBL, Expr* _E): TBL(_TBL), E(_E){}
		std::string unparse();
	private:
		TypeBindingList* TBL;
		Expr* E;
};
/*! This is a subclass of Expr for the bracket list expression */
class longExpr: public Expr {
	/*! This constructor takes in a TypeExpr and a OptionalExprList
	 * 	The unparse method returns the production
	 * 	\param TE is an expression type
	 * 	\param OEL is an optional expression list*/
	public:
		longExpr(TypeExpr* _TE, OptionalExprList* _OEL): TE(_TE), OEL(_OEL){}
		std::string unparse();
		std::string cppCode(){return "({ MyList<" + TE->cppCode() + "> temp; " + OEL->cppCode(1) + "temp; })";}
	private:
		TypeExpr* TE;
		OptionalExprList* OEL;
};
/*! This is a subclass of Expr for the read structure */
class readExpr: public Expr {
	/*! This constructor takes in a string, a TypeExpr, and an Expr
	 * 	The unparse method returns the production
	 * 	\param varName is a string
	 * 	\param TE is an expression type
	 *  \param E is an expression*/
	public:
		readExpr(string _varName, TypeExpr* _TE, Expr* _E): varName(_varName), TE(_TE), E(_E){}
		std::string unparse();
		std::string cppCode() {
			if(varName == "read") {
				return "({ " +TE->cppCode() + " temp ;  ifstream inStream(" + E->cppCode() + ") ; inStream >> temp ; temp ; })";
			}
		}
	private:
		string varName;
		TypeExpr* TE;
		Expr* E;
};
/*! This is a subclass of Expr for the fold structure */
class foldExpr : public Expr{
	/*! This constructor takes in string, and a three Expr
	 * 	The unparse method returns the production
	 * 	\param kwd is a string
	 * 	\param E1 is an expression
	 *  \param E2 is another expression
	 * 	\param E3 a third expression */
	public:
		foldExpr(string _kwd, Expr* _E1, Expr* _E2, Expr* _E3) : kwd(_kwd), E1(_E1), E2(_E2), E3(_E3){}
		std::string unparse();
		std::string cppCode() {
			return "( " + E3->cppCode() + ".fold(" + E1->cppCode() + ", " + E2->cppCode() + ") )";
		}
	private:
		string kwd;
		Expr* E1;
		Expr* E2;
		Expr* E3;
};
/*! This is a subclass of Expr which is for when we have a single Expr*/
class singleExpr : public Expr{
	/*! This constructor takes in a single Expr
	 * 	The unparse method returns the production
	 * 	\param E1 is an expression */
	public:
		singleExpr(Expr* _E1) : E1(_E1){}
		std::string unparse();
		std::string cppCode(){return "(" + E1->cppCode() + ")";}
	private:
		Expr* E1;
};
/*!This is a subclass of Expr which is for when we have multiple Expr*/
class multipleExpr : public Expr{
	/*! This constructor takes in a Expr and a ExprList
	 * 	The unparse method returns the production
	 * 	\param E1 is an expression
	 * 	\param EL is an expression list*/
	public:
		multipleExpr(Expr* _E1, ExprList* _EL) : E1(_E1), EL(_EL){}
		std::string unparse();
	private:
		Expr* E1;
		ExprList* EL;
}; 
/*!This is a subclass of Expr which is for expressions with a binary operation*/ 
class BinOpExpr : public Expr {
	/*! This constructor takes in a left Expr and a right Expr
	 * 	The unparse method returns the production
	 * 	\param left is the left expression(IntConst)
	 * 	\param right is the right expression(IntConst)*/
	public:
		~BinOpExpr(){delete left; delete right; }
		BinOpExpr( Expr* _left, Expr* _right, terminal_t _op) : left(_left), right(_right), op(_op) {}
		std::string unparse() ;
		std::string cppCode() {
			/*! In this unparse, what we return base on the terminal type 
			 * we got from our parse method.*/
			switch(op) {
				case dash:
					return "(" + left->cppCode() + " - " + right->cppCode() + ")";
				case plusSign: 
					return "(" + left->cppCode() + " + " + right->cppCode() + ")";
				case star:
					return "(" + left->cppCode() + " * " + right->cppCode() + ")";
				case forwardSlash:
					return "(" + left->cppCode() + " / " + right->cppCode() + ")";
				case percent:
					return "(" + left->cppCode() + " % " + right->cppCode() + ")";
				case colon:
					return "(" + right->cppCode() + ".cons" + left->cppCode() + ")";
				case plusPlus:
					return "(" + left->cppCode() + " ++ " + right->cppCode() + ")";
				case equalsEquals:
					return "(" + left->cppCode() + " == " + right->cppCode() + ")";
				case leftAngle:
					return "(" + left->cppCode() + " < " + right->cppCode() + ")";
				case rightAngle:
					return "(" + left->cppCode() + " > " + right->cppCode() + ")";
				case greaterThanEquals:
					return "(" + left->cppCode() + " >= " + right->cppCode() + ")";
				case lessThanEquals: 
					return "(" + left->cppCode() + " <= " + right->cppCode() + ")";
				case notEquals:
					return "(" + left->cppCode() + " != " + right->cppCode() + ")";
				default:
					return "";
			}
		};
	private:
		Expr* left;
		Expr* right;
		terminal_t op;
};

#endif // Node_H








#include <iostream>
#include <fstream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <string.h>

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token{
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,
	tok_int = -4,
	tok_void = -5,
	tok_IF = -6,
	tok_ELSE = -7,
	tok_WHILE = -8,
	tok_RETURN = -9,
	tok_identifier = -10,
	tok_number = -11,
};

static std::string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;              // Filled in if tok_number
/// gettok - Return the next token from standard input.
static int gettok()
{
	static int LastChar = ' ';
	// skip any white space
	while (isspace(LastChar))
		LastChar = getchar();
	
	if(isalpha(LastChar))
	{
		while (isalnum((LastChar = getchar())))
			IdentifierStr += LastChar;
		
		if (IdentifierStr == "def")
			return tok_def;
		if (IdentifierStr == "extern")
			return tok_extern;
		if (IdentifierStr == "int")
			return tok_int;
		if (IdentifierStr == "void")
			return tok_void;
		return tok_identifier;
	}
	
	if(isdigit(LastChar) || LastChar == '.')
	{
		std::string NumStr;
		do{
			NumStr += LastChar;
			LastChar = getchar();
		}	while(isdigit(LastChar) || LastChar == '.');
		//NumVal = strtod(NumStr.c_str(), 0);
		return tok_number;
	}
	
	if (LastChar == '/' && getchar() == '/'){

		std::fstream commentsfile;
		commentsfile.open("comments.txt");
		do {
			LastChar = getchar();
			commentsfile << LastChar;
			
		}while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
		if (LastChar != EOF)
			return gettok();
	}
	
	if (LastChar == '/' && getchar() == '*'){
		
		std::fstream commentsfile;
		commentsfile.open("comments.txt");
		do {
			LastChar = getchar();
			commentsfile << LastChar;
		}while (LastChar == EOF || (LastChar == '*' && getchar() == '/'));
		LastChar = getchar();
		if (LastChar != EOF)
			return gettok();
	}
	
	if (LastChar == EOF)
		return tok_eof;
	
	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;
	
} 

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

// StartAST - Base class for all nodes
class StartAST{
	public:
		virtual ~StartAST() {}
};

class TypeAST: public StartAST{
	std::string Type;
	public:
		TypeAST(std::string &type): Type(type){ }
};

class IdentifierAST: public StartAST{
	std::string Name;
	public:
		IdentifierAST(const std::string &name): Name(name) {	}
};

class ParameterAST : public StartAST{
	public:
		ParameterAST(TypeAST *type, IdentifierAST *Identifier) {}
};

class FunctionDefinitionAST : public StartAST{
	std::vector<ParameterAST*> Parameters;
	public:
		FunctionDefinitionAST(TypeAST *Type, IdentifierAST *Identifier, 
		const std::vector<ParameterAST> &Paramters) {	}
};


class ConstantAST: public StartAST{
	public:
		ConstantAST(double value) {	}
};



class ArgumentAST: public StartAST{
	public:
		ArgumentAST(IdentifierAST *Identifier){	}
};

class FunctionCallAST: public StartAST{
	std::vector<ArgumentAST*> Arguments;
	public:
		FunctionCallAST(IdentifierAST *Identifier, 
		const std::vector<ArgumentAST> &Arguments){	}
};

class StatementAST: public StartAST{

};

class BodyAST: public StartAST{
	std::vector<StatementAST*> Statements;
	public:
		BodyAST(const std::vector<StatementAST*> &Statements){	}
};

class ExpressionAST: public StartAST{
	
};

class BinaryExpressionAST: public ExpressionAST{
	public:
		BinaryExpressionAST(char op, ExpressionAST *lhs, ExpressionAST *rhs){}
};

class UnaryExpressionAST: public ExpressionAST{
	public:
		UnaryExpressionAST(char op, ExpressionAST *lhs){	}
};

class ReturnAST: public StatementAST{
	public:
		ReturnAST(ExpressionAST *last){	}
};
class ReturnStatementAST: public StatementAST{
	std::string Return;
	public:
	ReturnStatementAST(const std::string Return, ReturnAST *returna){ }
};


class ElseStatementAST: public StatementAST{
	std::string Else;
	public:
		ElseStatementAST(const std::string Else, BodyAST *body){ }
};
class IfStatementAST: public StatementAST{
	std::string If;
	public:
		IfStatementAST(const std::string If, ExpressionAST *condition, 
		BodyAST *Body, ElseStatementAST *Elsestmt) { }
};

class WhileStatementAST: public StatementAST{
	std::string While;
	public:
		WhileStatementAST(const std::string While, 
		ExpressionAST *condition, BodyAST *Body){	}
};

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

static int CurTok,newCurTok;
static int getNextToken() {
  return CurTok = gettok();
}

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;
  
  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0) return -1;
  return TokPrec;
}

/// Error* - These are little helper functions for error handling.
StartAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
StatementAST *Errors(const char *Str) { fprintf(stderr, "Error: %s\n", Str);
					return 0;}
// BinaryOP
//	: '='
//	| '+'
//	| '-'
//	| '*'
//	| '/'
//	| '%'
//	| '=='
//	| '+='
//	| '-='
//	| '*='
//	| '/='
//	| '%='
//	| '<='
//	| '<'
//	| '>'
//	| '>='
bool BinaryOP()
{
	newCurTok = getNextToken();
	if (newCurTok ==  '=' || '+' || '-' || '*' || '/' || '%' || "==" || "+="
			  "-=" || "*=" || "/=" || "%=" || "<=" || "<" || ">" ||
			  ">=")
		return true;
	else return false;
}

// UnaryOP
//	| '++'
//	| '--'
bool UnaryOP()
{
	newCurTok = getNextToken();
	if (newCurTok == "++" || newCurTok == "--")
		return true;
	else return false;
}
static StartAST *ParseStart();
static StatementAST *ParseIfStatement();
static StatementAST *ParseReturn();


static StartAST *ParseIdentifier()
{
	std::string Name = IdentifierStr;
	getNextToken();  // eat identifier.
	return new IdentifierAST(Name);
}


// Type
//	: INT
//	| VOID
static StartAST *ParseType()
{
	if (CurTok == tok_int)
	{
		std::string Type = "int";
		return new TypeAST(Type);
	}
	else
	{
		std::string Type = "Void";
		return new TypeAST(Type);
	}
	
}

// Parameters
//	: 
//	| Parameter Parameters

// Parameter
//	: Type Identifier
	
static StartAST *ParseParameter()
{
	StartAST *type = ParseType();
	StartAST *Identifier = ParseIdentifier();
	return new ParameterAST(*type, *Identifier);
}

static StatementAST *ParseVariableDeclaration()
{
	StartAST *type = ParseType();
	StartAST *Identifier = ParseIdentifier();
	return new ParameterAST(type, Identifier);
}	


// Functiondefinition
// 	: Type Identifier '(' parameters ')'
static StartAST *ParseFunctiondefinition()
{
	StartAST *Type = ParseType();
	StartAST *Parameters;
	 StartAST *Identifier = ParseIdentifier();
	if (CurTok == '(' ) getNextToken();
	if (CurTok != ')')
	{
		while(1)
		{
			Parameters = ParseParameter();
			if (!Parameters) return 0;
			Parameters->push_back (Parameters);
			
			if (CurTok == ')') break;
			if (CurTok == ',') 
				return Error( "Expected ')' or ',' in Parameters");
			getNextToken();
		}
	}
	getNextToken();
	return new FunctionDefinitionAST (Type, Identifier, Parameters);
}




// ReturnStatement
//	: RETURN ';'
//	| RETURN Identifier ';'
//	| RETURN Constant ';'
static StatementAST *ParseReturnStatement()
{
	getNextToken();
	StatementAST *returna = ParseReturn();
	return new ReturnStatementAST("return" , returna);
}
static StatementAST *ParseReturn()
{
	switch (CurTok)
	{
		case ';' : *last = NULL;
		case tok_identifier : *last = ParseIdentifier();
		case tok_number : *last = ParseConstant();
		default : return Error("Unexpected");
	}
	return new ReturnAST(last);
}

// Body
//	: '{' Statements '}'
static StartAST *ParseStatement()
{
	switch(CurTok)
	{
		case tok_int : ParseVariableDeclaration();
		case tok_void :
			return Error( "'void' should be replace with 'int'" );
		case tok_identifier : StartAST *Statement = ParseIdentifier();
		case tok_IF : StartAST *Statement = ParseIfStatement();
		case tok_WHILE : StartAST *Statement = ParseWhileStatement();
		case tok_RETURN : StartAST *Statement = ParseReturnStatement();
		default : StartAST *Statement = ParseExpression();
	}
	return StatementAST(Statement);
}

static StartAST *ParseBody()
{
	if (CurTok != '{') return Error (" Expected '{' in body");
	getNextToken();
	std::vector<StatementAST*> Statements;
	if (CurTok != '}')
	{
		while (1)
		{
			if (CurTok != tok_IF || tok_WHILE || tok_RETURN)
			{
				StatementAST *Statement = ParseStatement();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				if (CurTok == '}') break;
			}
			else
			{
				StatementAST *Statement = ParseStatement();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				if (CurTok == '}') break;
				if (CurTok == ';')
					return Error(" Expected ';' in body");
				getNextToken();
			}
		}
	}
	getNextToken();
	return new BodyAST(Statements);
}

// VariableDeclaration
//	: Type Identifier ';'	


// FunctionCall
//	: Identifier '(' Identifier ')' ';'
static StatementAST *ParseFunctionCall()
{
	StartAST *Identifier = ParseIdentifier();
	std::vector<ParseArgumentAST> Arguments;
	if (CurTok == '(' ) getNextToken();
	if (CurTok != ')')
	{
		while(1)
		{
			IdentifierAST *Argument = ParseIdentifier();
			if (!Identifier) return 0;
			Arguments.push_back (Identifier);
			
			if (CurTok == ')') break;
			if (CurTok == ',') 
				return Errors("Expected ')' or ',' in Arguments");
			getNextToken();
		}
	}
	getNextToken();
	return new FunctionCallAST(Type, Arguments);
}
// Expression
//	: Expression BinaryOP Expression
//	| Expression UnaryOP
//	| Identifier

//	| Constant
static StartAST *ParseExpression()
{
	switch (getNextToken())
	{
		case BinaryOP() : ParseBinaryExpression();

		case UnaryOP(): ParseUnaryExpression();
		default :
			{

				switch (CurTok)
				{
					case tok_identifier : ParseIdentifier();
					case tok_number : ParseConstant();
				}
			}
	}
}

// ElseStatement
//	: Else Body

static StatementAST *ParseElseStatement()
{
	getNextToken();
	StartAST *body = ParseBody();
	return new ElseStatementAST("else", body);

}

// IfStatement
//	: IF '(' Expression ')' Body
//	| IF '(' Expression ')' Body ElseStatement
static StatementAST *ParseIfStatement()
{
	getNextToken();
	if(CurTok != '(') return Errors(" Expected '(' in If Statement ");
	getNextToken();
	StatementAST *condition = ParseExpression();
	if(CurTok != ')') return Errors(" Expected ')' in If Statement ");
	getNextToken();
	StartAST *Body = ParseBody();
	if (getNextToken() != tok_ELSE) StatementAST ElseStmt = NULL;
	else Statement	AST *ElseStmt = ParseElseStatement();
	return new IfStatementAST("if", condition, Body, Elsestmt);
}



// WhileStatement
//	: WHILE '(' Condition ')' Body
static StatementAST *ParseWhileStatement()
{
	getNextToken();
	if(CurTok != '(') return Error(" Expected '(' in If Statement ");
	getNextToken();
	StatementAST *condition = ParseExpression();
	if(CurTok != ')') return Error(" Expected ')' in If Statement ");
	getNextToken();
	StartAST *Body = ParseBody();
	return new WhileStatementAST("while" , condition, body);
}




static ExpressionAST *ParseBinOpRHS(int ExprPrec, ExpressionAST *LHS) {
  // If this is a binop, find its precedence.
  while (1) {
    int TokPrec = GetTokPrecedence();
    
    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;
    
    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken();  // eat binop
    
    // Parse the primary expression after the binary operator.
    ExpressionAST *RHS = ParseExpression();
    if (!RHS) return 0;
    
    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec+1, RHS);
      if (RHS == 0) return 0;
    }
    
    // Merge LHS/RHS.
    LHS = new BinaryExpressionAST( op , lhs, rhs);
  }
}
static ExpressionAST *ParseBinaryExpression() {
  ExpressionAST *LHS = ParsePrimary();
  if (!LHS) return 0;
  
  return ParseBinOpRHS(0, LHS);
}

static ExpressionAST *ParseUnaryExpression(){
	ExpressionAST *LHS = ParseExpression();
	if(!LHS) reurn 0;
	return new UnaryExpressionAST(op , lhs);
}


// Statements
//	:
//	| Statement Statements

// Statement
//	:
//	| VariableDeclaraion
//	| FunctionCall
//	| IfStatement
//	| WhileStatement
//	| Expression
//	| ReturnStatement





//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence[tok_less] = 10;
  BinopPrecedence[tok_lessequal] = 10;
  BinopPrecedence[tok_greater] = 10;
  BinopPrecedence[tok_greaterequal] = 10;
  
  BinopPrecedence[tok_plus] = 20;
  BinopPrecedence[tok_minus] = 20;
  BinopPrecedence[tok_minusequal] = 20;
  BinopPrecedence[tok_plusequal] = 20;
  
  BinopPrecedence[tok_star] = 40;
  BinopPrecedence[tok_slash] = 40;
  BinopPrecedence[tok_percent] = 40;
  BinopPrecedence[tok_starequal] = 40;
  BinopPrecedence[tok_percentequal] = 40;
  
  BinopPrecedence[tok_equal] = 50;  // highest.

  // Prime the first token.
  fprintf(stderr, "ready> ");
  getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  return 0;
}

static void mainloop(){
	while (1)
	{
		fprintf(stderr, "ready> ");
    switch (CurTok) 
	{
    	case tok_eof:    return;
    	case ';':        getNextToken(); break;  // ignore top-level semicolons.
    	case '{':    ParseBody(); break;
    	case tok_int:
		case tok_void:
		 ParseStartAST(); break;
    	default:         ParseStatement(); break;
    }

	}
}

#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <string>
//#include "C:\Users\arpit\Desktop\getch_ungetch.cpp"
#include "candacsllexer.cpp"
/*// static void MainLoop();
//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_int = -2, tok_void = -3, tok_while = -4, tok_if = -5, tok_else = -6,
  tok_return = -7,

  // primary
  tok_identifier = -8, tok_number = -9,
  
  tok_plusplus = -10, tok_plusequal = -11, tok_minusminus = -12, 
  tok_minusequal = -13, tok_equalequal = -14, tok_greaterequal = -15,
  tok_lessequal = -16, tok_starequal = -17, tok_percentequal = -18,
  tok_slashequal = -19, tok_slash = -20, tok_ensures = -21, tok_requires = -22,
  tok_assigns = -23, tok_result = -24, tok_new = -25, tok_valid = -26,
  tok_old = -27, tok_exclaim = -28, tok_at = -29, tok_hash = -30, 
  tok_dollar = -31, tok_up = -32, tok_and = -33, tok_openparen = -34,
  tok_closeparen = -35, tok_opencurly = -36, tok_closecurly = -37,
  tok_openbrackect = -38, tok_closebrackect = -39, tok_colon = -40,
  tok_semicolon = -41, tok_comma = -42, tok_dot = -43, tok_question = -44,
  tok_underscore = -45
};
int flag_comment = 0; //to be 1 if lexer is inside comment
int flag_atTheRate = 0; //to be 1 if lexer has read 1 @ character
int flag_keyword = 0;
static std::string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;              // Filled in if tok_number

/// gettok - printf(" the next token from standard input.
static int gettok() {
  static int LastChar;
  static int newchar = ' ';
  int temp;
  printf("%c \n", LastChar);
  //getch();
  LastChar = getch();
  

   // Skip any whitespace.
 while (isspace(LastChar))
   LastChar = getch();
   
  temp = getch();
  printf("before any if");

  if(LastChar == '/' && temp == '*')
  {
  	flag_comment = 1;
  	//LastChar = getch();
  	return gettok();
  }
  printf("%d %d\n",LastChar, temp);
  if (LastChar == '*' && temp == '/')
  {
  	printf("i am here");
	 flag_comment = 0;
  	//LastChar = getch();
		gettok();
  }
  else
  {
  	
  	ungetch(temp);
	//printf("%c \n" , LastChar);
	temp = ' '; 	
  }
  
while (isspace(LastChar))
   LastChar = getch();
     
  if (flag_comment == 1) 
  {
  	//temp = getch();
  	if (LastChar == '@' ){
  		LastChar = getch();
		flag_atTheRate = 1;
  	}
	  else if (LastChar == ';') {
	  	LastChar = getch();
	  	flag_atTheRate = 0;
	  	flag_keyword = 0;
  	}//else ungetch(temp);
  	
  }
  

  // Skip any whitespace.
 while (isspace(LastChar))
   LastChar = getch();

  
if ((flag_comment == 0) || (flag_comment == 1 && flag_atTheRate == 1)) 
{
	printf("above switch %c \n" , LastChar);
 switch(LastChar)
 {
 	case '+' :  newchar = getch();
	 			if (newchar == '+')
 				{
 					LastChar = getch();
 					return tok_plusplus;
 				}
 				else if (newchar == '=')
 				{
 					LastChar = getch();
 					return tok_plusequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
				 
				 break;
 	case '-' : newchar = getch();if(newchar == '-')
 				{
 					LastChar = getch();
 					return tok_minusminus;
 				}
 				else if (newchar == '=')
 				{
 					LastChar = getch();
 					return tok_minusequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '=' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_equalequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '>' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_greaterequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '<' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_lessequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '*' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_starequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '%' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_percentequal;
 				}
 				else
 				{
 					int ch;
 					ch = LastChar;
 					ungetch(newchar);
 					LastChar = getch();
 					return ch;
 				}
 				break;
 	case '/' : newchar = getch();if(newchar == '=')
 				{
 					LastChar = getch();
 					return tok_slashequal;
 				}
 				else
 				{
 					ungetch(newchar);
 					int ch;
 					ch = LastChar;
 					LastChar = getch();
 					return ch;
 				}
 	
 	default :   if (isalpha(LastChar)) 
	 			{ // identifier: [a-zA-Z][a-zA-Z0-9]*
    				IdentifierStr = LastChar;
    				//printf("%c\n", LastChar);
    				getch();
    				//printf("%c \n" , LastChar);
    				while (isalnum(LastChar = getch()))
      					{
      						IdentifierStr += LastChar;
      						//printf("%c\n",LastChar);
      					}
      					//printf("%s\n", &IdentifierStr);
				    if ( (flag_comment == 0)  &&  (IdentifierStr == "int")) return tok_int;
    				else if (flag_comment == 0  && IdentifierStr == "void" ) return tok_void;
  					else if (flag_comment == 0  && IdentifierStr == "while" ) return tok_while;
    				else if (flag_comment == 0  && IdentifierStr == "if") return tok_if;
    				else if (flag_comment == 0 && IdentifierStr == "else") return tok_else;
    				else if (flag_comment == 0 && IdentifierStr == "return" ) return tok_return;
    				else if (IdentifierStr == "ensures" && flag_atTheRate == 1 && flag_keyword == 0){
    					flag_keyword = 1;
						return tok_ensures;
							
    				}
					else if(IdentifierStr == "requires" && flag_atTheRate == 1 && flag_keyword == 0)
					{
						flag_keyword = 1;
						return tok_requires;
					} 
    				else 
					{
						//printf("%d\n", flag_comment);
						return tok_identifier;
					}
  				}

				else if (isdigit(LastChar) || LastChar == '.') 
				{   // Number: [0-9.]+
    				std::string NumStr;
    				do {
      					NumStr += LastChar;
      					LastChar = getch();
    				} while (isdigit(LastChar) || LastChar == '.');

    				NumVal = strtod(NumStr.c_str(), 0);
    				return tok_number;
				}

  				else if (LastChar == '#') {
    				// Comment until end of line.
    				do LastChar = getch();
    				while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    
  					  if (LastChar != EOF)
      					return gettok();
  				 }
  
  					// Check for end of file.  Don't eat the EOF.
  				else if (LastChar == EOF)
				    return tok_eof;
	 
				int ThisChar = LastChar;
 				//LastChar = getch();
  				return ThisChar;
 				 				
 }
 }
 else
 {
 	do
	{
	 	LastChar = getch();
    }while (LastChar != EOF && LastChar != '@' && LastChar != '*');
    ungetch(LastChar);
    LastChar = getch();
    return gettok();
 }
 */


//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

/// ExprAST - Base class for all expression nodes.
class StartAST {
public:
  virtual ~StartAST() {}
};

class ExprAST : public StartAST{

};

class StatementAST : public StartAST{
public:
};


/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
  NumberExprAST(double val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;
public:
  VariableExprAST(const std::string &name) : Name(name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) {}
};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST {
public:
  UnaryExprAST(char op, ExprAST *lhs) {}
};


class AssignmentExprAST : public ExprAST {
public:
    AssignmentExprAST(VariableExprAST *lhs, ExprAST *rhs) { }
};


/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
public:
  CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
    : Callee(callee), Args(args) {}
};

class BodyAST : public ExprAST{
  std::vector<StatementAST*> Statements;
  std::vector<ExprAST*> Expressions;
 public:
  BodyAST(const std::vector<StatementAST*> &statements) : Statements(statements)
  {}
 /* BodyAST(const std::vector<ExprAST*> &expressions) : Expressions(expressions)
  {}*/
  
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST : public StatementAST{
  std::string Name;
  std::vector<std::string> Args;
public:
  PrototypeAST(const std::string &name, const std::vector<std::string> &args)
    : Name(name), Args(args) {}
  
};

class PreAST : public ExprAST
{
	std::string Keyword;
	public:
		PreAST(const std::string Keyword, const ExprAST *Precondition) {}
};


class PostAST : public ExprAST
{
	std::string Keyword;
	public:
		PostAST(const std::string Keyword, const ExprAST *Postcondition){}
};

class VarDeclAST : public StatementAST{
  std::vector<std::string> Vars;
public:
  VarDeclAST(const std::vector<std::string> &vars) :Vars(vars) {}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public StatementAST{
	std::vector<PreAST*> Pre;
	std::vector<PostAST*> Post;
public:
  FunctionAST(PrototypeAST *proto, std::vector<PreAST*> &pre, BodyAST *body, std::vector<PostAST*> &post)
  : Pre(pre), Post(post) {}
};

class FunctionCallAST : public StatementAST{
  std::string Name;
  std::vector<std::string> Args;
public:
  FunctionCallAST(const std::string &name, const std::vector<std::string> &args)
    : Name(name), Args(args) {}
  
};

class PrintAST : public StatementAST{
	std::string PrintStr;
	public:
		PrintAST(const std::string &printstr):PrintStr(printstr){}
};

class ExprStatementAST : public StatementAST{
	public:
		ExprStatementAST(ExprAST *Expr){}
};

class ReturnStatementAST : public StatementAST{
  std::string Name;
 public:
  ReturnStatementAST(const std::string Return) {}
  ReturnStatementAST(const std::string Return, const std::string &name)
  	: Name(name) {}
  ReturnStatementAST(const std::string Return, double NumVal) {}
 
 };
 
class IfStatementAST : public StatementAST{
  std::string If;
  std::string Else;
public:
  IfStatementAST(const std::string If, ExprAST *cond, BodyAST *body) {}
  IfStatementAST(const std::string If, ExprAST *cond, BodyAST *body,
  	const std::string Else, BodyAST *newbody) {}
};  
 
class WhileStatementAST : public StatementAST{
  std::string While;
public:
  WhileStatementAST(const std::string While,ExprAST *cond, BodyAST *body)
  {}
};


//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
#include "getNextToken_ungetTok.cpp"
/*static int getNextToken() 
{
  return CurTok = gettok();
}*/

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

static PrototypeAST *ParsePrototype();
/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() 
{
  if (!isascii(CurTok) && ( CurTok != tok_equalequal && CurTok != tok_greaterequal
   && CurTok != tok_lessequal && CurTok != tok_plusplus && CurTok != tok_lessequal && CurTok != tok_notequal ))
    return -1;
  
  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0) 
  return -1;
  return TokPrec;
}


/// Error* - These are little helper functions for error handling.
ExprAST *Error(const char *Str) 
{ 
fprintf(stderr, "Error: %s\n", Str);
return 0;
}
PrototypeAST *ErrorP(const char *Str) 
{ 
Error(Str); 
return 0; 
}
FunctionAST *ErrorF(const char *Str) 
{ 
Error(Str); 
return 0; 
}
BodyAST *ErrorB(const char *Str) 
{ 
Error(Str); 
return 0; 
}
StatementAST *ErrorS(const char *Str) 
{ 
Error(Str); 
return 0; 
}

static ExprAST *ParseExpression();
static BodyAST *ParseBody();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() 
{
	fprintf(stderr, "Entering in function ParseIdentifierExpr"
	"with current token %d\n", CurTok);
  std::string IdName = IdentifierStr;
  printf("before entering curtok is : %d\n", CurTok);
  getNextToken();  // eat identifier.
  //getNextToken();
  printf("before entering curtok is : %d\n", CurTok);
    
  if (CurTok != '(') // Simple variable ref.
    {
     fprintf(stderr, "Returning from function ParseIdentifierExpr"
	 "with current token %d\n", CurTok);
     return new VariableExprAST(IdName);
  }
  // Call.
  getNextToken();  // eat (
  //printf("\ncurrent token : %d\n",CurTok);
  std::vector<ExprAST*> Args;
  if (CurTok != ')') 
  {
  //printf("its in if of identifierexpr");
    while (1) 
	{
      ExprAST *Arg = ParseExpression();
      if (!Arg) return 0;
      Args.push_back(Arg);

      if (CurTok == ')') 
	  break;

      if (CurTok != ',')
        return Error("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  //getNextToken();
  fprintf(stderr, "Returning from function ParseIdentifierExpr"
  "with current token %d\n", CurTok);
  return new CallExprAST(IdName, Args);
  
}

/// numberexpr ::= number
static ExprAST *ParseNumberExpr() {
	fprintf(stderr, "Entering in function ParseNumberExpr"
	"with current token %d\n", CurTok);
  ExprAST *Result = new NumberExprAST(NumVal);
  getNextToken(); // consume the number
   fprintf(stderr, "Returning from function ParseNumberExpr"
   "with current token %d\n", CurTok);
   return Result;
 
}

/// parenexpr ::= '(' expression ')'
static ExprAST *ParseParenExpr() {
	fprintf(stderr, "Entering in function ParseParenExpr"
	"with current token %d\n", CurTok);
  getNextToken();  // eat (.
  ExprAST *V = ParseExpression();
  if (!V) return 0;
  
  if (CurTok != ')')
    return Error("expected ')'");
  getNextToken();  // eat ).
  fprintf(stderr, "Returning from function ParseParenExpr"
  "with current token %d\n", CurTok);
  return V;
  
}

//parse precondition



/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static ExprAST *ParsePrimary() {
	fprintf(stderr, "Entering in function ParsePrimary"
	"with current token %d\n", CurTok);
  switch (CurTok) {
  default: return Error("unknown token when expecting an expression");
  case tok_identifier: fprintf(stderr, "Returning from function ParsePrimary"
  						"with current token %d\n", CurTok); 
  						return ParseIdentifierExpr();
  case tok_number:     fprintf(stderr, "Returning from function ParsePrimary"
  						"with current token %d\n", CurTok); 
  						return ParseNumberExpr();
  case '(':            fprintf(stderr, "Returning from function ParsePrimary"
  						"with current token %d\n", CurTok);
  						return ParseParenExpr();
  }
  
}

/// binoprhs
///   ::= ('+' primary)*
static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
fprintf(stderr, "Entering in function ParseBinOpRhs"
"with current token %d\n", CurTok);
  // If this is a binop, find its precedence.
  while (1) {
    int TokPrec = GetTokPrecedence();
    
    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      {
      	fprintf(stderr, "Returning from function ParseBinaryOpRhs"
	  	"with current token %d\n", CurTok);
      	return LHS;
    }
    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken();  // eat binop
    //printf("after in binop %d", CurTok);
    // Parse the primary expression after the binary operator.
    ExprAST *RHS = ParsePrimary();
    //getNextToken();
    if (!RHS)
	{
	fprintf(stderr, "Returning from function ParseBinaryOpRhs with error"
	"and current token %d\n", CurTok); 
	return 0;
	}
    
    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec+1, RHS);
      if (RHS == 0) 
	  {
	  fprintf(stderr, "Returning from function ParseBinaryOpRhs with error"
	  "with current token %d\n", CurTok); 
	  return 0;
	  }
    }
    
    // Merge LHS/RHS.
    LHS = new BinaryExprAST(BinOp, LHS, RHS);
  }
}

/// expression
///   ::= primary binoprhs
///
static ExprAST *ParseExpression() {
fprintf(stderr, "Entering in function ParseExpression"
"with current token %d\n", CurTok);
  ExprAST *LHS = ParsePrimary();
  //int temptok = CurTok;
  //getNextToken();
  if (!LHS)
  {
  	fprintf(stderr, "Returning from function ParseExpression with error \n"); 
  	return 0;
  }
  fprintf(stderr, "Returning from function ParseExpression"
  "with current token %d\n", CurTok);
  return ParseBinOpRHS(0, LHS);
}


static ExprAST *ParseUnaryExpression(){
fprintf(stderr, "Entering in function ParseUnaryExpression"
"with current token %d\n", CurTok);
	char op = CurTok;
	ExprAST *lhs = ParseExpression();
	if(!lhs) 
	{ 
		fprintf(stderr, "Returning from function ParseUnary with error \n"); 
		return 0;
	}
	fprintf(stderr, "Returning from function ParseUnary"
	"with current token %d\n", CurTok);
	return new UnaryExprAST(op , lhs);
}

static StatementAST *ParseIfStatement()
{
	fprintf(stderr, "Entering in function ParseIf"
	"with current token %d\n", CurTok);
	std::string If = IdentifierStr;
	getNextToken();// eat if
	if(CurTok != '(') 
	return ErrorS(" Expected '(' in If Statement ");
	getNextToken();// eat '('
	ExprAST *cond = ParseExpression();
	//getNextToken();
	if(CurTok != ')') 
	return ErrorS(" Expected ')' in If Statement ");
	//if(CurTok == ')')
	getNextToken();
	printf("curtok is : %d", CurTok);
	BodyAST *body = ParseBody();
	if (CurTok != tok_else)
	{
		//std::string Else = NULL;
		//BodyAST *newBody = NULL;
		fprintf(stderr, "Returning from function ParseIf"
		"with current token %d\n", CurTok);
		return new IfStatementAST (If, cond, body);
	}
	else 
	{
		std::string Else = IdentifierStr;
		getNextToken();
		BodyAST *newBody = ParseBody();
		fprintf(stderr, "Returning from function ParseIf"
		"with current token %d\n", CurTok);
		return new IfStatementAST(If, cond, body, Else, newBody);
	}
}

// WhileStatement
//	: WHILE '(' Condition ')' Body
static StatementAST *ParseWhileStatement()
{
	fprintf(stderr, "Entering in function ParseWhile"
	"with current token %d\n", CurTok);
	std::string While = IdentifierStr;
	getNextToken();
	if(CurTok != '(') 
	return ErrorS(" Expected '(' in while Statement ");
	getNextToken();
	ExprAST *condition = ParseExpression();
	//getNextToken();
	if(CurTok != ')') 
	return ErrorS(" Expected ')' in while Statement ");
	getNextToken();
	BodyAST *body = ParseBody();
	fprintf(stderr, "Returning from function ParseWhile"
	"with current token %d\n", CurTok);
	return new WhileStatementAST(While , condition, body);
}

// ReturnStatement
//	: RETURN ';'
//	| RETURN Identifier ';'
//	| RETURN Constant ';'
static StatementAST *ParseReturnStatement()
{
	fprintf(stderr, "Entering in function ParseReturn"
	"with current token %d\n", CurTok);
	std::string Return = IdentifierStr;
	getNextToken();
	if(CurTok == ';') 
	{ 
		getNextToken();
		fprintf(stderr, "Returning from function ParseReturn"
		"with current token %d\n", CurTok); 
		return new ReturnStatementAST(Return);
	}
	if(CurTok == tok_identifier) 
	{
		std::string Name = IdentifierStr;
		getNextToken();
		if(CurTok == ';') 
		{ 
			getNextToken();
			fprintf(stderr, "Returning from function ParseReturn"
			"with current token %d\n", CurTok);
			return new ReturnStatementAST(Return, Name);
		}
		else return ErrorS(" Expected ';' in return Statement ");
	}
	if(CurTok == tok_number){
		getNextToken();
		if(CurTok == ';'){
		getNextToken();
		fprintf(stderr, "Returning from function ParseReturn"
		 "with current token %d\n", CurTok);
		return new ReturnStatementAST(Return, NumVal);}
		return ErrorS(" Expected ';' in return Statement ");
	}
	return ErrorS(" Wrong return Statement ");
}
static StatementAST *ParseVarDecl(){
fprintf(stderr, "Entering in function ParseVarDecl with current token %d\n", CurTok);
 getNextToken(); // eat int
  std::vector<std::string> VarNames;
  VarNames.push_back(IdentifierStr);
   //getNextToken();//ead id    
  while (CurTok == ',')
  {
  	  getNextToken(); // eat ','
          VarNames.push_back(IdentifierStr);
          getNextToken();
  }
          
      if(CurTok != ';') return ErrorS("Expected ';' at the in variable declaration");
    fprintf(stderr, "Returning from function ParseVarDecl with current token %d\n", CurTok);
    return new VarDeclAST(VarNames);
}

static StatementAST *ParsePrint(){
	getNextToken(); // eat printf
	int Str;
	std::string PrintStr;
	if(CurTok != '(') return ErrorS("Expected '(' at the in variable declaration");
	//getNextToken();
	//if(CurTok != '"') return ErrorS("Expected '\"' at the in variable declaration");
	//getNextToken();
	while(CurTok != ';')
	{
		
		//printf("%c",(char)CurTok);
		//Str = getch();
		//printf("%c\n",Str);
		//if( Str == ')' ) break;
		if (CurTok == tok_identifier) PrintStr += IdentifierStr;
		else PrintStr += CurTok;
		PrintStr += ' ';
		getNextToken();
		
	}
	
	//getNextToken();
	printf("from printf CurTok %c", (char)CurTok);
	return new PrintAST(PrintStr);
}


// FunctionCall
//	: Identifier '(' Identifier ')' ';'
static StatementAST *ParseFunctionCall()
  {
  fprintf(stderr, "Entering in function ParseFunctionCall with current token %d\n", CurTok);
  std::string FnName = IdentifierStr;
  getNextToken();
  printf("%d \n" , CurTok);
  if(CurTok != '(')
    return ErrorS("Expected '(' in function call");
  
  std::vector<std::string> ArgNames;
  while(getNextToken() == tok_identifier)
    {
    ArgNames.push_back(IdentifierStr);
    }
  if(CurTok != ')')
    return ErrorS("Expected ')' in function call");
  
  // success.
  getNextToken();  // eat ')'.
  fprintf(stderr, "Returning from function ParseFunctionCall with current token %d\n", CurTok);
  return new FunctionCallAST(FnName, ArgNames);
}

static StatementAST *ParseExprStatement()
{
	ExprAST *Expression;
	Expression = ParseExpression();
	return new ExprStatementAST(Expression);
}

static BodyAST *ParseBody()
{
	fprintf(stderr, "Entering in function ParseBody with current token %d\n", CurTok);
	if (CurTok != '{') return ErrorB (" Expected '{' in body");
	getNextToken();//eat '{'
	std::vector<StatementAST*> Statements;
	StatementAST *Statement;
	std::vector<ExprAST*> Expressions;
	ExprAST *Expression;
	while (CurTok != '}')
	{
		
		
			if (CurTok == tok_int)
			{
				getNextToken();//eat int
				StatementAST *Statement = ParseVarDecl();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				if (CurTok == '}') 
				{
					getNextToken();
					break;
				}
				getNextToken();
			}
			else if(CurTok == tok_void) 
				return ErrorB( "'void' should be replace" 
				"with 'int'" );
			else if(CurTok == tok_if)
			{
				StatementAST *Statement = ParseIfStatement();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				
			}
			else if(CurTok == tok_while)
			{
				StatementAST *Statement = ParseWhileStatement();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				
			}
			else if(CurTok == tok_return)
			{
				StatementAST *Statement = ParseReturnStatement();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				//if (CurTok == '}') break;
				//if (CurTok != ';')
				//	return ErrorB(" Expected ';' in body");
				//getNextToken();
			}
			else if(CurTok == tok_print)
			{
				StatementAST *Statement = ParsePrint();
				//getNextToken();
				if (!Statement) return 0;
				Statements.push_back(Statement);
				//if (CurTok == '}') break;
				if (CurTok != ';')
					return ErrorB(" Expected1 ';' in body");
				getNextToken();			
			}
			else if(CurTok == tok_identifier )
			{
				int temptok;
				if( (temptok = gettok()) == '(')
				{
					ungetTok(temptok);
					StatementAST *Statement = ParseFunctionCall();
					if (!Statement) return 0;
					Statements.push_back(Statement);
					//if (CurTok == '}') break;
					if (CurTok != ';')
					return ErrorB(" Expected2 ';' in body");
				getNextToken();
				}
			
				else
				{
					ungetTok(temptok);
					StatementAST *Statement = ParseExprStatement();
					
					/*ExprAST *Expression = ParseExpression();*/
					if (!Expression) return 0;
					Statements.push_back(Statement);
					//getNextToken();
					if (CurTok != ';')
					return ErrorB(" Expected3 ';' in body");
					getNextToken(); //eat ';'
					//if (CurTok == '}') break;
					
					
					
				}
			}
			/*fprintf(stderr, "Returning from function ParseBody with current token %d\n", CurTok);
			return new BodyAST(Expressions);*/
	
	}
	getNextToken();//eat '}'
	fprintf(stderr, "Returning from function ParseBody with curtok : %d\n", CurTok);
	return new BodyAST(Statements);
}

/// prototype
///   ::= id1 '(' id* ')'
static PrototypeAST *ParsePrototype() {
fprintf(stderr, "Entering in function ParsePrototype with curtok : %d\n", CurTok);
//printf("curtok is : %d", CurTok);
  if (CurTok != tok_identifier )
    return ErrorP("Expected function name in prototype");

  std::string FnName = IdentifierStr;
  getNextToken();//eat id1
  printf("Curtok is : %d\n", CurTok);
  //printf("current token is : %d. Working from ParsePrototype\n", CurTok);
  if (CurTok != '(')
  {
  	return ErrorP("Expected '(' in prototype");
  }
  getNextToken();	//eat '('
  printf("curtok is: %d working from ParsePrototype\n", CurTok);
  std::vector<std::string> ArgNames;
   
  if(CurTok != ')' )
  {
  	printf("Curtok is : %d. Working from ParsePrototype\n", CurTok);
	getNextToken(); //eat int
  ArgNames.push_back(IdentifierStr);
    getNextToken();// eat id
  while (CurTok == ',')
    {
    getNextToken(); //eat ,
    getNextToken(); //eat int
    ArgNames.push_back(IdentifierStr);
    getNextToken();// eat id
      
    }
    
  if (CurTok != ')')
  {
  	printf("curtok is : %d\n", CurTok);
  	return ErrorP("Expected ')' in prototype");
  }
    
  }
  
  // success.
  getNextToken();  // eat ')'.
  fprintf(stderr, "Returning from function ParsePrototype with current token %d\n", CurTok);
  return new PrototypeAST(FnName, ArgNames);
}

static PreAST *ParsePrecondition()
{
	fprintf(stderr, "Entering in function ParsePrecondition with current token %d\n", CurTok);
		std::string keyword = IdentifierStr;
		
		if(CurTok == tok_requires || tok_terminates)
		{
			getNextToken(); // eat keyword
			ExprAST *expression = ParseExpression();
			
			//getNextToken(); //get next keyword :D
			printf("exiting from function ParsePrecondition");
			return new PreAST(keyword, expression);
		}
		if(CurTok == tok_decrease)
		{
			getNextToken(); // eat keyword
			ExprAST *identifier = ParseIdentifierExpr();
			//getNextToken(); //get next keyword
			printf("exiting from function ParsePrecondition");
			return new PreAST(keyword, identifier);
		}
						
}

static PostAST *ParsePostcondition()
{
	fprintf(stderr, "Entering in function ParsePostcondition with current token %d\n", CurTok);
	std::string keyword = IdentifierStr;
	
	if(CurTok == tok_ensures)
	{
		getNextToken(); // eat keyword
		ExprAST *expression = ParseExpression();
			
			//getNextToken(); //get next keyword 
			printf("exiting from function ParsePostcondition");
			return new PostAST(keyword, expression);
	}
}

/// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() 
{
fprintf(stderr, "Entering in function ParseDefinition with current token %d\n", CurTok);
std::vector<PreAST*> pre;
std::vector<PostAST*> post;
while(CurTok != tok_int && CurTok != tok_void)
{
	if(CurTok == tok_requires || CurTok == tok_terminates || CurTok == tok_decrease)
	{
		PreAST *condition = ParsePrecondition();
		pre.push_back(condition);
		//getNextToken();
	}
	if(CurTok == tok_ensures)
	{
		PostAST *condition = ParsePostcondition();
		post.push_back(condition);
		//getNextToken();
	}
}


	//getNextToken(); //eat keyword
	//ExprAST *precondition = ParseExpression();
	//pre = ParsePrecondition();
	//getNextToken();


	//getNextToken(); //eat keyword
	//ExprAST *postcondition = ParseExpression();
	//post = ParsePostcondition();
	//getNextToken();

if(CurTok == tok_int || CurTok == tok_void)
{
  getNextToken();  // eat def.
 // printf("CURTOKen is : %d", CurTok);
  PrototypeAST *Proto = ParsePrototype();
  if (Proto == 0) 
  return 0;
printf("\ncurtok is in def after proto b4 body : %d\n", CurTok);
  if (BodyAST *E = ParseBody())
  {
    fprintf(stderr, "Returning from function ParseDefinition with current token %d\n", CurTok);
   // getNextToken();
    //printf("CurTok in definition: %d",CurTok);
    return new FunctionAST(Proto, pre, E, post);
    }
}
  return 0;
}


//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (1) {
    //fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof		:    return;
    case ';'			:        getNextToken(); break;  // ignore top-level semicolons.
    case tok_int		:    
    case tok_void		:   
    case tok_requires 		: 
    case tok_ensures 		: 
    case tok_terminates 	: 
    case tok_decrease 		: ParseDefinition(); 
    					break;
    case '{' 			: ParseBody(); 
    					break;
	default 		: //printf("CURRENT TOKEN IS: %d",CurTok);
					fprintf(stderr, "error in default"); 
					return;
    }
    //printf("m still in while");
    //getNextToken();
  }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence[tok_lessequal] = 10;
  BinopPrecedence['>'] = 10;
  BinopPrecedence[tok_greaterequal] = 10;
  
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence[tok_plusequal] = 20;
  BinopPrecedence[tok_minusequal] = 20;
  
  BinopPrecedence['*'] = 40;
  BinopPrecedence[tok_slash] = 40;
  BinopPrecedence['%'] = 40;
  BinopPrecedence[tok_starequal] = 40;
  BinopPrecedence[tok_percentequal] = 40;
  BinopPrecedence[tok_slashequal] = 40;
  
  BinopPrecedence[tok_equalequal] = 50;
  BinopPrecedence['='] = 50;  
  BinopPrecedence[tok_notequal] = 50;// highest.
  
  // Prime the first token.
  //fprintf(stderr, "ready> ");
  getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  return 0;
} 

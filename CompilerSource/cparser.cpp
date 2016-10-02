#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

static int CurTok;
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
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }

// primary_expression
//	: IDENTIFIER
//	| CONSTANT
//	| STRING_LITERAL
//	| '(' expression ')'
//	;

static ExprAST *ParsePriamryExpression()
{
	switch (CurTok)
	{
		default : return Error("unknown token");
		case tok_identifier : {
			std::string IdName = IdentifierStr;
			return new IdentifierAST(IdName);
			break;
		}
		case tok_constant   : return ParseConstant();
		case tok_stringlit  : return ParseStringlit();
		case tok_l_paren     : {
			getNextToken(); // eat left paren
			ParseExpression();
			getNextToken();
			if (CurTok != tok_rparen) return Error(" ) missing");
			getNextToken();
			break;
		}
	}
}

// postfix_expression
//	: primary_expression
//	| postfix_expression '[' expression ']'
//	| postfix_expression '(' ')'
//	| postfix_expression '(' argument_expression_list ')'
//	| postfix_expression '.' IDENTIFIER
//	| postfix_expression PTR_OP IDENTIFIER
//	| postfix_expression INC_OP
//	| postfix_expression DEC_OP
//	;

static ExprAST *ParsePostfixExpression()
{
	ParsePrimaryExpression();
	getNextToken();
	ParseRestPostfixExpression();
}

static ExprAST *ParseRestPostfixExpression()
{
	switch (CurTok) 
	{
		default : return ;
		case tok_lsquare : {
			getNextToken();
			ParseExpression();
			getNextToken();
			if (CurTok != tok_rsquare) return Error(" ] missing");
			getNextToken();
			ParseRestPostfixExpression();
			break;
		}
		case tok_lparen : {
			getNextToken();
			if (CurTok == tok_rparen) break;
			ParseArgumentExpressionList();
			getNextToken();
			if (CurTok != tok_rparen) return Error(" ) missing");
			getNextToken();
			ParseRestPostfixExpression();			
			break;
		}
		case tok_dot : {
			getNextToken();
			ParseIdentifier();
			getNextToken();
			ParseRestPostfixExpression();
			break;
		}
		case tok_plusplus : {
			
			getNextToken();
			ParseRestPostfixExpression();
			break;
		}
		case tok_minusminus : {
			
			getNextToken();
			ParseRestPostfixExpression();
			break;
		}
			
	}
	return ;
}

// argument_expression_list
//	: assignment_expression
//	| argument_expression_list ',' assignment_expression
//  ;	

static ExprAST = *ParseArgumentExpressionList()
{
	ParseAssignmentExpression();
	getNextToken();
	ParseRestArgumentExpressionList();
}

static ExprAst = *ParseRestArugmentExpressionList()
{
	if (CurToken == tok_comma)
	{
		getNextToken;
		ParseAssignentExpression();
		ParseRestAssignmentExpression();
	}
	else return;
}

// unary_expression
//	: postfix_expression
//	| INC_OP unary_expression
//	| DEC_OP unary_expression
//	| unary_operator cast_expression
//	| SIZEOF unary_expression
//	| SIZEOF '(' type_name ')'
//	;

static ExprAst = *ParseUnaryExpression()
{
	switch (CurTok)
	{
		default : ParsePostfixExpression();
		case tok_plusplus : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseUnaryExpression();
			int op = tok_plusplus;
			LHS = NULL;
			break;
		}
		case tok_minusminus : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseUnaryExpression();
			int op = tok_minusminus;
			LHS = NULL;
			break;
		}
		case tok_sizeof : {
			getNextToken();
			if (CurTok == tok_lparen)
			{
				ExprAST *V = new BinaryExprAST(op , LHS , RHS);
				*RHS = ParseTypeName();
				int op = tok_sizeof;
				LHS = NULL;
			}
			else 
			{
				ExprAST *V = new BinaryExprAST(op , LHS , RHS);
				*RHS = ParseUnaryExpression();
				int op = tok_sizeof;
				LHS = NULL;
			}
			break;
		}
		case tok_amp : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_amp;
			LHS = NULL;
			break;
		}
		case tok_star : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_star;
			LHS = NULL;
			break;
		}
		case tok_plus : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_plus;
			LHS = NULL;
			break;
		}
		case tok_minus : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_minus;
			LHS = NULL;
			break;
		}
		case tok_tilda;{
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_tilda;
			LHS = NULL;
			break;
		}
		case tok_exclaim : {
			ExprAST *V = new BinaryExprAST(op , LHS , RHS);
			*RHS = ParseCastExpression();
			int op = tok_exclaim;
			LHS = NULL;
			break;
		}
	}
	return ;
}

// cast_expression
//	: unary_expression
//	| '(' type_name ')' cast_expression
//	;

static ExprAST *ParseCastExpression()
{
	if ( Curtok == tok_lparen) 
	{
		getNextToken();
		ParseTypeName();
		getNextToken();
		if (CurTok != tok_rparen) return Error(" ) required");
		getNextToken();
		ParseCastExpression();
	}
	else ParseUnaryExpression();
	return ;
} 

// binary_expression
//	: cast_expression
//	| binary_expression binop binary_expression
//	;
/// binoprhs
///   ::= ('+' primary)*
static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
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
    ExprAST *RHS = ParseCastExpression();
    if (!RHS) return 0;
    
    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec+1, RHS);
      if (RHS == 0) return 0;
    }
    
    // Merge LHS/RHS.
    LHS = new BinaryExprAST(BinOp, LHS, RHS);
  }
}

/// expression
///   ::= primary binoprhs
///
static ExprAST *ParseBinaryExpression() {
  ExprAST *LHS = ParsePrimary();
  if (!LHS) return 0;
  
  return ParseBinOpRHS(0, LHS);
}

// conditional_expression
// 	: binary_expression
// 	| binary_expression '?' expression ':' conditional_expression
//	;
static ExprAST *ParseConditionExpression(){
	if (getNextToken == tok_question)
	{
		ExprAST *Logic = ParseBinaryExpression();
		getNextToken();
		ExprAST *Expression = ParseExpression();
		getNextToken();
		if (Curtok != tok_colon) Error(" : expected ");
		ExprAST *Condition = ParseConditionExpression();
		return new ConditionExpressionAST(Logic , Expression , Condition)
	}
	else return ParseBinaryExpression();
}

// assignment_expression
//	: conditional_expression
//	| unary_expression assignment_operator assignment_expression
//	;

//assignment_operator
//	: '='
//	| MUL_ASSIGN
//	| DIV_ASSIGN
//	| MOD_ASSIGN
//	| ADD_ASSIGN
//	| SUB_ASSIGN
//	| LEFT_ASSIGN
//	| RIGHT_ASSIGN
//	| AND_ASSIGN
//	| XOR_ASSIGN
//	| OR_ASSIGN
//	;

static ExprAST *ParseAssignmentExpression(){
	
}

// expression
//	: assignment_expression
//	| expression ',' assignment_expression
//	;

static ExprAST *ParseExpression(){
	ParseAssignmentExpression();
	getNextToken();
	ParseRestExpression();
}

static ExprAST *ParseRestExpression(){
	if (CurTok = tok_comma) return ParseRestExpression();
	else return 0 ;
}

// constant_expression
//	: conditional_expression
//	;
static ExprAST *ParseConstantExpression() {
	return ParseConditionalExpression();
}


// declaration
//	: declaration_specifiers ';'
//	| declaration_specifiers init_declarator_list ';'
//	;

static ExprAST *ParseDeclaration(){
	ParseDeclarationSpecifiers();
	getNextToken();
	if (CurTok != tok_semi)
	{
		getNextToken();
		ParseInitDeclartionList();
		if (getNextToken != tok_semi) return Error(" ; required");
		return 1;
	}
	return 1;
}

// declaration_specifiers
// 	: storage_class_specifier
// 	| storage_class_specifier declaration_specifiers
//	| type_specifier
//	| type_specifier declaration_specifiers
//	| type_qualifier
//	| type_qualifier declaration_specifiers
//	;

static ExprAST *ParseDeclarationSpecifiers(){
	switch (CurTok)
	{
		default : return 0;
		case tok_typedef : {
			ExprAST *Type = 'typedef' ;
			break;
		}
		case tok_extern : {
			ExprAST *Type = 'extern' ;
			break;
		}case tok_static : {
			ExprAST *Type = 'static' ;
			break;
		}case tok_auto : {
			ExprAST *Type = 'auto' ;
			break;
		}case tok_register : {
			ExprAST *Type = 'register' ;
			break;
		}
		case tok_void : {
			ExprAST *Type = 'void' ;
			break;
		}
		case tok_char : {
			ExprAST *Type = 'char' ;
			break;
		}
		case tok_short : {
			ExprAST *Type = 'short' ;
			break;
		}
		case tok_int : {
			ExprAST *Type = 'int' ;
			break;
		}
		case tok_long : {
			ExprAST *Type = 'long' ;
			break;
		}
		case tok_float : {
			ExprAST *Type = 'float' ;
			break;
		}
		case tok_double : {
			ExprAST *Type = 'double' ;
			break;
		}
		case tok_signed : {
			ExprAST *Type = 'signed' ;
			break;
		}
		case tok_unsigned : {
			ExprAST *Type = 'unsigned' ;
			break;
		}
		case tok_struct : {
			ExprAST *Type = 'struct' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 1;
				} 
				ParseStructDeclerationList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 1;
			}
			ParseStructDeclerationList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 1;
			break;
		}
		case tok_union : {
			ExprAST *Type = 'union' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 1;
				} 
				ParseStructDeclerationList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 1;
			}
			ParseStructDeclerationList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 1;
			break;
		}
		case tok_enum : {
			ExprAST *Type = 'enum' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 1;
				} 
				ParseEnumeratorList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 1;
			}
			ParseEnumeratorList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 1;
			break;
		}
		case tok_typename : {
			ExprAST *Type = 'typename' ;
			break;
		}
		ParseDeclationSpecifier();
	}
} 
// init_declarator_list
//	: init_declarator
//	| init_declarator_list ',' init_declarator
//	;
static ExprAST *ParseInitDeclaratorList(){
	ParseInitDeclarator();
	ParseRestInistDeclarator();
	return 0;
}

static ExprAST *ParseRestInistDeclarator(){
	if (CurTok == tok_comma) return ParseInitDeclarator();
	else return 0;
}

// init_declarator
//	: declarator
//	| declarator '=' initializer
//	;
static ExprAST *ParseInitDeclarator(){
	ParseDeclarator();
	getNextToken();
	if (CurTok == tok_equal) ParseInitializer();
	return 0;
}

// storage_class_specifier
//	: TYPEDEF
//	| EXTERN
//	| STATIC
//	| AUTO
//	| REGISTER
//	;

// type_specifier
//	: VOID
//	| CHAR
//	| SHORT
//	| INT
//	| LONG
//	| FLOAT
//	| DOUBLE
//	| SIGNED
//	| UNSIGNED
//	| struct_or_union_specifier
//	| enum_specifier
//	| TYPE_NAME
//	;

// struct_or_union_specifier
//	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
//	| struct_or_union '{' struct_declaration_list '}'
//	| struct_or_union IDENTIFIER
//	;

// struct_or_union
//	: STRUCT	
//	| UNION
//	;

// struct_declaration_list
//	: struct_declaration
//	| struct_declaration_list struct_declaration
//	;
static ExprAST *ParseStructDeclarationList(){
	ParseStructDeclaration();
	getNextToken();
	ParseRestStructDeclaration();
	return 0;
}

static ExprAST *ParseRestStructDeclaration(){
	return 0;
}

// struct_declaration
//	: specifier_qualifier_list struct_declarator_list ';'
//	;
static ExprAST *ParseStructDeclation(){
	ExprAST *Qualifier = new ParseSpecifierQualifierList();
	ExprAST *Declator = new ParseDeclatorList();
	if (getNextToken != tok_semi) return Error(" ; required");
	return new ParseStructDeclatorAST (Qualifier , Declarator);
}

// specifier_qualifier_list
//	: type_specifier specifier_qualifier_list
//	| type_specifier
//	| type_qualifier specifier_qualifier_list
//	| type_qualifier
//	;

static ExprAST *ParseSpecifierQualifierList() {
	switch (CurTok)
	{
		case tok_const: {
			ExprAST *Type = 'const';
			break;
		}
		case tok_volatile : {
			ExprAST *Type = 'volatile';
			break;
		}
		case tok_void : {
			ExprAST *Type = 'void' ;
			break;
		}
		case tok_char : {
			ExprAST *Type = 'char' ;
			break;
		}
		case tok_short : {
			ExprAST *Type = 'short' ;
			break;
		}
		case tok_int : {
			ExprAST *Type = 'int' ;
			break;
		}
		case tok_long : {
			ExprAST *Type = 'long' ;
			break;
		}
		case tok_float : {
			ExprAST *Type = 'float' ;
			break;
		}
		case tok_double : {
			ExprAST *Type = 'double' ;
			break;
		}
		case tok_signed : {
			ExprAST *Type = 'signed' ;
			break;
		}
		case tok_unsigned : {
			ExprAST *Type = 'unsigned' ;
			break;
		}
		case tok_struct : {
			ExprAST *Type = 'struct' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 0;
				} 
				ParseStructDeclerationList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 0;
			}
			ParseStructDeclerationList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 0;
			break;
		}
		case tok_union : {
			ExprAST *Type = 'union' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 0;
				} 
				ParseStructDeclerationList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 0;
			}
			ParseStructDeclerationList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 0;
			break;
		}
		case tok_enum : {
			ExprAST *Type = 'enum' ;
			getNextToken();
			if (CurTok != tok_l_brace){
				ParseIdentifier();
				getNextToken();
				if (CurTok != tok_l_brace){
					return 0;
				} 
				ParseEnumeratorList();
				if (CutTok != tok_r_brace) return Error(" } required");
				return 0;
			}
			ParseEnumeratorList();
			if (CutTok != tok_r_brace) return Error(" } required");
			return 0;
			break;
		}
		case tok_typename : {
			ExprAST *Type = 'typename' ;
			break;
		}
		ParseSpecifierQualifierList();
	}
}

// struct_declarator_list
//	: struct_declarator
//	| struct_declarator_list ',' struct_declarator
//	;
static ExprAST *ParseStructDeclaratorList(){
	ParseStructDeclarator();
	ParseRestStructDeclarator();
}

static ExprAST *ParseRestStructDeclarator(){
	if (getNextToken == tok_comma) return ParseStructDeclarator();
	else return 0;
}

// struct_declarator
//	: declarator
//	| ':' constant_expression
//	| declarator ':' constant_expression
//	;
static ExprAST *ParseStructDeclarator()
{
	if (CurTok  == tok_colon) {
		getNextToken(); //eat colon
		return ParseConstantExpression();
	}
	else {
		ParseDeclarator();
		if (CurTok  == tok_colon) {
			getNextToken(); //eat colon
			return ParseConstantExpression();
		}
	}
}

// enumerator_list
//	: enumerator
//	| enumerator_list ',' enumerator
//	;

static ExprAST *ParseEnumertorList(){
	ParseEnumerator();
	ParseRestEnumeratorList();
}

static ExprAST *ParseRestEnumeratorList(){
	if (CurTok == tok_comma){
		getNextToken(); //eat comma
		ParseEnumeratorList();
	}
	else return 0;
}

// enumerator
//	: IDENTIFIER
//	| IDENTIFIER '=' constant_expression
//	;

static ExprAST *ParseEnumerator(){
	ParseIdentifier();
	if (getNextToken == tok_equal){
		getNextToken(); //eat equal
	}
}

// declarator
//	: pointer direct_declarator
//	| direct_declarator
//	;

static ExprAST *ParseDeclarator(){
	if (CurTok == tok_star) ParsePointer();
	return ParseDirectDeclarator();
}

// direct_declarator
//	: IDENTIFIER
//	| '(' declarator ')'
//	| direct_declarator '[' constant_expression ']'
//	| direct_declarator '[' ']'
//	| direct_declarator '(' parameter_type_list ')'
//	| direct_declarator '(' identifier_list ')'
//	| direct_declarator '(' ')'
//	;

static ExprAST *ParseDirectDeclarator(){
	switch (CurTok) :
		{
			default : ParseRestDirectDeclarator();
			case tok_identifier : {
				std::string IdName = IdentifierStr;
				return new IdentifierAST(IdName);
				break;
			}
			case tok_l_paren : {
				ParseDeclarator();
				getNextToken();
				if (CurTok != tok_r_paren) return Error(" ) requires");
				return 0;
				break;
			}
		}
}

static ExprAST *ParseRestDirectDeclarator(){
	switch (CurTok): 
	{
		case tok_l_square : {
			if (getNextToken() != tok_r_square ) { 
				ParseConstantExpression();
				getNextToken();
				if (CurTok != tok_r_square) Error(" ] required");
			}
			break;
		}
		case tok_l_paren : {
			if (getNextToken() != tok_r_paren) {
				if (getNextToken() != tok_identifier){
					ParseParameterTypeList();
				}
				else ParseIdentifierList();
				getNextToken();
				if (CurTok != tok_r_paren) Error(" ) required ");
			}
			break;
		}
	}
	ParseRestDirectDeclarator();
}

// pointer
//	: '*'
//	| '*' type_qualifier_list
//	| '*' pointer
//	| '*' type_qualifier_list pointer
//	;
static ExprAST *ParseAST(){
	getNextToken();
	if (CurTok == tok_star) return ParsePointer();
	else {
		ParseTypeQualifierList();
		getNextToken();
		if (CurTok == tok_star) return ParsePointer();
	}
}

// type_qualifier_list
//	: type_qualifier
//	| type_qualifier_list type_qualifier
//	;
static ExprAST *ParseQualifierList(){
	while (CurTok == tok_const || tok_volatile)
	{
		if (CurTok == tok_const) {
			ExprAST *Type == ' const ';			
		}
		else ExprAST *Type == ' volatile ';
		getNextToken();
	}
}

// parameter_type_list
//	: parameter_list
//	| parameter_list ',' ELLIPSIS
//	;
static ExprAST *ParseParameterTypeList(){
	ParseParameterList();
	getNextToken();
	if (CurTok == tok_comma && getNextTok == tok_ellipsis) return 0;
	else Error(" , or ... missing");
}

// parameter_list
//	: parameter_declaration
//	| parameter_list ',' parameter_declaration
//	;
static ExprAST *ParseParameterList(){
	ParseParameterDeclaration();
	getNextToken();
	ParseRestParameterList();
}

static ExprAST *ParseRestParameterList(){
	if (CurTok == tok_comma) {
		getNextToken(); //eat comma
		ParseParameterList();
	}
	return 0;	
}

// parameter_declaration
//	: declaration_specifiers declarator
//	| declaration_specifiers abstract_declarator
//	| declaration_specifiers
//	;

static ExprAST *ParseParameterDeclaration(){
	ParseDeclarationSpecifiers();
	getNextToken();
	if (CurTok == tok_l_square || tok_l_brace || tok_l_paren) ParseAbstractDeclarator();
	else if (CurTok == tok_star && ( getNextToken() == tok_l_square || tok_l_brace || tok_l_paren )) ParseAbstractDeclarator();
	else ParseDeclaration();
}

// identifier_list
//	: IDENTIFIER
//	| identifier_list ',' IDENTIFIER
//	;

static ExprAST *ParseIdentifierList(){
	std::string IdName = IdentifierStr;
	ExprAST *Ident = new IdentifierAST(IdName);
	getNextToken();
	ParseRestIdentifierList();		
}

static ExprAST *ParseRestIdentifierList(){
	if (CurTok == tok_comma) return ParseIdentifierList();
	else return 0;
	
}

// type_name
//	: specifier_qualifier_list
//	| specifier_qualifier_list abstract_declarator
//	;

static ExprAST *ParseTypeName(){
	ParseSpecifierQualifierList();
	getNextToken();
	if (CurToken == tok_r_paren) ParseAbstractDeclarator
}

// abstract_declarator
//	: pointer
//	| direct_abstract_declarator
//	| pointer direct_abstract_declarator
//	;

static ExprAST *ParseAbstractDeclarator(){
	if (CurTok == tok_star) 
	{
		getNextToken();
		if (CurToken != tok_r_paren) ParseDirectAbstractDeclarator();
	}
	ParseDirectAbstractDeclarator();
}

// direct_abstract_declarator
//	: '(' abstract_declarator ')'
//	| '[' ']'
//	| '[' constant_expression ']'
//	| direct_abstract_declarator '[' ']'
//	| direct_abstract_declarator '[' constant_expression ']'
//	| '(' ')'
//	| '(' parameter_type_list ')'
//	| direct_abstract_declarator '(' ')'
//	| direct_abstract_declarator '(' parameter_type_list ')'
//	;

static ExprAST *ParseDirectAbstractDeclarator(){
	
}

// initializer
//	: assignment_expression
//	| '{' initializer_list '}'
//	| '{' initializer_list ',' '}'
//	;

static ExprAST *ParseInitializer(){
	if (CurTok != tok_l_brace) return ParseAssignmentExpression();
	else {
		getNextToken(); //eat left bracket
		ParseInitializerList();
		getNextToken();
		switch (CurTok) 
		{
			default : return Error( " } or , requires");
			case tok_r_brace : ;//do nothing
			case tok_comma : {
				if(getNextToken != tok_r_brace) {
					getNextToken();
					return Error(" } requires") ;
				}
				getNextToken;
				return 0;
				break;
			}
		}
	}
}

// initializer_list
//	: initializer
//	| initializer_list ',' initializer
//	;
static ExprAST *ParseInitializerList(){
	ParseInitializer();
	getNextToken();
	ParseRestInitializer();
}
static ExprAST *ParseRestInitializer(){
	if (CurTok == tok_comma) return ParseIntializerList();
	else return 0;
}

// statement
//	: labeled_statement
//	| compound_statement
//	| expression_statement
//	| selection_statement
//	| iteration_statement
//	| jump_statement
//	;
	
static StmtAST *ParseStatement(){
	switch (CurTok)
	{
		default : return ParseExpressionStatement();
		case tok_identifier || tok_case || tok_default : return ParseLabeledStament();
		case tok_l_brace : return ParseCompoundStatemt();
		case tok_if || tok_switch : return ParseSelectionStatement();
		case tok_for || tok_while || tok_do : return ParseIterationStatement();
		case tok_goto || tok_continue || tok_break || tok_return : return ParseJumpStatement();
	}
}
	
// labeled_statement
//	: IDENTIFIER ':' statement
//	| CASE constant_expression ':' statement
//	| DEFAULT ':' statement
//	;
static StmtAST *ParseLabeledStatement(){
	switch (CurTok)
	{
		default :  Error (" Unknown Token");
		case tok_identifier : {
			std::string IdName = IdentifierStr;
			StmtAST *Label = new IdentifierAST(IdName);
			break;
		}
		case tok_case : {
			getNextToken();
			StmtAST *Label = ParseConstantExpression();
			break;
		}
		case tok_default : {
			StmtAST *Label = 'Default' ;
			break;
		}
	}
	getNextToken();
	if (CurTok != tok_colon) Error(" : required");
	StmtAST *Stmt = ParseStmt();
	return new LabelStatementAST( Label , Stmt);
}

// compound_statement
//	: '{' '}'
//	| '{' statement_list '}'
//	| '{' declaration_list '}'
//	| '{' declaration_list statement_list '}'
//	;

static StmtAST *ParseCompoundStatement(){
	
}

// declaration_list
//	: declaration
//	| declaration_list declaration
//	;
static StmtAST *ParseDeclarationList(){
	while (1){
		ParseDeclaration();
	}
}

// statement_list
//	: statement
//	| statement_list statement
//	;

static StmtAST *ParseStatementList(){
	while (1){
		ParseStatement();
	}
}

// expression_statement
//	: ';'
//	| expression ';'
//	;

static StmtAST *ParseExpressionStatement(){
	if (CurTok != tok_colon) {
		ParseExpression();
		getNextToken();
		if (CurTok != tok_colon) return Error(" ; required");
	}
	else return 1;
}

// selection_statement
//	: IF '(' expression ')' statement
//	| IF '(' expression ')' statement ELSE statement
//	| SWITCH '(' expression ')' statement
//	;

static StmtAST *ParseSelectionStatement(){
	switch (CurTok)
	{
		case tok_if : {
			StmtAST *Selection = 'if' ;
			getNextToken();
			if (CurTok != tok_l_paren) return Error(" ( requires");
			getNextToken(); // eat left paren
			StmtAST *Expression = ParseExpression();
			if (CurTok != tok_r_paren) return Error(" ) requires");
			getNextToken(); // eat right paren
			StmtAST *Statement = ParseStatement();
			if (getNextToken() == tok_else){
				StmtAST *Selection == new SelectionStmtAST( Selection , Expression , Statement );
				StmtAST *Expression = 'else';
				getNextToken();
				StmtAST *Statement = ParseStatement();
			}
			return new SelectionStmtAST ( Selection , Expression , Statement);
			break;
		}
		case tok_switch : {
			StmtAST *Selection = 'if' ;
			getNextToken();
			if (CurTok != tok_l_paren) return Error(" ( requires");
			getNextToken(); // eat left paren
			StmtAST *Expression = ParseExpression();
			if (CurTok != tok_r_paren) return Error(" ) requires");
			getNextToken(); // eat right paren
			StmtAST *Statement = ParseStatement();
			return new SelectionStmtAST ( Selection , Expression , Statement);
			break;
		}
	}	
}

// iteration_statement
//	: WHILE '(' expression ')' statement
//	| DO statement WHILE '(' expression ')' ';'
//	| FOR '(' expression_statement expression_statement ')' statement
//	| FOR '(' expression_statement expression_statement expression ')' statement
//	;

static StmtAST *ParseIterationStatement(){
	switch (CurTok)
	{
		case tok_while : {
			StmtAST *Selection = 'while' ;
			getNextToken();
			if (CurTok != tok_l_paren) return Error(" ( requires");
			getNextToken(); // eat left paren
			StmtAST *Expression = ParseExpression();
			if (CurTok != tok_r_paren) return Error(" ) requires");
			getNextToken(); // eat right paren
			StmtAST *Statement = ParseStatement();
			return new SelectionStmtAST ( Selection , Expression , Statement);
			break;
		}
		case tok_do : {
			getNextToken(); // eat do
			StmtAST *Statement = ParseStatement();
			getNextToken();
			if (CurTok != tok_while) Error("while expected");
			StmtAST *Selection = 'while' ;
			getNextToken();
			if (CurTok != tok_l_paren) return Error(" ( requires");
			getNextToken(); // eat left paren
			StmtAST *Expression = ParseExpression();
			if (CurTok != tok_r_paren) return Error(" ) requires");
			getNextToken(); // eat right paren
			return new SelectionStmtAST ( Selection , Expression , Statement);
			break;
		}
		case tok_for : {
			
			StmtAST *Selection = 'for' ;
			getNextToken();
			if (CurTok != tok_l_paren) return Error(" ( requires");
			getNextToken(); // eat left paren
			StmtAST *Start = ParseExpressionStatement();
			StmtAST *Pre = ParaseExpressionStatement();
			if (CurTok != tok_r_paren) StmtAST *Expression = ParseExpression();
			if (CurTok != tok_r_paren) Error(" ) expected");
			getNextToken(); // eat right paren
			StmtAST *Statement = ParseStatement();
			return new ForStmtAST ( Start , Pre , Expression , Statement);
			break;
		}
	}
} 

// jump_statement
//	: GOTO IDENTIFIER ';'
//	| CONTINUE ';'
//	| BREAK ';'
//	| RETURN ';'
//	| RETURN expression ';'
//	;

static StmtAST *ParseJumpStatement()
{
	switch (CurTok)
	{
		case tok_goto : {
			StmtASt *Jump = 'goto' ;
			getNextToken();
			std::string IdName = IdentifierStr;
			StmtAST *At = IdName
			break;
		}
		case tok_continue: StmtAST *Jump = 'continue' ;
		case tok_break : StmtAST *Jump = 'break' ;
		case tok_return : {
			if (getNextToken() != tok_semi) StmtAST *At = ParseExpression();
			break;
		}
	}
	getNextToken();
	if (CurTok != tok_semi) return Error(" ; required") ;
	return new JumpStmtAST(Jump , At);
}

// translation_unit
//	: external_declaration
//	| translation_unit external_declaration
//	;

static AST *ParseTranslationUnit(){
	ParseExternalDeclaration();
	getNewToken();
	ParseRestTranslation();
}

static AST *ParseRestTranslation(){
	while (1) {
		ParseRestTranslation();
	}
}

// external_declaration
//	: function_definition
//	| declaration
//	;

// function_definition
//	: declaration_specifiers declarator declaration_list compound_statement
//	| declaration_specifiers declarator compound_statement
//	| declarator declaration_list compound_statement
//	| declarator compound_statement
//	;

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
  
  BinopPrecedence[tok_star] = 40;
  BinopPrecedence[tok_slash] = 40;
  BinopPrecedence[tok_percent] = 40;  // highest.

  // Prime the first token.
  fprintf(stderr, "ready> ");
  getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  return 0;
}

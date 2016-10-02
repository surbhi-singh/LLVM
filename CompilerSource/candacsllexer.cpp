#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <string>
#include "getch_ungetch.cpp"
// static void MainLoop();
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
  tok_old = -27, tok_notequal = -28, tok_terminates = -29, tok_decrease = -30,
  tok_openbrace = -31, tok_closebrace = -32, tok_print = -33 
};
int flag_comment = 0; //to be 1 if lexer is inside comment
int flag_atTheRate = 0; //to be 1 if lexer has read 1 @ character
static std::string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;              // Filled in if tok_number

/// gettok - printf(" the next token from standard input.
static int gettok() {
  static int LastChar = ' ';
  static int newchar = ' ';
  static int temp = ' ';
  //printf("%c \n", LastChar);
  //getch();
  //LastChar = getch();
  

   // Skip any whitespace.
 while (isspace(LastChar))
   LastChar = getch();
   
  temp = getch();
  //printf("before any if");

  if(LastChar == '/' && temp == '*')
  {
  	flag_comment = 1;
  	LastChar = getch();
  	
  	return gettok();
  }
  //printf("%d %d\n",LastChar, temp);
  else if (LastChar == '*' && temp == '/')
  {
  	//printf("i am here");
	 flag_comment = 0;
  	LastChar = getch();
  	
		return gettok();
  }
  else
  {
  	//if(temp != '\n')
  	ungetch(temp);
  	//getch();
	//printf("%c \n" , LastChar);
		
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
  	}//else ungetch(temp);
  	
  }
  

  // Skip any whitespace.
 while (isspace(LastChar))
   LastChar = getch();

  
if ((flag_comment == 0) || (flag_comment == 1 && flag_atTheRate == 1)) 
{
	//printf("above switch %c \n" , LastChar);
 switch(LastChar)
 {
 	case '+' :  newchar = getch();
	 			if (newchar == '+')
 				{
 					//getch();
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
 					//getch();
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
 	case '=' : newchar = getch();
	 			if(newchar == '=')
 				{
 					//getch();
 					LastChar = getch();
 					//printf("its ==\n lasthar is: %d", LastChar);
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
 	case '>' : newchar = getch();
	 			if(newchar == '=')
 				{
 					//getch();
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
 	case '<' : newchar = getch();
	 			if(newchar == '=')
 				{
 					//getch();
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
 					//getch();
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
 					//getch();
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
 	case '!' : newchar = getch();
 				if(newchar == '=')
 				{
 					//getch();
 					LastChar = getch();
 					return tok_notequal;
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
 					//getch();
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
	default :   //printf("in default");
	 			if (isalpha(LastChar)) 
	 			{ // identifier: [a-zA-Z][a-zA-Z0-9]*
    				IdentifierStr = LastChar;
    				//printf("%c\n", LastChar);
    				//getch();
    				//printf("%c \n" , LastChar);
    				while (isalnum(LastChar = getch()))
      					{
      						IdentifierStr += LastChar;
      						//printf("%c\n",LastChar);
      					}
      				//ungetch(LastChar);
      					//printf("%s\n", &IdentifierStr);
				if ( (flag_comment == 0)  &&  (IdentifierStr == "int")) return tok_int;
    				else if (flag_comment == 0  && IdentifierStr == "void" ) return tok_void;
  				else if (flag_comment == 0  && IdentifierStr == "while" ) return tok_while;
    				else if (flag_comment == 0  && IdentifierStr == "if") return tok_if;
    				else if (flag_comment == 0 && IdentifierStr == "else") return tok_else;
    				else if (flag_comment == 0 && IdentifierStr == "return" ) return tok_return;
    				else if (flag_comment == 0 && IdentifierStr == "printf" ) return tok_print;
    				else if (IdentifierStr == "ensures" && flag_atTheRate == 1) return tok_ensures;
				else if(IdentifierStr == "requires" && flag_atTheRate == 1) return tok_requires;
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
			//	else if(LastChar == '(')
			//	return tok_openbrace;
	 
				int ThisChar = LastChar;
 				LastChar = getch();
  				return ThisChar;
 				 				
 }
 }
 else
 {
 	while (LastChar != EOF && LastChar != '@' && LastChar != '*')
	{
	 	LastChar = getch();
    }
    ungetch(LastChar);
    LastChar = getch();
    return gettok();
 }

 //if (LastChar == '+' && newchar == '+') printf(" tok_plusplus");
 //else if (LastChar == '+' && newchar == '=') printf(" tok_plusequal");
 //else if (LastChar == '-' && newchar == '-') printf(" tok_minusminus");
 //else if (LastChar == '-' && newchar == '=') printf(" tok_minusequal");
// else if (LastChar == '=' && newchar == '=') printf(" tok_equalequal");
 //else if (LastChar == '>' && newchar == '=') printf(" tok_greaterequal");
 //else if (LastChar == '<' && newchar == '=') printf(" tok_lessequal");
 //else if (LastChar == '*' && newchar == '=') printf(" tok_starequal");
 //else if (LastChar == '%' && newchar == '=') printf(" tok_percentequal");
 //else if (LastChar == '/' && newchar == '=') printf(" tok_slashequal");
 //else if (LastChar == '/' && newchar == '*')
 //{
	
 //} 
 //else if (LastChar == '/') printf(" tok_slash");
 //else 
 //{
 //if(newchar == '+' || '-' || '=')
 // ungetch(newchar);
  
  // Otherwise, just return the character as its ascii value.
  //char ThisChar = (char)LastChar;
  //LastChar = getch();
  //printf(" %c ", ThisChar);
  
}

/*int main(){
	while(1)
	{
		int token = gettok();
		printf("token is %d\n", token);
		
	}
	
}*/

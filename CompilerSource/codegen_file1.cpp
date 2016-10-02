#include "candacslparsercopy.cpp"

//using namespace llvm;




Value *ErrorV(const char *Str) { ErrorV(Str); return 0; }

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                 TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0,
                           VarName.c_str());
}


Value *NumberExprAST::Codegen() {
	//////printf("\nI am in numexprAST function\n");
  return Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(32 , Val));
}

Value *VariableExprAST::Codegen() {
  //--- Look this variable up in the function.
  Value *V = NamedValues[Name];
  if (V == 0) return ErrorV("Unknown variable name");

  // Load the value.
  return Builder.CreateLoad(V, Name.c_str());
}

Value *AssignmentV(ExprAST *LHS, Value *RHS)
{
	VariableExprAST *LHSE = dynamic_cast<VariableExprAST*>(LHS);
  	if(!LHSE)
  	return ErrorV("destination of '=' must be a variable");
  	//Codegen the RHS
  	// Value *Val = RHS->Codegen();
 	 //if(Val == 0)
 	 //return 0;
 	 //Look up the name
 	 Value *Variable = NamedValues[LHSE->getName()];
 	 if(Variable == 0)
 	 return ErrorV("Unknown variable name");
 	 //////printf("\nI am in assignmentV function\n");
 	 Builder.CreateStore(RHS, Variable);
 	 return RHS;
  }

Value *BinaryExprAST::Codegen() {
////printf("\nI am in binaryexprAST function\n");
  Value *L = Lhs->Codegen();
  Value *R = Rhs->Codegen();
  if (L == 0 || R == 0) return 0;
  
  switch (Op) {
  case '='		: ////printf("\nI am in binaryexprAST function\n");
  			return AssignmentV(Lhs, R);
  				 
  case '+'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateAdd(L, R, "addtmp");
  
  case '-'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateSub(L, R, "subtmp");
  
  case '*'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateMul(L, R, "multmp");
  
  case '/'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateExactSDiv(L, R, "divtmp");
  
  case '%'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateFRem(L, R, "remtmp");
  
  case '<'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpULT(L, R, "cmptmp");
  
  case '>'		: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpUGT(L, R, "cmptmp");
  
  case tok_greaterequal	: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpUGE(L, R, "cmptmp");
  
  case tok_lessequal	: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpULE(L, R, "cmptmp");
  
  case tok_equalequal	: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpEQ(L, R, "cmptmp");
  
  case tok_notequal	: ////printf("\nI am in binaryexprAST function\n");
  			return Builder.CreateICmpNE(L, R, "cmptmp");
    
  case tok_plusplus 	: ////printf("\nI am in binaryexprAST function\n");
  			R = Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 1));
  			//R=ConstantFP::get(getGlobalContext(), APFloat(1.00)); 
  			R = Builder.CreateAdd(L, R, "addtmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_minusminus	: ////printf("\nI am in binaryexprAST function\n");
  			R = Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 1));
  			//R=ConstantFP::get(getGlobalContext(), APFloat(1.00)); 
  			R = Builder.CreateSub(L, R, "subtmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_plusequal	: ////printf("\nI am in binaryexprAST function\n");
  			R = Builder.CreateAdd(L, R, "addtmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_minusequal	: ////printf("\nI am in binaryexprAST function\n");
  			R = Builder.CreateSub(L, R, "subtmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_starequal	: ////printf("\nI am in binaryexprAST function\n");
  			R = Builder.CreateMul(L, R, "multmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_slashequal	: ////printf("\nI am in binaryexprAST function\n");
  			R = Builder.CreateExactSDiv(L, R, "divtmp" ); 
  			return AssignmentV(Lhs, R);
  
  case tok_percentequal	: ////printf("\nI am in binaryexprAST function\n");
  			R = Builder.CreateFRem(L, R, "remtmp" ); 
  			return AssignmentV(Lhs, R);
  
  default		: ////printf("\nI am in binaryexprAST function\n");
  			return ErrorV("invalid binary operator");
  }
}

Value *FunctionCallAST::Codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = TheModule->getFunction(Callee);
  if (CalleeF == 0)
    return ErrorV("Unknown function referenced");
  
  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return ErrorV("Incorrect # arguments passed");

  std::vector<Value*> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->Codegen());
    if (ArgsV.back() == 0) return 0;
  }
  ////printf("\nI am in functioncallAST function\n");
  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Value *IfStatementAST::Codegen() {
////printf("\nI am in if statementAST function\n");
  Value *CondV = Cond->Codegen();
  ////printf("after condition generation and b4 compare\n");
  if (CondV == 0) return 0;
//  Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , Val));
  
  // Convert condition to a bool by comparing equal to 0.0.
  CondV = Builder.CreateICmpNE(CondV, 
                              Constant::getIntegerValue(Type::getInt1Ty(getGlobalContext()), APInt(1, 0)),
                                "ifcond");
  ////printf("after the condition in if\n");
  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  ////printf("after thefunction in if\n");
  
  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB = BasicBlock::Create(getGlobalContext(), "then", TheFunction);
  ////printf("after thefunction in if 1\n");
  BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else");
  ////printf("after thefunction in if 2\n");
  BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");
  ////printf("after thefunction in if 3\n");
  Builder.CreateCondBr(CondV, ThenBB, ElseBB);
  ////printf("after thefunction in if after condition is made\n");
  // Emit then value.
  Builder.SetInsertPoint(ThenBB);
  ////printf("after thefunction in if after set insert\n");
  Value *ThenV = Body->Codegen();
  ////printf("after thefunction in if after body's codegen is called\n");
  if (ThenV == 0) return 0;
  
  Builder.CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.GetInsertBlock();
  
  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);
  
  Value *ElseV = Newbody->Codegen();
  if (ElseV == 0) return 0;
  
  Builder.CreateBr(MergeBB);
  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.GetInsertBlock();
  
  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  PHINode *PN = Builder.CreatePHI(Type::getInt32Ty(getGlobalContext()), 2,
                                  "iftmp");
  ////printf("\nI am in ifstatementAST function1\n");
  //LLVMAddIncoming(PN, ThenV, ThenBB, 2);
  PN->addIncoming(ThenV, ThenBB);
  ////printf("\nI am in ifstatementAST function2\n");
  PN->addIncoming(ElseV, ElseBB);
  ////printf("\nI am in ifstatementAST function3\n");
  
 
  //Value *growOperands(Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(32, 2)));
  //PN->setIncomingValue(1, ThenV);
  //PN->setIncomingBlock(1,ThenBB);
  //PN->setIncomingBlock(2, ElseBB);
 return PN;
}

Value *WhileStatementAST::Codegen() {
Value *CondV = Cond->Codegen();
  if (CondV == 0) return 0;
  
  // Convert condition to a bool by comparing equal to 0.0.
  CondV = Builder.CreateICmpNE(CondV, 
                             Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 0)),
                           //   ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                                "whilecond");
                                
Function *TheFunction = Builder.GetInsertBlock()->getParent();
   BasicBlock *LoopBB = BasicBlock::Create(getGlobalContext(), "loop", TheFunction);
  BasicBlock *BodyBB = BasicBlock::Create(getGlobalContext(), "body");
  BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "whilecont");
    
  Builder.SetInsertPoint(LoopBB);
  
  Builder.CreateCondBr(CondV, BodyBB, MergeBB);
  
  Builder.SetInsertPoint(BodyBB);
  
  Value *BodyV = Body->Codegen();
  if(BodyV == 0) return 0;
  
  Builder.CreateBr(LoopBB);
  BodyBB = Builder.GetInsertBlock();
  
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  
  // while stmnt always returns 0.0.
  ////printf("\nI am in whilestatementAST function\n");
  return Constant::getNullValue(Type::getInt32Ty(getGlobalContext()));
}

Value *ExprStatementAST::Codegen()
{
	////printf("\nI am in exprstatementAST function\n");
	return Expr->Codegen();
}


Value *PrintAST::Codegen()
{
	////printf("\nI am in printAST function\n");
	return Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 1));
	//ConstantFP::get(getGlobalContext(), APFloat(1.0));
}

Value *ReturnStatementAST::Codegen()
{
	////printf("i am in returnstatement \n");
	//createPost();
	//////printf("i am after createpost\n");
	
	if(Expr != NULL)
	{
		Function *TheFunction = Builder.GetInsertBlock()->getParent();
		Value *ExprV = Expr->Codegen();
		if (Post.size() >= 1 )
	  	{
	   		BasicBlock *PreBB = BasicBlock::Create(getGlobalContext(), "post", TheFunction);
	   		BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "return");
	   		BasicBlock *FalseBB = BasicBlock::Create(getGlobalContext(), "False");
	   		Builder.CreateBr(PreBB);
	  		Builder.SetInsertPoint(PreBB);
			std::vector<Value*> PostV;
			for (unsigned i = 0, e = Post.size(); i != e; ++i) {
	    			Value *V = Post[i]->Codegen();
		    		Builder.CreateCondBr(V, BB, FalseBB);
	    			PostV.push_back(V);
	    			if (PostV.back() == 0) return 0;
	    		}
			////printf("\nI am in returnstatementAST function\n");
			TheFunction->getBasicBlockList().push_back(BB);
			Builder.SetInsertPoint(BB);
			Builder.CreateRet(ExprV);
			/*TheFunction->getBasicBlockList().push_back(FalseBB);
  			Builder.SetInsertPoint(FalseBB);
 			Builder.CreateRetVoid();*/
		}
		else Builder.CreateRet(ExprV);
		
	}
	else
	{
		
		////printf("\nI am in returnstatementAST function\n");
		Builder.CreateRetVoid();
	}
	return Constant::getNullValue(Type::getInt32Ty(getGlobalContext()));
	
}
/*int ReturnStatementAST::createPost()
{
	////printf("i am in createpost of return\n");
	  std::vector<Value*> PostV;
  for (unsigned i = 0, e = Post.size(); i != e; ++i) {
  ////printf("i am in for of createpost\n");
    Value *V = Post[i]->Codegen();
    //Builder.CreateCondBr(V, BB, FalseBB);
    PostV.push_back(V);
    //assertV
    //PreV.push_back(Pre[i]->createPre(BB, FalseBB));
    if (PostV.back() == 0) return 0;
    }
	//Value *V = Post->Codegen();
	//return //Builder.CreateCond(V, ReturnBB, FalseBB);
}*/

Value *VarDeclAST::Codegen()
{
	////printf("\nI am in vardeclAST function\n");
	return Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 1));
	//ConstantFP::get(getGlobalContext(), APFloat(1.0));
	
}

Value *BodyAST::Codegen(){
////printf("\nI am in bodyAST function's starting\n");
 //Function *TheFunction = Builder.GetInsertBlock()->getParent();
 //BasicBlock *BodyBB = BasicBlock::Create(getGlobalContext(), "body", TheFunction);
 //Builder.SetInsertPoint(BodyBB);
 std::vector<Value*> TempStatement;
 for(unsigned i =0, e = Statements.size(); i !=e; ++i)
 {
 	//if(i != isReturn)
 	TempStatement.push_back(Statements[i]->Codegen());
 	//else
 	//createPost(extern FunctionAST::Post);
 	if (TempStatement.back() == 0) return 0;
 }
 //BodyBB = Builder.GetInsertBlock();
 ////printf("\nI am in bodyAST function\n");
 return Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 1));
}
//static ExprAST *Binexpr;
/*Value *ExprAST::Codegen()
{
	if(isBinOp)
	{	
		
		return BinaryExprAST->Codegen();
	}
	else if(isIdentifier)
	return VariableExprAST::*Codegen();
	else if(isNumber)
	return NumberExprAST::Codegen();
	else
	return ErrorV("Invalid");
	
}*/

Value *PreAST::Codegen()
{
	
	////printf("\nI am in preAST function\n");
	//Function *TheFunction = Builder.GetInsertBlock()->getParent();
	
	/*if(isBinOp)
	{
		////printf("\nI am in preAST function's binary operator\n");
		BinaryExprAST *precond = dynamic_cast<BinaryExprAST*>(Precondition);
		CondV = precond->Codegen();
	}
	else if(isIdentifier)
	{
		////printf("\nI am in preAST function's identifier\n");
		VariableExprAST *precond = dynamic_cast<VariableExprAST*>(Precondition);
		CondV = precond->Codegen();
	}
	else if(isNumber)
	{	
		////printf("\nI am in preAST function's number\n");
		NumberExprAST *precond = dynamic_cast<NumberExprAST*>(Precondition);
		CondV = precond->Codegen();
	}*/
	
	////printf("\nI am in preAST function b4 precondition's codegen is called\n");
	Value *CondV = Precondition->Codegen();
	////printf("\nI am in preAST function after precondition's codegen is called\n");
	if (Keyword == "requires")
	{
		////printf("\nI am in preAST function\n");
		/*return Builder.CreateICmpNE(CondV, 
                Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 0)),
                "reqcond");
                ////printf("\nseg fault is in return\n");*/
                return CondV;
                //
               // ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                //"reqcond");
         }
         /*else if(Keyword == "terminates")
         {
         	////printf("\nI am in preAST function\n");
         	return Builder.CreateFCmpONE(CondV, 
         	
                ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                "reqcond");
         }
         else if(Keyword == "decrease")
         {
         	////printf("\nI am in preAST function\n");
         	return Builder.CreateFCmpONE(CondV, 
                ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                "reqcond");
         }*/
         else return ErrorV("Invalid precondition");
}
Value *PostAST::Codegen()
{
	Value *CondV = Postcondition->Codegen();
	if(Keyword == "ensures")
	{
		////printf("\nI am in postAST function\n");
		/*return Builder.CreateFCmpONE(CondV,
		Constant::getIntegerValue(Type::getInt32Ty(getGlobalContext()), APInt(1 , 0)),
  		//ConstantFP::get(getGlobalContext(), APFloat(0.0)),
  		"enscond");*/
  		return CondV;
	}
	else return ErrorV("Invalid precondition");
}
/*void BodyAST::createPost()
{
	
}*/


Function *PrototypeAST::Codegen() {
  
  ////printf("\nI am in prototypeAST function\n");
  
  //size_t s = 0;
//for (std::vector<set<std::string> >::const_iterator cit = Args.begin(); cit != Args.end(); ++cit) {
  //  size += cit->size();
//}
  //////printf()
  //size_t s = 0;
  //////printf("\nbeginning is: %d\n",(int) Args.begin());
  //////printf("\nthis size is: %d \n", this->Size);
  //////printf("actual param size is : %d\n", Size);
   
  
  
  // Make the function type:  double(double,double) etc.
    std::vector<Type*> Integers(ArgNames.size(),
                             Type::getInt32Ty(getGlobalContext()));
	//////printf("\nI am in prototypeAST function's 1\n");
	FunctionType *FT;
	if(isFunInt)
	{
  FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                                       Integers, false);
        ////printf("i am in proto's if\n");
        }
        else if (isFunVoid)
         {
         FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                                       Integers, false);
           ////printf("\nI am in prototypeAST function's else\n");
           }
  
  Function *F = Function::Create(FT, Function::ExternalLinkage, FnName, TheModule);
  //////printf("\nI am in prototypeAST function's 3\n");
  // If F conflicted, there was already something named 'Name'.  If it has a
  // body, don't allow redefinition or reextern.
  if (F->getName() != FnName) {
  ////printf("\nI am in prototypeAST function's if\n");
    // Delete the one we just made and get the existing one.
    F->eraseFromParent();
    //////printf("\nI am in prototypeAST function's if 1\n");
    F = TheModule->getFunction(FnName);
    
    // If F already has a body, reject this.
    if (!F->empty()) {
      ErrorF("redefinition of function");
      return 0;
    }
    
    // If F took a different number of args, reject.
    if (F->arg_size() != ArgNames.size()) {
      ErrorF("redefinition of function with different # args");
      return 0;
    }
  }
  
  // Set names for all arguments.
  unsigned Idx = 0;
  for (Function::arg_iterator AI = F->arg_begin(); Idx != ArgNames.size();
       ++AI, ++Idx) {
    AI->setName(ArgNames[Idx]);
  }
  ////printf("\nI am in prototypeAST function\n");
  return F;
}

/// CreateArgumentAllocas - Create an alloca for each argument and register the
/// argument in the symbol table so that references to it will succeed.
void PrototypeAST::CreateArgumentAllocas(Function *F) {
////printf("\nI am in prototypeAST alloca function\n");
  Function::arg_iterator AI = F->arg_begin();
  ////printf("\nI am in prototypeAST alloca function arg \n");
  for (unsigned Idx = 0, e = ArgNames.size(); Idx != e; ++Idx, ++AI) {
  //////printf("name of first argument is: %c", AI);
    //std::cout<<AI;
    // Create an alloca for this variable.
    ////printf("\nI am in prototypeAST function for\n");
    AllocaInst *Alloca = CreateEntryBlockAlloca(F, ArgNames[Idx]);
 ////printf("\nI am in prototypeAST function for 1\n");
    // Store the initial value into the alloca.
    Builder.CreateStore(AI, Alloca);
    ////printf("\nI am in prototypeAST function for2\n");

    // Add arguments to variable symbol table.
    NamedValues[ArgNames[Idx]] = Alloca;
    ////printf("\nI am in prototypeAST function for 3\n");
  }
}



Function *FunctionAST::Codegen() {
////printf("\nI am in functionAST function\n");
//Value *R=ConstantFP::get(getGlobalContext(), APFloat(1.00)); 
//Value *L=ConstantFP::get(getGlobalContext(), APFloat(2.00)); 
  //			R = Builder.CreateFSub(L, R, "subtmp" );

  NamedValues.clear();
  ////printf("\nI am in functionAST function after clear\n");
  
  Function *TheFunction = Prototype->Codegen();
  ////printf("\nI am in functionAST function after prototype is called\n");
  if (TheFunction == 0)
    return 0;
  BasicBlock *BB;
  BasicBlock *FalseBB;
  if (Pre.size() >= 1 )
  {
   BasicBlock *PreBB = BasicBlock::Create(getGlobalContext(), "pre", TheFunction);
   BB = BasicBlock::Create(getGlobalContext(), "entry");
   FalseBB = BasicBlock::Create(getGlobalContext(), "False");
  Builder.SetInsertPoint(PreBB);
  std::vector<Value*> PreV;
  for (unsigned i = 0, e = Pre.size(); i != e; ++i) {
    Value *V = Pre[i]->Codegen();
    Builder.CreateCondBr(V, BB, FalseBB);
    PreV.push_back(V);
    //assertV
    //PreV.push_back(Pre[i]->createPre(BB, FalseBB));
    if (PreV.back() == 0) return 0;
    }
    // Builder.CreateCondBr(PreV, BB, FalseBB);
    //BB = BasicBlock::Create(getGlobalContext(), "entry");
  
  }
  else 
   {
   	BB = BasicBlock::Create(getGlobalContext(), "entry", TheFunction);
   	FalseBB = BasicBlock::Create(getGlobalContext(), "False");
  }
  ////printf("\nI am in functionAST function after pre\n");
  
  // Create a new basic block to start insertion into.
  
  ////printf("\nI am in functionAST function after bb1\n");
  if(Pre.size() >=1) TheFunction->getBasicBlockList().push_back(BB);
  Builder.SetInsertPoint(BB);
  ////printf("\nI am in functionAST function after bb2\n");
  Prototype->CreateArgumentAllocas(TheFunction);
  
  ////printf("\nI am in functionAST function after proto alloc\n");
  if (Value *RetVal = Body->Codegen()) {
  // Finish off the function.
    //Builder.CreateRet(RetVal);
  TheFunction->getBasicBlockList().push_back(FalseBB);
  Builder.SetInsertPoint(FalseBB);
  Builder.CreateRetVoid();
  	//Value *PostV = Post->Codegen();
  ////printf("\nI am in functionAST function after body\n");
    
    ////printf("\nI am in functionAST function after retval\n");

    // Validate the generated code, checking for consistency.
    //verifyFunction(*TheFunction);
    ////printf("\nI am in functionAST function after verify\n");

    // Optimize the function.
    //TheFPM->run(*TheFunction);
    ////printf("\nI am in functionAST function\n");
    return TheFunction;
  }
  
  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return 0;
}

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

static ExecutionEngine *TheExecutionEngine;

static void HandleDefinition() {
  if (FunctionAST *NEW = ParseDefinition()) {
  	////printf("\nI am in Handledefinition \n");
  if( Function *the = NEW->Codegen())
    {	////printf("\nI am in preAST function's inner if\n");
      fprintf(stderr, "Read function definition:");
      the->dump();
     }
     
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

//#include "codegen_file1.cpp"
//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

/// top ::= definition | external | expression | ';'
static void MainLoop() {

  while (1) {
    //fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof		: return;
    case ';'			: getNextToken(); 
    				break;  // ignore top-level semicolons.
    case tok_int		:    
    case tok_void		:   
    case tok_requires 		: 
    case tok_ensures 		: 
    case tok_terminates 	: 
   // case tok_behaviour	:
    case tok_decrease 		: return HandleDefinition(); 
  //  case '{' 			: ParseBody( ); 
   // case tok_invalid		: fprintf(stderr, "invalid token");return 0;
	default 		: //////printf("CURRENT TOKEN IS: %d",CurTok);
						fprintf(stderr, "error in default"); 
					return;
    }
    //////printf("m still in while");
    //getNextToken();
  }
  
}

//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//

/// putchard - putchar that takes a double and returns 0.
extern "C" 
double putchard(double X) {
  putchar((char)X);
  return 0;
}

/// printd - ////printf that takes a double prints it as "%f\n", returning 0.
extern "C" 
double printd(double X) {
  ////printf("%f\n", X);
  return 0;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {

   InitializeNativeTarget();
  LLVMContext &Context = getGlobalContext();
  
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
  
 // Make the module, which holds all the code.
  TheModule = new Module("my cool jit", Context);

  // Create the JIT.  This takes ownership of the module.
  std::string ErrStr;
  TheExecutionEngine = EngineBuilder(TheModule).setErrorStr(&ErrStr).create();
  if (!TheExecutionEngine) {
    fprintf(stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str());
    exit(1);
  }

  FunctionPassManager OurFPM(TheModule);

  // Set up the optimizer pipeline.  Start with registering info about how the
  // target lays out data structures.
  OurFPM.add(new DataLayout(*TheExecutionEngine->getDataLayout()));
  // Provide basic AliasAnalysis support for GVN.
  OurFPM.add(createBasicAliasAnalysisPass());
  // Promote allocas to registers.
  OurFPM.add(createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  OurFPM.add(createInstructionCombiningPass());
  // Reassociate expressions.
  OurFPM.add(createReassociatePass());
  // Eliminate Common SubExpressions.
  OurFPM.add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  OurFPM.add(createCFGSimplificationPass());

  OurFPM.doInitialization();

  // Set the global so the code gen can use this.
  TheFPM = &OurFPM;

  // Run the main "interpreter loop" now.
  MainLoop();

  TheFPM = 0;

  // Print out all of the generated code.
  TheModule->dump();

  return 0;
}

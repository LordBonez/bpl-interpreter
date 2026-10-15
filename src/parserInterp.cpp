/* Implementation of Recursive-Descent Parser
 * for the Basic Perl-Like Language (BPL)
 * parse.cpp
 * Programming Assignment 2
 * Anthony Vorontsov
 * Fall 2025
*/

#include "parserInt.h"


map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value>* ValQue; //declare a pointer variable to a queue of Value objects

int sign = 0;
bool execute = true;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}


//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;
	//cout << tok << " " << tok.GetLinenum() <<endl;
	
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
			
	if(!f1) 
	{
		ParseError(line, "Missing Program");
		return false;
	}
	else 
	{
		
		cout << endl <<endl;
		cout << "DONE" << endl;	
		return true;
	}	
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
		
	LexItem tok;
	
	status = Stmt(in, line);
	while(status )
	{
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else if(tok == RBRACES)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		
		if(tok != SEMICOL)
		{
			line--;
			ParseError(line, "Missing semicolon at end of Statement");
			return false;
		}
		
		status = Stmt(in, line);
		
	}
			
	tok = Parser::GetNextToken(in, line);
	
	if(tok == ELSE )
	{
		ParseError(line, "Illegal If Statement Else-Clause");
		return false;
	}
	else if(tok == RBRACES)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	else if(tok == DONE && !status)
	{
		
		ParseError(line, "Syntactic error in Program Body");
		return false;
	}
	else
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	
}//End of StmtList function

//Stmt ::= AssignStme | PrintLnStmt | IfStmt  
bool Stmt(istream& in, int& line){
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {
	case IDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Assignment Statement.");
			return status;
		}
		break;
	case PRINTLN:
		
		status = PrintLnStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect PrintLn Statement");
			return status;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect If-Statement.");
			return status;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	
	case DONE:
		return true;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function


//PrintLnStmt:= PRINTLN (ExpreList) 
bool PrintLnStmt(istream& in, int& line) {
	LexItem t;
		
	ValQue = new queue <Value>;

	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		ParseError(line, "Missing Left Parenthesis of PrintLn Statement");
		delete ValQue;
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after PrintLn");
		delete ValQue;
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of PrintLn Statement");
		delete ValQue;
		return false;
	}

	if (execute)
	{
		while (!ValQue->empty())
		{
			cout << ValQue->front();
			ValQue->pop();
		}
		cout << endl;
	}

	delete ValQue;
	return true;
}//End of PrintlnStmt

//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	
	Value retVal;

	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	
	ex = Expr(in, line, retVal);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != LBRACES)
	{
		Parser::PushBackToken(t);
		ParseError(line, "Missing left brace for If Statement Clause");
		return false;
	}

	bool condition = (retVal.IsBool() && retVal.GetBool() == true) ||
		(retVal.IsString() && !retVal.GetString().empty() && retVal.GetString()[0] != '0');

	execute = condition;
	status = StmtList(in, line);
	execute = true;
	if (!status)
	{
		ParseError(line, "Missing Statement for If Statement Clause");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if(t != RBRACES)
	{
		ParseError(line, "Missing right brace for If Statement Clause");
		return false;
	}	
	t = Parser::GetNextToken(in, line);
	
	if( t == ELSE ) {
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES)
		{
			Parser::PushBackToken(t);
			ParseError(line, "Missing left brace for Else-Clause");
			return false;
		}

		execute = !condition;
		status = StmtList(in, line);
		execute = true;
		if (!status)
		{
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if( t != RBRACES)
		{
			line--;
			Parser::PushBackToken(t);
			ParseError(line, "Missing right brace for an Else-Clause");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(t);
		return true;
	}
	return true;
}//End of IfStmt function

//Var ::= IDENT
bool Var(istream& in, int& line, LexItem& idtok)
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT ){
		identstr = tok.GetLexeme();
		
		//if (!(defVar.find(identstr)->second))
		if (defVar.find(tok.GetLexeme()) == defVar.end())
			defVar[identstr] = true;

		if (execute)
			idtok = tok;

		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	
	return false;
}//End of Var

//AssignStmt:= Var AssignOp Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t;
	
	Value retVal;

	LexItem idtok;

	varstatus = Var( in, line, idtok);
		
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP || t == CADDA || t == CSUBA || t == CCATA){
			status = Expr(in, line, retVal);
			
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}

			if (execute)
			{
				Value& ValueL = TempsResults[idtok.GetLexeme()];

				if (sign == 2 && retVal.GetType() == VSTRING)
				{
					ParseError(line, "Illegal Assignment of a boolean value to a variable.");
					return false;
				}

				if (t == ASSOP)
				{
					ValueL = retVal;
				}
				else if (t == CADDA)
				{
					ValueL = ValueL + retVal;
				}
				else if (t == CSUBA)
				{
					ValueL = ValueL - retVal;
				}
				else if (t == CCATA)
				{
					ValueL = ValueL.Catenate(retVal);
				}
				if (ValueL.IsErr())
				{
					ParseError(line, "Error in Assignment Statement");
					return false;
				}
			}
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	
	Value retVal;

	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}

	ValQue->push(retVal);
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr ::= OrExpr ::= AndExpr { || AndExpr }
bool Expr(istream& in, int& line, Value& retVal) {
	
	LexItem tok;
	bool t1 = AndExpr(in, line, retVal);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR  ) 
	{
		Value retValR;
		t1 = AndExpr(in, line, retValR);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after OR operator");
			return false;
		}
		
		if (execute)
		{
			retVal = retVal || retValR;
			if (retVal.IsErr())
			{
				ParseError(line, "Illegal Relational operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of OrExpr or Expr

//	AndExpr ::= RelExpr  [ && RelExpr ]
bool AndExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	bool t1 = RelExpr(in, line, retVal);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND  ) 
	{
		Value retValR;
		t1 = RelExpr(in, line, retValR);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after AND operator");
			return false;
		}
		
		if (execute)
		{
			retVal = retVal && retValR;
			if (retVal.IsErr())
			{
				ParseError(line, "Illegal Relational operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AndExpr

//RelExpr ::= AddExpr  [ ( @le | @gt | @eq | < | >= | == ) AddExpr ]
bool RelExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	bool t1 = AddExpr(in, line, retVal);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NGTE || tok == NLT || tok == NEQ || tok == SLTE || tok == SGT || tok == SEQ ) 
	{
		Value retValR;
		t1 = AddExpr(in, line, retValR);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after a relational operator");
			return false;
		}

		if (execute)
		{
			if (tok == NGTE)
			{
				retVal = retVal >= retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
			else if (tok == NLT)
			{
				retVal = retVal < retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
			else if (tok == NEQ)
			{
				retVal = retVal == retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
			else if (tok == SLTE)
			{
				retVal = retVal.SLE(retValR);
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
			else if (tok == SGT)
			{
				retVal = retVal.SGT(retValR);
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
			else if (tok == SEQ)
			{
				retVal = retVal.SEQ(retValR);
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal Relational operation.");
					return false;
				}
			}
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value& retVal) {
	
	bool t1 = MultExpr(in, line, retVal);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		Value retValR;
		t1 = MultExpr(in, line, retValR);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (execute)
		{
			if (tok == PLUS)
			{
				retVal = retVal + retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal opernad type for the operation.");
					return false;
				}
			}
			else if (tok == MINUS)
			{
				retVal = retVal - retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal opernad type for the operation.");
					return false;
				}
			}
			else if (tok == CAT)
			{
				retVal = retVal.Catenate(retValR);
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal opernad type for the operation.");
					return false;
				}
			}
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr

//MultExpr ::= ExponExpr { ( * | / | .x.) ExponExpr }
bool MultExpr(istream& in, int& line, Value& retVal) {
	
	bool t1 = UnaryExpr(in, line, retVal);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	//while ( tok == MULT || tok == DIV  || tok == SREPEAT)
	while ( tok == MULT || tok == DIV || tok == REM || tok == SREPEAT)
	{
		Value retValR;
		t1 = UnaryExpr(in, line, retValR);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (execute)
		{
			if (tok == MULT)
			{
				retVal = retVal * retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal opernad type for the operation.");
					return false;
				}
			}
			else if (tok == DIV)
			{
				retVal = retVal / retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal operand type or value for the division operation.");
					return false;
				}
			}
			else if (tok == REM)
			{
				retVal = retVal % retValR;
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal operand type for the operation.");
					return false;
				}
			}
			else if (tok == SREPEAT)
			{
				retVal = retVal.Repeat(retValR);
				if (retVal.IsErr())
				{
					ParseError(line, "Illegal operand type for the string repetition operation.");
					return false;
				}
			}
		}

		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//UnaryExpr ::= [( - | + | ! )] ExponExpr 
bool UnaryExpr(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	//int sign = 0;
	sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else if(t == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(t);
		
	status = ExponExpr(in, line, sign, retVal);
	if(!status)
	{
		ParseError(line, "Missing operand for an operator");
		return false;
	}

	return status;
}//End of UnaryExpr

//ExponExpr ::= PrimaryExpr { ** PrimaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, int sign, Value& retVal)
{
	bool status;
		
	status = PrimaryExpr(in, line, sign, retVal);
	if( !status ) {
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT)
	{
		Value retValR;
		status = ExponExpr(in, line, sign, retValR);
		if( !status ) {
			ParseError(line, "Missing exponent operand after exponentiation");
			return false;
		}

		if (execute)
		{
			retVal = retVal.Expon(retValR);
			if (retVal.IsErr())
			{
				ParseError(line, "Illegal exponentiation operation.");
				return false;
			}
		}

		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of ExponExpr


//PrimaryExpr ::= IDENT | ICONST | FCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign, Value& retVal) {

Next:
	LexItem tok = Parser::GetNextToken(in, line);
	
	if( tok == IDENT ) {
		 
		string lexeme = tok.GetLexeme();
		//if (!(defVar.find(lexeme)->second))
		if (defVar.find(lexeme) == defVar.end())
		{
			ParseError(line, "Using Undefined Variable: "+ lexeme);
			Parser::PushBackToken(tok);
			
			return false;	
		}

		if (execute)
		{
			if (TempsResults.find(lexeme) == TempsResults.end())
			{
				ParseError(line, "Using Undefined Variable: " + lexeme);
				Parser::PushBackToken(tok);

				return false;
			}

			if (sign == 2)
				retVal = !TempsResults[lexeme];
			else
				retVal = TempsResults[lexeme];
		}
		return true;
	}
	else if( tok == ICONST ) {

		if (execute)
		{
			retVal.SetType(VNUM);
			int Temp = atoi(tok.GetLexeme().c_str());
			if (sign == -1)
				Temp = -Temp;
			retVal.SetNum(Temp);
		}
		return true;
	}
	else if( tok == SCONST ) {

		if (execute)
		{
			if (sign == -1)
			{
				ParseError(line, "Illegal Operand Type for Sign Operator");
				Parser::PushBackToken(tok);
				return false;
			}
			retVal.SetType(VSTRING);
			retVal.SetString(tok.GetLexeme());
		}
		return true;
	}
	else if( tok == FCONST ) {

		if (execute)
		{
			retVal.SetType(VNUM);
			double Temp = atof(tok.GetLexeme().c_str());
			if (sign == -1)
				Temp = -Temp;
			retVal.SetNum(Temp);
		}
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	if (!execute)
		goto Next;	// ignore unrecognized Token when not executing IF condition and move to the Next Token

	return false;
}

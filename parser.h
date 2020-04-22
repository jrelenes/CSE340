#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "lexer.h"
#include <map>
#include <string.h>
#include <string>
using namespace std;

string mem[1000];
map<string, int> SymbolTable;


class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek(); //get unget
  public:

	void program();
	void scope(string);
	void scope_list(string);
	void var_decl(string);
	void id_list(struct Variables*, string);
	void type_name(struct Variables*);
	void stmt_list(string);
	void stmt(string);
	void assign_stmt(string);
	void while_stmt(string);
	void expr(Token, string);
	void arithmetic_expr();
	void boolean_expr();
	void arithmetic_operator();
	void binary_boolean_operator();
	void relational_operator();
	void primary(string, Token);
	void arithmetic_primary();
	void boolean_primary();
	void bool_const();
	void condition();
	void SyntaxError();
	pair<bool,string> name(Token, Token);

};
	//tracks scope location
	// if not found local is
	//compatible with greater scope values

	int ScopeLevelCounter = 0;
	bool printh = true;

    struct Variables
	{
		vector<pair<Token,bool>> variable;
		Token type;
		int ScopeLevel;
		string LevelName;
		struct Variables *next;
	};


	struct TYPE
	{
		//type mismatch error
		//declare all in same structure to
		//call later
		
		int lineNo;
		string errorType;
		
		struct TYPE *next;
	};	
	
	struct UNINITIALIZED
	{
		string uninitialized_name;
		int uninitialized_line_no;
		struct UNINITIALIZED *next;
	};

	struct NO_ERROR
	{
		string left;
		Token declared_line_no;
		int assigned_line_no;
		struct NO_ERROR *next;
	};	

	struct Error
	{
		string error_code;
		string symbol_name;
		int priority;
		struct Error *next;
	};	

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "parser.h"
#include <map>
using namespace std;

int next_available = 0;
int prio = 0;

//uninitialized variables
UNINITIALIZED* uninitialized = new UNINITIALIZED();

//type checking
TYPE* type_eval = new TYPE();

//variables
Variables* var_types = new Variables();

//error codes
Error* errorCode = new Error();

//scope version
int version = 0;


//revaluate the scope version LevelName


/*
FIRST(program) = {LBRACE}
FIRST(scope) = {LBRACE}
FIRST(scope_list) = {LBRACE, ID, WHILE}
FIRST(var_decl) = {ID}
FIRST(id_list) = {ID}
FIRST(type_name) = {REAL, INT, BOOLEAN, STRING}
FIRST(stmt_list) = {ID, WHILE}
FIRST(stmt) = {ID, WHILE}
FIRST(assign_stmt) = {ID}
FIRST(while_stmt) = {WHILE}
FIRST(expr) = {}
FIRST(arithmetic_expr) = {PLUS, MINUS, MULT, DIV, ID, NUM, REALNUM, STRING_CONSTANT}
FIRST(boolean_expr) = {AND, OR, XOR, GREATER, GTEQ, LESS, NOTEQUAL, LTEQ, NOT, ID, TRUE, FALSE}
FIRST(arithmetic_operator) = {PLUS, MINUS, MULT, DIV}
FIRST(binary_boolean_operator) = {AND, OR, XOR}
FIRST(relational_operator) = {GREATER, GTEQ, LESS, NOTEQUAL, LTEQ}
FIRST(primary) = {ID, NUM, REALNUM, STRING_CONSTANT, TRUE, FALSE}
FIRST(arithmetic_primary) = {ID, NUM, REALNUM, STRING_CONSTANT}
FIRST(boolean_primary) = {ID, TRUE, FALSE}
FIRST(bool_const) = {TRUE, FALSE}
FIRST(condition) = {}



FOLLOW(program) = {$}
FOLLOW(scope) = {LBRACE, RBRACE ID, WHILE, $}
FOLLOW(scope_list) = {RBRACE, $}
FOLLOW(var_decl) = { LBRACE, ID, WHILE, $}
FOLLOW(id_list) = {COLON, $}
FOLLOW(type_name) = {SEMICOLON}
FOLLOW(stmt_list) = {RBRACE, $}
FOLLOW(stmt) = {ID, WHILE, $}
FOLLOW(assign_stmt) = {$}
FOLLOW(while_stmt) = {$}
FOLLOW(expr) = {SEMICOLON, $}
FOLLOW(arithmetic_expr) = {PLUS, MINUS, MULT, DIV, ID, NUM, REALNUM, STRING_CONSTANT, $}
FOLLOW(boolean_expr) = {AND, OR, XOR, GREATER, GTEQ, LESS, NOTEQUAL, LTEQ, NOT, ID, TRUE, FALSE, $}
FOLLOW(arithmetic_operator) = {PLUS, MINUS, MULT, DIV, ID, NUM, REALNUM, STRING_CONSTANT}
FOLLOW(binary_boolean_operator) = {AND, OR, XOR, GREATER, GTEQ, LESS, NOTEQUAL, LTEQ, NOT, ID, TRUE, FALSE}
FOLLOW(relational_operator) = {PLUS, MINUS, MULT, DIV, ID, NUM, REALNUM, STRING_CONSTANT,
AND, OR, XOR, GREATER, GTEQ, LESS, NOTEQUAL, LTEQ, NOT, ID, TRUE, FALSE}
FOLLOW(primary) = {}
FOLLOW(arithmetic_primary) = {$}
FOLLOW(boolean_primary) = {$}
FOLLOW(bool_const) = {$}
FOLLOW(condition) = {ID, WHILE}
*/



void Parser::program()
{

    uninitialized->next = NULL;
    type_eval->next=NULL;
    var_types->next= NULL;
    errorCode->next = NULL;

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type !=  LBRACE)
    {
        SyntaxError();
cout<<"Syntax Error 1"<<endl; 


    }

    string Level = "GLOBAL";
    scope(Level);
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type !=  END_OF_FILE)
    {
        SyntaxError();
cout<<"Syntax Error 2"<<endl;

    }

    

}

void Parser::scope(string Level)
{
    Token t = lexer.GetToken();
    if(t.token_type != LBRACE)
    {
        SyntaxError();
cout<<"Syntax Error 3"<<endl;

    }
    scope_list(Level);
    Token t1 = lexer.GetToken();
    if(t1.token_type != RBRACE)
    {
        SyntaxError();
cout<<"Syntax Error 4"<<endl;

    }

    ///////////////////////////


    //////////////////////////



}
///error must fix
void Parser::scope_list(string Level)
{
    Token t = lexer.GetToken();
    Token k = lexer.GetToken();
    lexer.UngetToken(k);
    lexer.UngetToken(t);
    if(t.token_type != LBRACE && t.token_type != ID && t.token_type != WHILE)
    {
        SyntaxError();
cout<<"Syntax Error 5"<<endl;

    }



    if(t.token_type == LBRACE)
    {
        //scope locations where they are increased
        //at lower level and decreased after exiting
        //scope
        Level = "alpha" + to_string(version);
        ScopeLevelCounter++;
        scope(Level);
        ScopeLevelCounter--;
        version++;

        Token j = lexer.GetToken();
        lexer.UngetToken(j);   
        if(j.token_type != RBRACE )
        {
            scope_list(Level);
        }
    }
    else if(t.token_type == ID && (k.token_type == COMMA || k.token_type == COLON))
    {
        var_decl(Level);
        Token j = lexer.GetToken();
        lexer.UngetToken(j);   
        if(j.token_type != RBRACE )
        {
            scope_list(Level);
        }
    }
    else if(t.token_type == ID || t.token_type == WHILE)
    {
        stmt(Level);
        Token j = lexer.GetToken();
        lexer.UngetToken(j);   
        if(j.token_type != RBRACE )
        {
            scope_list(Level);
        }
    }



}





void Parser::var_decl(string Level)
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != ID)
    {
        SyntaxError();
cout<<"Syntax Error 6"<<endl;

    }

    Variables* tempV1 = new Variables();
    Variables* tempV = var_types;

    while(tempV->next != NULL)
    {
        tempV = tempV->next;
    }

    vector<string> bar;
    id_list(tempV1, Level, bar);

    Token t1 = lexer.GetToken();
    if(t1.token_type != COLON)
    {
        SyntaxError();
cout<<"Syntax Error 7"<<endl;

    }
    type_name(tempV1);
    tempV1->LevelName = Level;
    tempV1->next = NULL;
    tempV->next = tempV1;

    Token t2 = lexer.GetToken();
    if(t2.token_type != SEMICOLON)
    {
        SyntaxError();
cout<<"Syntax Error 8"<<endl;

    }
}

void Parser::id_list(struct Variables *tempV1, string Level, vector<string> bar)
{
    
    Token t = lexer.GetToken();
    mem[SymbolTable[t.lexeme]] == 0;

    for(auto k : bar)
    {
        if(k == t.lexeme && printh ==true)
        {
            printh = false;
            Error* temp = errorCode;
            Error* temp1 = new Error();

            while(temp->next != NULL)
            {
                temp = temp->next;
            }

                //error 1.2
                temp1->error_code = "1.1";
                temp1->symbol_name = t.lexeme;
                temp1->priority = ScopeLevelCounter;
                temp1->next = NULL;
                temp->next = temp1; 
            


        }
    }

    bar.push_back(t.lexeme);
//////////////////////////////
    if(t.lexeme == "REAL" || t.lexeme == "INT" || t.lexeme == "BOOLEAN" || t.lexeme == "STRING")
    {
        cout<<"Syntax Error &!#@"<<endl;
        exit(0);

    }
   

    //////////////////
    //cout<<"break"<<endl;
    bool eval = false;
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
                //cout<<k.first.lexeme<<" "<<temp00->ScopeLevel<<" "<<ScopeLevelCounter<<endl;

                if(k.first.lexeme == t.lexeme && temp00->ScopeLevel == ScopeLevelCounter)
                {
                    //cout<<k.first.lexeme<<" "<<temp00->ScopeLevel<<" "<<ScopeLevelCounter<<endl;

                    eval = true;
                }

            }

            //if(temp00->ScopeLevel > ScopeLevelCounter)
            //{
            //    eval = false;
            //}

        }
       temp00 = temp00->next;
    }
////////////////////////////////////////





    Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if(eval == true && printh == true)
    {
        printh = false;
        //error 1.2
        temp1->error_code = "1.1";
        temp1->symbol_name = t.lexeme;
        temp1->priority = ScopeLevelCounter;
        temp1->next = NULL;
        temp->next = temp1;
        
    }





////////////////////


    //memory declaration for variables
    SymbolTable[t.lexeme] = next_available;
    next_available++;

    if(t.token_type != ID)
    {
        SyntaxError();
cout<<"Syntax Error 9"<<endl;

    }

    tempV1->variable.push_back(make_pair(t,false));


    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type == COMMA)
    {
        lexer.GetToken();
        id_list(tempV1, Level, bar);
    }

}

void Parser::type_name(struct Variables *tempV1)
{
    Token t = lexer.GetToken();
    if(t.token_type != REAL && t.token_type != INT && t.token_type != BOOLEAN && t.token_type != STRING)
    {
        SyntaxError();
cout<<"Syntax Error 10"<<endl;

    }
    tempV1->type = t;
    tempV1->ScopeLevel = ScopeLevelCounter;

    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != SEMICOLON)
    {
        SyntaxError();
cout<<"Syntax Error 11"<<endl;

    }
}

void Parser::stmt_list(string Level)
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != ID && t.token_type != WHILE)
    {
        SyntaxError();
cout<<"Syntax Error 12"<<endl;

    }
    stmt(Level);
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type == ID || t1.token_type == WHILE)
    {
        stmt_list(Level);
    }
}

void Parser::stmt(string Level)
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != ID && t.token_type != WHILE)
    {
        SyntaxError();
cout<<"Syntax Error 13"<<endl;

    }
    if(t.token_type == ID)
    {
        assign_stmt(Level);


    }
    else if(t.token_type == WHILE)
    {
        while_stmt(Level);
    }
}

void Parser::assign_stmt(string Level)
{
    Token t = lexer.GetToken();
    //mem[SymbolTable[t.lexeme]] = 1;


    ////////////////////////////

    //for the equal value not declaration

        Variables* temp008 = var_types;

        while(temp008 != NULL)
        {
            vector<pair<Token,bool>> pushIt;

                for(auto k: temp008->variable)
                {
                    //vector<pair(string,>

                    if(k.first.lexeme == t.lexeme && temp008->ScopeLevel <= ScopeLevelCounter && globalWhile == 0)
                    {
                        k.second = true;
    
                    }

                    pushIt.push_back(k);

                }

            temp008->variable = pushIt;
            
            temp008 = temp008->next;
        }










    /////////////////////////
    bool eval = false;
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
                if(k.first.lexeme == t.lexeme && temp00->ScopeLevel <= ScopeLevelCounter)
                {
                    eval = true;
                    
                }

            }

            

        }
       temp00 = temp00->next;
    }


    Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if(eval == false && printh == true)
    {
        printh = false;
        //error 1.2
        temp1->error_code = "1.2";
        temp1->symbol_name = t.lexeme;
        temp1->priority = ScopeLevelCounter;
        temp1->next = NULL;
        temp->next = temp1; 
       
    }
    
    

    Token t1 = lexer.GetToken();
    if(t.token_type != ID && t1.token_type != EQUAL)
    {
       SyntaxError();
cout<<"Syntax Error 14"<<endl;

    }



    expr(t, Level);
    Token t2 = lexer.GetToken();
    if(t2.token_type != SEMICOLON)
    {
        SyntaxError();
cout<<"Syntax Error 15"<<endl;

    }
}


void Parser::while_stmt(string Level)
{
    Token t = lexer.GetToken();
    Token t1 = lexer.GetToken();
    if(t.token_type != WHILE && t1.token_type != LPAREN)
    {
       SyntaxError();
cout<<"Syntax Error 16"<<endl;

    }
        condition();

    
    Token t2 = lexer.GetToken();
    Token t3 = lexer.GetToken();
    lexer.UngetToken(t3);
    if(t2.token_type != RPAREN)
    {
         SyntaxError();
         cout<<"Syntax Error 17"<<endl;

    }

    if(t3.token_type == LBRACE)
    {
        lexer.GetToken();

        globalWhile++;
        stmt_list(Level);
        globalWhile--;
        Token t4 =lexer.GetToken();
        if(t4.token_type != RBRACE)
        {
            SyntaxError();
            cout<<"Syntax Error 18"<<endl;
        }

    }
    else
    {
        globalWhile++;
        stmt(Level);
        globalWhile--;
    }

              

}


void Parser::expr(Token left_variable, string Level)
{

    Token t = lexer.GetToken();

    lexer.UngetToken(t);
    if(t.token_type != PLUS && t.token_type != MINUS && t.token_type != MULT && 
    t.token_type != DIV && t.token_type != ID && t.token_type != NUM && t.token_type 
    != REALNUM && t.token_type != STRING_CONSTANT && t.token_type != AND && t.token_type != XOR && t.token_type != GTEQ && 
    t.token_type != LESS && t.token_type != NOTEQUAL && t.token_type != LTEQ && t.token_type 
    != NOT && t.token_type != ID && t.token_type !=TRUE && t.token_type != FALSE)
    {
       SyntaxError();
cout<<"Syntax Error 19"<<endl;

    }
    string left_variable1 = left_variable.lexeme;
    //need remainder of grammar by professor
    if(t.token_type == AND || t.token_type == XOR || t.token_type == GREATER||t.token_type == GTEQ || 
    t.token_type == LESS || t.token_type == NOTEQUAL || t.token_type == LTEQ || t.token_type 
    == NOT)
    {
               // cout<<"1"<<endl;

        boolean_expr(left_variable.lexeme);
    }
    else if(t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || 
    t.token_type == DIV)
    {
                //cout<<"2"<<endl;

        arithmetic_expr(left_variable.lexeme);

    }
    else if(t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || 
    t.token_type == STRING_CONSTANT || t.token_type == TRUE || t.token_type == FALSE)
    {
             //   cout<<"3"<<endl;

        primary(Level, left_variable);
    }

    //cout<<left_variable.lexeme<<" "<<mem[SymbolTable[left_variable.lexeme]]<<endl;;

}

void Parser::arithmetic_expr(string left_variable)
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != PLUS && t.token_type != MINUS && t.token_type != MULT && 
    t.token_type != DIV && t.token_type != ID && t.token_type != NUM && t.token_type 
    != REALNUM && t.token_type != STRING_CONSTANT)
    {
       SyntaxError();
cout<<"Syntax Error 20"<<endl;

    }

        


    if(t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || 
    t.token_type == DIV)
    {
        arithmetic_operator();
        arithmetic_expr(left_variable);  //variable  //variable
        arithmetic_expr(left_variable);  //variable
        

    }


    else if(t.token_type != ID || t.token_type != NUM || t.token_type != REALNUM || 
    t.token_type != STRING_CONSTANT)
    {
        arithmetic_primary(left_variable);
        
      
    }
    else
    {
        SyntaxError();
cout<<"Syntax Error 21"<<endl;

    }

    

}

void Parser::boolean_expr(string left_variable)
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != AND && t.token_type != XOR && t.token_type != GTEQ && t.token_type != GREATER &&
    t.token_type != LESS && t.token_type != NOTEQUAL && t.token_type != LTEQ && t.token_type 
    != NOT && t.token_type != ID && t.token_type !=TRUE && t.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 22"<<endl;

    }



    if(t.token_type == AND || t.token_type == OR || t.token_type == XOR)
    {

        //data structure with errors below to be added
        binary_boolean_operator();
        boolean_expr(left_variable);
        boolean_expr(left_variable);
    }
    else if(t.token_type == GREATER || t.token_type == GTEQ || t.token_type == LESS || 
    t.token_type == NOTEQUAL || t.token_type == LTEQ)
    {
        Token h = lexer.GetToken();
        Token j = lexer.GetToken();
        lexer.UngetToken(j);
        lexer.UngetToken(h);

        if(j.token_type == PLUS || j.token_type == MINUS || j.token_type == MULT || 
    j.token_type == DIV || j.token_type == ID || j.token_type == NUM || j.token_type 
    == REALNUM || j.token_type == STRING_CONSTANT)
        {
            relational_operator();
            arithmetic_expr(left_variable);
            arithmetic_expr(left_variable);
        }//can go both ways
        else if(j.token_type == AND || j.token_type == XOR || j.token_type == GTEQ || 
    j.token_type == LESS || j.token_type == NOTEQUAL || j.token_type == LTEQ || j.token_type 
    == NOT || j.token_type == ID || j.token_type ==TRUE || j.token_type == FALSE)
        {
            relational_operator();
            boolean_expr(left_variable);
            boolean_expr(left_variable);
        }
        else
        {
           SyntaxError();
cout<<"Syntax Error 23"<<endl;

        }
    }
    else if(t.token_type == NOT)
    {
        lexer.GetToken();
        boolean_expr(left_variable);
    }
    else if(t.token_type == ID || t.token_type == TRUE || t.token_type == FALSE)
    {
        boolean_primary();
    }
}

void Parser::arithmetic_operator()
{
    Token t = lexer.GetToken();
    if(t.token_type != PLUS && t.token_type != MINUS && t.token_type != MULT && 
    t.token_type != DIV)
    {
        SyntaxError();
cout<<"Syntax Error 24"<<endl;

    }
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != PLUS && t1.token_type != MINUS && t1.token_type != MULT && 
    t1.token_type != DIV && t1.token_type != ID && t1.token_type != NUM && t1.token_type 
    != REALNUM && t1.token_type != STRING_CONSTANT)
    {
        SyntaxError();
cout<<"Syntax Error 25"<<endl;

    }
}

void Parser::binary_boolean_operator()
{
    Token t = lexer.GetToken();
    if(t.token_type != AND && t.token_type != OR && t.token_type != XOR)
    {
        SyntaxError();
cout<<"Syntax Error 26"<<endl;

    }
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != AND && t1.token_type != OR && t1.token_type != XOR && t1.token_type != 
    GREATER && t1.token_type != GTEQ && t1.token_type != LESS && t1.token_type != NOTEQUAL && 
    t1.token_type != LTEQ && t1.token_type != NOT && t1.token_type != ID && t1.token_type != TRUE 
    && t1.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 27"<<endl;

    }
}

void Parser::relational_operator()
{
    

    Token t = lexer.GetToken();
    if(t.token_type != GREATER && t.token_type != GTEQ && t.token_type != LESS && 
    t.token_type != NOTEQUAL && t.token_type != LTEQ)
    {
        SyntaxError();
cout<<"Syntax Error 28"<<endl;

    }
    Token t1 = lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != PLUS && t1.token_type != MINUS && t1.token_type != MULT && t1.token_type != 
    DIV && t1.token_type != ID && t1.token_type != NUM && t1.token_type != REALNUM && 
    t1.token_type != STRING_CONSTANT && t1.token_type != AND && t1.token_type != OR && t1.token_type != XOR 
    && t1.token_type != GREATER && t1.token_type != GTEQ && t1.token_type != LESS && t1.token_type != 
    NOTEQUAL && t1.token_type != LTEQ && t1.token_type != NOT && t1.token_type != ID && t1.token_type != 
    TRUE && t1.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 29"<<endl;

    }

    
}
//is not reachable due to incomplete grammar by professor
void Parser::primary(string Level, Token left_variable)
{    Token t = lexer.GetToken();


    /*
    TYPE* temp00 = type_eval;
    TYPE* temp01 = new TYPE();

    while(temp00->next != NULL)
    {
        temp00=temp00->next;
    }
    //error of types
   
    pair<bool, string> test;

    Variables* enter = var_types;
    Token A, B;

    while(enter != NULL)
    {
        if(!enter->type.lexeme.empty())
        {
            for(auto k: enter->variable)
            {
                if(left_variable.lexeme == k.first.lexeme)
                {
                    A = enter->type;
                }

                if(t.lexeme == k.first.lexeme)
                {
                    B = enter->type;
                }
            }
           
        }
       enter = enter->next;

    }


    test = name(A,B);

    //stores token data for mismatch errors
    Variables* temp00s = var_types;
    int n1 = 0;
    int n2 = 0;
    bool h= false;
    bool l=false;
    while(temp00s != NULL)
    {
        if(!temp00s->type.lexeme.empty())
        {
            for(auto k: temp00s->variable)
            {
                if(k.first.token_type == left_variable.token_type)
                {
                    h= true;

                }

                if(k.first.token_type == t.token_type)
                {
                    //n2 = k.ScopeLevel;
                    l=true;
   
                }
                
            }
            
        }

       
            if(h == true)
            {
                h=false;
                n1 = temp00s->ScopeLevel;

            }
            else if(l == true)
            {
                l=false;

                n2 = temp00s->ScopeLevel;

            }


       temp00s = temp00s->next;

    }
if(n1 >= n2)
{

    if(test.first)
    {
        temp01->lineNo = left_variable.line_no;
        temp01->errorType = test.second;
        temp01->next = NULL;
        temp00->next = temp01;

    }
}   
    */

    if(t.token_type != ID && t.token_type != NUM && t.token_type != REALNUM && 
    t.token_type != STRING_CONSTANT && t.token_type != TRUE && t.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 30"<<endl;

    }

    if(t.lexeme == "REAL" || t.lexeme == "INT" || t.lexeme == "BOOLEAN" || t.lexeme == "STRING")
    {
        cout<<"Syntax Error &!#@"<<endl;
        exit(0);

    }
  


    if(t.token_type == ID)
    {

        //////////////////////
        Variables* temp00a = var_types;
        while(temp00a != NULL)
        {
                    vector<pair<Token,bool>> pushIt;

                for(auto k: temp00a->variable)
                {
                    if(k.first.lexeme == t.lexeme && temp00a->ScopeLevel <= ScopeLevelCounter && globalWhile == 0)
                    {
                        k.second = true;

                        //////////////////////likely to be deleted
                        mem[SymbolTable[left_variable.lexeme]] = 1;


                        UNINITIALIZED* temp1L = new UNINITIALIZED();
                        UNINITIALIZED* tempL = uninitialized;


                        while(tempL->next != NULL)
                        {
                            tempL = tempL->next;
                        }


                        if(mem[SymbolTable[t.lexeme]] == 0)
                        {
                            temp1L->uninitialized_name = t.lexeme;
                            temp1L->uninitialized_line_no = t.line_no;
                            temp1L->next = NULL;
                            tempL->next = temp1L;

                        }
                        ////////////////////////


                            
                    }
                   
                            pushIt.push_back(k);


                }
                        temp00a->variable = pushIt;

        temp00a = temp00a->next;

        }

        ////////////////////////
         




        //used variable
         bool eval = false;
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
                if(k.first.lexeme == t.lexeme && temp00->ScopeLevel <= ScopeLevelCounter)
                {
                    eval = true;
                    
                }


            }

        }
       temp00 = temp00->next;
    }


Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if(eval == false && printh == true)
    {
        printh = false;
        //error 1.2
        temp1->error_code = "1.2";
        temp1->symbol_name = t.lexeme;
        temp1->priority = prio;
        temp1->next = NULL;
        temp->next = temp1; 
       
    }



        
       
        

    }

    

    if(t.token_type == TRUE || t.token_type == FALSE)
    {
        lexer.UngetToken(t);
        bool_const();
    }
}





void Parser::arithmetic_primary(string left_variable)
{
    Token t = lexer.GetToken();
    if(t.token_type != ID && t.token_type != NUM && t.token_type != REALNUM && 
    t.token_type != STRING_CONSTANT)
    {
       SyntaxError();
cout<<"Syntax Error 31"<<endl;

    }
        
/*
    Variables* temp00a = var_types;
        while(temp00a != NULL)
        {

                for(auto k: temp00a->variable)
                {
                    if(k.first.lexeme == left_variable && temp00a->ScopeLevel <= ScopeLevelCounter)
                    {
                       
                            mem[SymbolTable[left_variable]] = 1;
                    }

                }

        temp00a = temp00a->next;

        }

*/

    if(t.lexeme == "REAL" || t.lexeme == "INT" || t.lexeme == "BOOLEAN" || t.lexeme == "STRING")
    {
        cout<<"Syntax Error &!#@"<<endl;
        exit(0);

    }


    if(t.token_type != REAL || t.token_type != INT || t.token_type != STRING)
    {
        //cout<<"TYPE MISMATCH "<<t.line_no<<" C4"<<endl;
    }

    if(t.token_type == ID)
    {

        
        Variables* temp00a = var_types;
        while(temp00a != NULL)
        {
                    vector<pair<Token,bool>> pushIt;

                for(auto k: temp00a->variable)
                {
                    if(k.first.lexeme == t.lexeme && temp00a->ScopeLevel <= ScopeLevelCounter && globalWhile == 0)
                    {
                        k.second = true;

                        //////////////////////likely to be deleted


                        UNINITIALIZED* temp1L = new UNINITIALIZED();
                        UNINITIALIZED* tempL = uninitialized;


                        while(tempL->next != NULL)
                        {
                            tempL = tempL->next;
                        }


                        if(mem[SymbolTable[t.lexeme]] == 0)
                        {
                            temp1L->uninitialized_name = t.lexeme;
                            temp1L->uninitialized_line_no = t.line_no;
                            temp1L->next = NULL;
                            tempL->next = temp1L;

                        }
                        ////////////////////////

                            
                    }
                            pushIt.push_back(k);


                }
                        temp00a->variable = pushIt;

        temp00a = temp00a->next;

        }


        

//////////////////////
 

               bool eval = false;
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
                if(k.first.lexeme == t.lexeme && temp00->ScopeLevel <= ScopeLevelCounter)
                {
                    eval = true;
                    
                }


            }

        }
       temp00 = temp00->next;
    }


Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if(eval == false && printh == true)
    {
        printh = false;
        //error 1.2
        temp1->error_code = "1.2";
        temp1->symbol_name = t.lexeme;
        temp1->priority = prio;
        temp1->next = NULL;
        temp->next = temp1; 
       
    }




////////////////////////        

        /*//likely to be deleted
        UNINITIALIZED* temp1 = new UNINITIALIZED();
        UNINITIALIZED* temp = uninitialized;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        if(mem[SymbolTable[t.lexeme]].empty())
        {   
            temp1->uninitialized_name = t.lexeme;
            temp1->uninitialized_line_no = t.line_no;
            temp1->next = NULL;
            temp->next = temp1;
        }
        /*//////////////////////////////////////



    }

}

void Parser::boolean_primary()
{
    Token t = lexer.GetToken();
    if(t.token_type != ID && t.token_type != TRUE && t.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 32"<<endl;

    }


    if(t.lexeme == "REAL" || t.lexeme == "INT" || t.lexeme == "BOOLEAN" || t.lexeme == "STRING")
    {
        cout<<"Syntax Error &!#@"<<endl;
        exit(0);

    }






    
    if(t.token_type == ID)
    {
        

        Variables* temp00a = var_types;
        while(temp00a != NULL)
        {
                    vector<pair<Token,bool>> pushIt;

                for(auto k: temp00a->variable)
                {
               
                    if(k.first.lexeme == t.lexeme && temp00a->ScopeLevel <= ScopeLevelCounter && globalWhile == 0)
                    {
                        k.second = true;

                         //////////////////////likely to be deleted

                        UNINITIALIZED* temp1L = new UNINITIALIZED();
                        UNINITIALIZED* tempL = uninitialized;


                        while(tempL->next != NULL)
                        {
                            tempL = tempL->next;
                        }


                        if(mem[SymbolTable[t.lexeme]] == 0)
                        {
                            temp1L->uninitialized_name = t.lexeme;
                            temp1L->uninitialized_line_no = t.line_no;
                            temp1L->next = NULL;
                            tempL->next = temp1L;

                        }
                        ////////////////////////




                            
                    }
                            pushIt.push_back(k);


                }
                    temp00a->variable = pushIt;

        temp00a = temp00a->next;

        }
        
        

    /////////////////////
    
           bool eval = false;
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty() && temp00->ScopeLevel >= 0)
        {
            for(auto k: temp00->variable)
            {
                
                if(k.first.lexeme == t.lexeme  && temp00->ScopeLevel <= ScopeLevelCounter)
                {
                    eval = true;
                    
                }


            }

        }
       temp00 = temp00->next;
    }


Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if(eval == false && printh == true)
    {
        printh = false;
        //error 1.2
        temp1->error_code = "1.2";
        temp1->symbol_name = t.lexeme;
        temp1->priority = prio;
        temp1->next = NULL;
        temp->next = temp1; 
       
    }




////////////////







        /*//likely to be removed
        UNINITIALIZED* temp1 = new UNINITIALIZED();
        UNINITIALIZED* temp = uninitialized;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        if(mem[SymbolTable[t.lexeme]].empty())
        {
            temp1->uninitialized_name = t.lexeme;
            temp1->uninitialized_line_no = t.line_no;
            temp1->next = NULL;
            temp->next = temp1;

        }
        */

    }



    if(t.token_type == TRUE || t.token_type == FALSE)
    {
        lexer.UngetToken(t);
        bool_const();
    }
}

void Parser::bool_const()
{
    Token t = lexer.GetToken();
    if(t.token_type != TRUE && t.token_type != FALSE)
    {
        return;
    }
}

void Parser::condition()
{
    //needs remainder of grammar by professor
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != AND && t.token_type != XOR && t.token_type != GTEQ && t.token_type != GREATER &&
    t.token_type != LESS && t.token_type != NOTEQUAL && t.token_type != LTEQ && t.token_type 
    != NOT && t.token_type != ID && t.token_type !=TRUE && t.token_type != FALSE)
    {
        SyntaxError();
cout<<"Syntax Error 33"<<endl;
    }

    boolean_expr("");

}

void Parser::SyntaxError()
{
    cout<<"Syntax Error &!#@"<<endl;
    exit(0);
}

pair<bool,string> Parser::name(Token left_variable, Token t)
{
    
    if((left_variable.token_type == BOOLEAN && t.token_type != BOOLEAN) || 
    (left_variable.token_type == STRING && !(t.token_type == STRING)))
    {
        return make_pair(true, "C1");
    }
    else if(left_variable.token_type == INT && !(t.token_type == INT || t.token_type == BOOLEAN))
    {
        return make_pair(true,"C2");
    }
    else if(left_variable.token_type == REAL && !(t.token_type == INT || t.token_type == REAL))
    {
        return make_pair(true, "C3");
    }
    else if((left_variable.token_type == BOOLEAN && t.token_type == BOOLEAN) || 
    (left_variable.token_type == STRING && t.token_type == STRING) || 
    (left_variable.token_type == INT && (t.token_type == INT || t.token_type == BOOLEAN)) || 
    (left_variable.token_type == REAL && (t.token_type == INT || t.token_type == REAL)))
    {
        return make_pair(false,"");
    }
    else
    {
        return make_pair(true,"M1");
    }

}


int main()
{
    /*
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

    */

    Parser test;
    test.program();




    ///////////////
    Error* temp = errorCode;
    Error* temp1 = new Error();

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
               // cout<<k.first.lexeme<<" "<<k.second<<endl;
               if(k.second == false && printh == true)
               {
                   printh = false;
                    temp1->error_code = "1.3";
                    temp1->symbol_name = k.first.lexeme;
                    temp1->priority = ScopeLevelCounter;
                    temp1->next = NULL;
                    temp->next = temp1; 
               }

            }
        }
       temp00 = temp00->next;

    }


    //////////////


    
    Error* temp00A = errorCode;

        while(temp00A != NULL)
        {    
            if(!temp00A->error_code.empty() && !temp00A->symbol_name.empty())
            { 
                cout<<"ERROR CODE "<<temp00A->error_code<<" ";
                cout<<temp00A->symbol_name<<endl;
            }
        
        temp00A = temp00A->next;

        }
    

/*
    Variables* temp00 = var_types;
    while(temp00 != NULL)
    {
        if(!temp00->type.lexeme.empty())
        {
            for(auto k: temp00->variable)
            {
                cout<<k.lexeme<<" ";
            }
            cout<<" : "<<temp00->type.lexeme<<" "<<temp00->ScopeLevel<<" "<<temp00->LevelName<<endl;
        }
       temp00 = temp00->next;

    }

*/


/*
    TYPE* temp0 = type_eval;
    while(temp0 != NULL)
    {
       
        if(!temp0->errorType.empty())
        {
            cout<<"TYPE MISMATCH "<<temp0->lineNo<<" ";
            cout<<temp0->errorType<<endl;
        }

       temp0 = temp0->next;

    }
*/
    
    if(errorCode == NULL || errorCode->next == NULL)
    {

        UNINITIALIZED *temp7 = uninitialized;

        while(temp7 != NULL)
        {
            if(!temp7->uninitialized_name.empty())
            {
                cout<<"UNINITIALIZED "<<temp7->uninitialized_name<<" ";
                cout<<temp7->uninitialized_line_no<<endl;
            }
            temp7 = temp7->next;

        }
    }



}

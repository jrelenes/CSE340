#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include <iostream>
#include "parser.h"
#include <map>
#include "compiler.h"
using namespace std;


//indices of variables
map<string,int> symbolTable;

//FIRST(program)={ID}
//FIRST(var_section)={ID}
//FIRST(id_list)={ID}
//FIRST(body)={LBRACE}
//FIRST(stmt_list)={ID, WHILE, IF, SWITCH, FOR, output, input}
//FIRST(stmt)={ID, WHILE, IF, SWITCH, FOR, output, input}
//FIRST(assign_stmt)={ID}
//FIRST(expr)={ID, NUM}
//FIRST(primary)={ID, NUM}
//FIRST(op)={PLUS, MINUS, MULT, DIV}
//FIRST(output_stmt)={output}
//FIRST(input_stmt)={input}
//FIRST(while_stmt)={WHILE}
//FIRST(if_stmt)={IF}
//FIRST(condition)={ID, NUM}
//FIRST(relop)={GREATER, LESS, NOTEQUAL}
//FIRST(switch_stmt)={SWITCH}
//FIRST(for_stmt)={FOR}
//FIRST(case_list)={CASE}
//FIRST(case)={CASE}
//FIRST(default_case)={DEFAULT}
//FIRST(inputs)={NUM}
//FIRST(num_list)={NUM}


//FOLLOW(program)={$}
//FOLLOW(var_section)={LBRACE}
//FOLLOW(id_list)={SEMICOLON, $}
//FOLLOW(body)={NUM, $}
//FOLLOW(stmt_list)={$}
//FOLLOW(stmt)={ID, WHILE, IF, SWITCH, FOR, output, input, $}
//FOLLOW(assign_stmt)={ID, NUM, RPAREN, $}
//FOLLOW(expr)={SEMICOLON}
//FOLLOW(primary)={SEMICOLON, PLUS, MINUS, MULT, DIV, GREATER, LESS, NOTEQUAL, $}
//FOLLOW(op)={ID, NUM}
//FOLLOW(output_stmt)={$}
//FOLLOW(input_stmt)={$}
//FOLLOW(while_stmt)={$}
//FOLLOW(if_stmt)={$}
//FOLLOW(condition)={LBRACE, SEMICOLON}
//FOLLOW(relop)={ID, NUM}
//FOLLOW(switch_stmt)={$}
//FOLLOW(for_stmt)={$}
//FOLLOW(case_list)={RBRACE, DEFAULT, $}
//FOLLOW(case)={$, CASE}
//FOLLOW(default_case)={RBRACE}
//FOLLOW(inputs)={$}
//FOLLOW(num_list)={$}


struct InstructionNode * parse_generate_intermediate_representation()
{

    Parser initialize;

    struct InstructionNode* test;
    
     test = initialize.program();

    /*
    cout<<test->type<<endl;
    cout<<IN<<endl;
    cout<<test->next->type<<endl;
    cout<<ASSIGN<<endl;
    cout<<test->next->next->type<<endl;
    cout<<test->next->next->next->type<<endl;
    cout<<test->next->next->next->next->type<<endl;
    */
   
    return test;

};

 struct InstructionNode* Parser::program()
{
    InstructionNode * final;

    Token t= lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != ID)
    {
        cout<<"Syntax error 1"<<endl;
        return 0;
    }
    var_section();
    final = body();

    //throws a syntax error due to type error
    parse_inputs();
    Token k= lexer.GetToken();
    lexer.UngetToken(k);
    if(k.token_type != END_OF_FILE)
    {
        cout<<"Syntax error 2"<<endl;
        return 0;
    }

    /////////////
    struct InstructionNode* test = final;
    /*
    
    while(test->next != NULL)
    {   
        cout<<test->type<<endl;
        test = test->next;
    }
    
    /*
    cout<<test->type<<endl;
    cout<<test->next->type<<endl;
    cout<<test->next->next->type<<endl;
    cout<<test->next->next->next->type<<endl;
    cout<<test->next->next->next->output_inst.var_index<<endl;
    cout<<test->next->next->next->next->type<<endl;

    cout<<test->next->next->next->next->next->type<<endl;
    cout<<test->next->next->next->next->next->next->output_inst.var_index<<endl;
    cout<<test->next->next->next->next->next->next->next->type<<endl;

*/  

    //cout<<mem[test->next->next->next->next->cjmp_inst.operand1_index]<<" "<<"here"<<endl;
/*
    
    cout<<"first: INPUT"<<" "<<IN<<endl;
    cout<<"first: ASSIGN"<<" "<<ASSIGN<<endl;
    cout<<"first: CJMP"<<" "<<CJMP<<endl;
    cout<<"first: OUT"<<" "<<OUT<<endl;
    cout<<"first: JMP"<<" "<<JMP<<endl;
    cout<<"first: NOOP"<<" "<<NOOP<<endl;
*/

    ///////////

    
    return final;
    //end of error 29 & 2
}

int Parser::var_section()
{
    Token t= lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != ID)
    {
        cout<<"Syntax error 3"<<endl;
        return 0;
    }
    id_list();
    lexer.GetToken();


    Token k= lexer.GetToken();
    lexer.UngetToken(k);
    if(k.token_type != LBRACE)
    {
        cout<<"Syntax error 4"<<endl;
        return 0;
    }
}

//where the ID information in located
int Parser::id_list()
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != ID)
    {
        cout<<"Syntax error 5"<<endl;
        return 0;
    }
    Token id = lexer.GetToken();
    //cout<<id.lexeme<<" ";

    //declare memory for variable and intializes
    // memory location to zero

    symbolTable[id.lexeme] = next_available;
    mem[next_available] = 0;
    next_available++;


    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    if(t.token_type == COMMA)
    {
        lexer.GetToken();
        id_list();
    }


}
//believed to be complete
struct InstructionNode* Parser::body()
{
    struct InstructionNode* instl ;

    Token t= lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type != LBRACE)
    {
        cout<<"Syntax error 6"<<endl;
        return 0;
    }

    lexer.GetToken();
    instl = stmt_list();
    lexer.GetToken();

    return instl;

}

//need to append the data structures before returning
struct InstructionNode* Parser::stmt_list()
{
    struct InstructionNode *inst1;
    struct InstructionNode *inst2;

    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);

    //verify output & input term 
    if(t0.token_type != ID && t0.token_type != WHILE && t0.token_type !=
    IF && t0.token_type != SWITCH && t0.token_type != FOR && t0.token_type != OUTPUT
    && t0.token_type != INPUT)
    {
        cout<<"Syntax error 7"<<endl;
        return 0;
    }

    inst1 = stmt();
    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    //verify output & input term 
    if(t.token_type == ID || t.token_type == WHILE || t.token_type ==
    IF || t.token_type == SWITCH || t.token_type == FOR || t.token_type == OUTPUT
    || t.token_type == INPUT)
    {
        ///////////////////////////
        //need to append inst1 to inst
        inst2 = stmt_list();

        struct InstructionNode * temp = inst1;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next  = inst2;
        //////////////////////////
        return inst1;
    }
    else if(t.token_type == RBRACE)
    {
        //cout<<"single"<<endl;
        return inst1;
    }

}


//has not been worked on but struct return
//type declared
struct InstructionNode* Parser::stmt()
{
    InstructionNode * inst;

    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);

    //verify output & input term 
    if(t0.token_type != ID && t0.token_type != WHILE && t0.token_type != IF 
    && t0.token_type != SWITCH && t0.token_type != FOR && t0.token_type != OUTPUT 
    && t0.token_type != INPUT)
    {
        cout<<"Syntax error 8"<<endl;
        return 0;
    }




    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    //verify output & input term 
    if(t.token_type == ID)
    {
        //cout<<t.lexeme<<"here"<<endl;
        inst = assign_stmt();
        return inst;
    }
    else if(t.token_type == WHILE)
    {
        //not complete modification
        inst = while_stmt(); //need to be finalized otherwise error 

        return inst;

    }
    else if(t.token_type == IF)
    {

        //not complete modification
        inst = if_stmt(); //need to be finalized otherwise error

        return inst;

    }
    else if(t.token_type == SWITCH)
    {
        //cout<<t.lexeme<<endl;
        inst = switch_stmt();


        return inst;
    }
    else if(t.token_type == FOR)
    {
        //not complete modification
        inst = for_stmt(); //need to be finalized otherwise error
        return inst;
    }
    else if(t.token_type == OUTPUT)
    {
        inst = output_stmt();
        return inst;
    }
    else if(t.token_type == INPUT)
    {
        inst = input_stmt();
        return inst;
    }

   



}
////////////////////////////////////


//the data structure deed more design analysis
struct InstructionNode* Parser::assign_stmt()
{
    InstructionNode *assign = new InstructionNode();
    assign->type = ASSIGN;
    assign->next = NULL;

    Token t0= lexer.GetToken();
    Token t1= lexer.GetToken();
    lexer.UngetToken(t1);
    lexer.UngetToken(t0);
    if(t0.token_type != ID && t1.token_type != EQUAL)
    {
        cout<<"Syntax error 9"<<endl;
        return 0;
    }

    Token z = lexer.GetToken();
    assign->assign_inst.left_hand_side_index = symbolTable[z.lexeme];
    lexer.GetToken();
    Token t = lexer.GetToken();
    Token k = lexer.GetToken();
    lexer.UngetToken(k);
    lexer.UngetToken(t);

    if(k.token_type == PLUS || k.token_type == MINUS || k.token_type == MULT
    || k.token_type == DIV)
    {
        expr(assign);
    }
    else
    {
        
        assign->assign_inst.operand1_index = primary();
        assign->assign_inst.op = OPERATOR_NONE;
    }
    lexer.GetToken();

    return assign;
}

int Parser::expr(struct InstructionNode *assign)
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != ID && t0.token_type != NUM)
    {
        cout<<"Syntax error 10"<<endl;
        return 0;
    }

    assign->assign_inst.operand1_index = primary();
    assign->assign_inst.op = op();
    assign->assign_inst.operand2_index = primary();

    Token t1= lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != SEMICOLON)
    {
        cout<<"Syntax error 11"<<endl;
        return 0;
    }
}

int Parser::primary()
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != ID && t0.token_type != NUM)
    {
        cout<<"Syntax error 12"<<endl;
        return 0;
    }

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type == ID)
    {
       Token z = lexer.GetToken();
       return symbolTable[z.lexeme];

    }
    else if(t.token_type == NUM)
    {
        Token z2 = lexer.GetToken();

        //allocates memory to numbers without variables
        mem[next_available] = stoi(z2.lexeme);
        int location = next_available;

        //increments the memory counter
        next_available++;
        return location;
   
    }
////////////////////////////////////////////////////////////////////

}

ArithmeticOperatorType Parser::op()
{

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type == PLUS)
    {
        lexer.GetToken();
        return OPERATOR_PLUS;
    }
    else if(t.token_type == MINUS)
    {
        lexer.GetToken();
        return OPERATOR_MINUS;
    }
    else if (t.token_type == MULT)
    {
        lexer.GetToken();
        return OPERATOR_MULT;
    }
    else if(t.token_type == DIV)
    {
        lexer.GetToken();
        return OPERATOR_DIV;
    }
    else
    {
        cout<<"Syntax error 13"<<endl;
    }

    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != ID && t0.token_type != NUM)
    {
        cout<<"Syntax error 14"<<endl;
    }
}

struct InstructionNode* Parser::output_stmt()
{
    InstructionNode *output = new InstructionNode();
    output->type = OUT;
    output->next = NULL;

    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    //verify output term 
    if(t.token_type == OUTPUT)
    {
        //cout<<t.lexeme<<"second"<<endl;
        lexer.GetToken();
        Token z = lexer.GetToken();
        output->output_inst.var_index = symbolTable[z.lexeme];

        lexer.GetToken();
    }
    else
    {
        cout<<"Syntax error 15"<<endl;
    }

    return output;
}

struct InstructionNode* Parser::input_stmt()
{
    //create node
    InstructionNode *inputStruct = new InstructionNode();

    //assing struct type
    inputStruct->type = IN;
    inputStruct->next = NULL;

    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    //verify input term 
    if(t.token_type == INPUT)
    {
        //cout<<t.lexeme<<"zero"<<endl;
        lexer.GetToken();
        Token z =lexer.GetToken();

        //assign index of variable
        //need to pop values from the input vector and method
        inputStruct->input_inst.var_index = symbolTable[z.lexeme];

        lexer.GetToken();
    }
    else
    {
        cout<<"Syntax error 16"<<endl;
        return 0;
    }

    return inputStruct;
}

//needs jump conditions
struct InstructionNode* Parser::while_stmt()
{
    //need to finalize but mostly correct
    InstructionNode * inst = new InstructionNode();
    InstructionNode * empty = new InstructionNode();
    InstructionNode * jump = new InstructionNode();

    empty->type = NOOP;
    jump->type = JMP;
    inst->type = CJMP;

    empty->next = NULL;

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type == WHILE)
    {
        lexer.GetToken();
        condition(inst);
        inst->next = body();
        jump->jmp_inst.target = inst;

        struct InstructionNode* temp = inst;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = jump;
        temp->next->next = empty;
        jump->next = empty;
        
        inst->cjmp_inst.target = empty;


        return inst;

    }
    else
    {
        cout<<"Syntax error 17"<<endl;
    }

    
}

//needs cjmp and jmp instructions
struct InstructionNode* Parser:: if_stmt()
{
    InstructionNode* inst = new InstructionNode();
    InstructionNode* empty = new InstructionNode();
    empty->type = NOOP;
    empty->next = NULL; 

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    if(t.token_type == IF)
    {
        inst->type = CJMP;
        lexer.GetToken();
        condition(inst);
        inst->next = body(); 
        inst->cjmp_inst.target = empty;

        struct InstructionNode* temp = inst;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = empty;

    }
    else
    {
        cout<<"Syntax error 18"<<endl;
    }

    return inst;

}

int Parser::condition(struct InstructionNode* inst)
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != ID && t0.token_type != NUM)
    {
        cout<<"Syntax error 19"<<endl;
        return 0;
    }

    inst->cjmp_inst.operand1_index = primary();

    relop(inst);
    inst->cjmp_inst.operand2_index = primary();


    Token t1= lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != LBRACE && t1.token_type != SEMICOLON)
    {
        cout<<"Syntax error 20"<<endl;
        return 0;
    }
}

int Parser::relop(struct InstructionNode* inst)
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != GREATER && t0.token_type != LESS && t0.token_type != NOTEQUAL)
    {
        cout<<"Syntax error 21"<<endl;
        return 0;
    }

    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    if(t.token_type == GREATER)
    {
        inst->cjmp_inst.condition_op = CONDITION_GREATER;
        lexer.GetToken();
        
    }
    else if(t.token_type == LESS)
    {
        inst->cjmp_inst.condition_op = CONDITION_LESS;
        lexer.GetToken();

    }
    else if(t.token_type == NOTEQUAL)
    {
        inst->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
        lexer.GetToken();

    }

    Token t1= lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != ID && t1.token_type != NUM)
    {
        cout<<"Syntax error 22"<<endl;
        return 0;
    }
}

struct InstructionNode* Parser::switch_stmt()
{
    InstructionNode * inst; //node
    InstructionNode * empty = new InstructionNode();
    empty->type = NOOP;
    empty->next = NULL;


    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != SWITCH)
    {
        cout<<"Syntax error 23"<<endl;
        return 0;
    }

    lexer.GetToken();

    //ID
    Token z = lexer.GetToken();

    //cout<<"print"<<endl;

    lexer.GetToken();
    inst = case_list(z, empty);

    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    //cout<<"outside"<<endl;
    if(t.token_type == DEFAULT)
    {

        //cout<<CJMP<<endl;
        struct InstructionNode* temp = inst;
        while(temp->next != NULL)
        {
            //cout<<temp->type<<endl;
            temp = temp->next;
        }
        //concatenates default with noop
        temp->next = default_case(empty);

    }
    else
    {
        struct InstructionNode* temp = inst;
        while(temp->next != NULL)
        {
            //cout<<temp->type<<endl;
            temp = temp->next;
        }
        //concatenates default with noop
        temp->next = empty;
    }
    
    
    
    lexer.GetToken();

    return inst;   //return node
}

struct InstructionNode* Parser::for_stmt()
{
    struct InstructionNode* inst = new InstructionNode();
    struct InstructionNode* empty = new InstructionNode();
    struct InstructionNode* jump = new InstructionNode();
    
    empty->type = NOOP;
    jump->type = JMP;
    inst->type = CJMP;

    empty->next = NULL;


    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != FOR)
    {
        cout<<"Syntax error 24"<<endl;
    }

    lexer.GetToken();
    lexer.GetToken();
    struct InstructionNode* start;
    start = assign_stmt();
    start->next = inst;
    condition(inst);
    lexer.GetToken();

    struct InstructionNode* updateValues;
    updateValues = assign_stmt();

    lexer.GetToken();
    inst->next = body();
    jump->jmp_inst.target = inst;

    

    struct InstructionNode* temp = inst;

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = updateValues;
    temp->next->next = jump;
    temp->next->next->next = empty;
    jump->next = empty;

    inst->cjmp_inst.target = empty;

    return start;
}


struct InstructionNode* Parser::case_list(Token z, struct InstructionNode* empty)
{
    struct InstructionNode* inst1;
    struct InstructionNode* inst2;

    inst1 = case_(z, empty);
    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    if(t.token_type == CASE)
    {
        inst2 = case_list(z, empty);
        
        struct InstructionNode* temp = inst1;
        while(temp->next != NULL)
        {
            //cout<<temp->type<<endl;
            temp = temp->next;
        }
        temp->next = inst2;
        return inst1;
    }
    else
    {
        return inst1;

    }
    

}

//error is guranteed to be here
struct InstructionNode* Parser::case_(Token z, struct InstructionNode* exitNode)
{

    //format as an if statement otherwise wrong
    InstructionNode* inst= new InstructionNode();
    InstructionNode* empty = new InstructionNode();
    InstructionNode* jump = new InstructionNode();

    jump->type = JMP;
    jump->jmp_inst.target = exitNode ;
    jump->next = NULL;

    empty->type = NOOP;
    empty->next = NULL;

    inst->type = CJMP;
    inst->cjmp_inst.operand1_index = symbolTable[z.lexeme];


    inst->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != CASE)
    {
        cout<<"Syntax error 26"<<endl;
        return 0;
    }

    lexer.GetToken();
    //NUM type
    Token z1 = lexer.GetToken();
    mem[next_available] = stoi(z1.lexeme);
    inst->cjmp_inst.operand2_index = next_available;
    next_available++;
    
    /*
    NOOP Statement noop1;


    start of switch
    if statement if1
       body
       GOTO noop1
    Noop2
    if statement if2
       body
       GOTO noop1
    Noop3
    Noop1
    
    
    
    
    */


    //create jump to point to end
    lexer.GetToken();    
    
    
    struct InstructionNode* address = body();
    struct InstructionNode* temp0 = address;

    while(temp0->next != NULL)
    {
        //cout<<temp->type<<endl;
        temp0 = temp0->next;
    }

    temp0->next= jump;
    //inst->cjmp_inst.target = body();
    
    inst->next = temp0->next->next;
    inst->cjmp_inst.target = address;

    ///////////////////////////////////////////////////////////////
    struct InstructionNode* temp = inst;

    while(temp->next != NULL)
    {
        //cout<<temp->type<<endl;
        temp = temp->next;
    }

    temp->next = empty;

    return inst;
    
}

struct InstructionNode* Parser::default_case(struct InstructionNode* empty)
{
    InstructionNode * inst;
    
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != DEFAULT)
    {
        cout<<"Syntax error 27"<<endl;
        return 0;
    }

    //cout<<"default"<<endl;
    lexer.GetToken();
    lexer.GetToken();
    inst = body();

    struct InstructionNode* temp = inst;

    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    
    temp->next = empty;

    Token t1= lexer.GetToken();
    lexer.UngetToken(t1);
    if(t1.token_type != RBRACE)
    {
        cout<<"Syntax error 28"<<endl;
        return 0;
    }

    return inst;

}



int Parser::parse_inputs()
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != NUM)
    {
        cout<<"Syntax error 29"<<endl;
        return 0;
    }

    num_list();
}

//this is wehere input numbers are parsed
//for example, 1 2 3 4 5 used as input a(variable)
int Parser::num_list()
{
    Token t0= lexer.GetToken();
    lexer.UngetToken(t0);
    if(t0.token_type != NUM)
    {
        cout<<"Syntax error 30"<<endl;
        return 0;
    }

    Token input = lexer.GetToken();

    //pushes back inputs in order into a vector
    //and converts the string to int type for the list
    inputs.push_back(stoi(input.lexeme));

    Token t = lexer.GetToken();
    lexer.UngetToken(t);

    if(t.token_type == NUM)
    {
        num_list();
    }

}



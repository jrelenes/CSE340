/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

//UNDER CONSTRUCTION
int next_available = 0; //memory counter
symbolTableN* table = new symbolTableN(); // 
//*********************

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!!\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected typlse. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();

    
    if (t.token_type != expected_type)
        syntax_error();
    return t;

}



// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parseInput()
{
    cout<<"1"<<endl;
    parseProgram();
    parseInputs();
    expect(END_OF_FILE);
    cout<<"2"<<endl;
}

void Parser::parseProgram()
{
    cout<<"3"<<endl;
    parsePoly_decl_section();
    parseStart();
    cout<<"4"<<endl;
}
/////////////////////////////////////////////////////////
void Parser::parseInputs()
{
    cout<<"5"<<endl;

    //stores variable values
    Token t = peek();
    if(t.token_type != EOF)
    {
        variableInputs.push_back(atoi(t.lexeme));
    }
    
    //continues normal parsing
    
    expect(NUM);
    t = peek();
    if(t.token_type == NUM)
    {
        cout<<"5A"<<endl;
    parseInputs();
    }
    else
    {
    return;
    }  
    cout<<"6"<<endl;

    
}

void Parser::parsePoly_decl_section()
{
    cout<<"7"<<endl;
    parsePoly_decl();
    Token t = peek();
    if(t.token_type == POLY)
    {
        parsePoly_decl_section();
    }
    else
    {
        return;
    }
    cout<<"8"<<endl;
}

void Parser::parsePoly_decl()
{
    cout<<"9"<<endl;
        expect(POLY);
        cout<<"error011"<<endl;
        parsePolynomial_header();
        cout<<"error012"<<endl;
        expect(EQUAL);
        cout<<"error013"<<endl;
        parsePolynomial_body();
        expect(SEMICOLON);
        cout<<"10"<<endl;
}

void Parser::parsePolynomial_header()
{
    cout<<"11"<<endl;
    parsePolynomial_name();
    Token t = peek();
    if(t.token_type == LPAREN)
    {
        cout<<"error0"<<endl;
            t = lexer.GetToken();
            parseId_list();
            expect(RPAREN);
        cout<<"error1"<<endl;
    }
    else
    {
        cout<<"error0"<<endl;
        return;
    }
    cout<<"12"<<endl;
}

void Parser::parseId_list()
{
    cout<<"13"<<endl;
    expect(ID);
    Token t = peek();
    if(t.token_type == COMMA)
    {
        t = lexer.GetToken();
        parseId_list();
    }
    else
    {
        return;
    }
    cout<<"14"<<endl;
}

void Parser::parsePolynomial_name()
{
    cout<<"15"<<endl;
    expect(ID);
    cout<<"16"<<endl;
}

void Parser::parsePolynomial_body()
{
    cout<<"17"<<endl;
    parseTerm_list();
    cout<<"18"<<endl;
}

void Parser::parseTerm_list()
{
    cout<<"19"<<endl;
    parseTerm();
    Token t = peek();
    if(t.token_type == PLUS || t.token_type == MINUS)
    {
        parseAdd_operator();
        t = peek();
        if(t.token_type == NUM || t.token_type == ID)
        {
        parseTerm_list();
        }
        else
        {
            syntax_error();
        }
    }
    else
    {
        return;
    }
    cout<<"20"<<endl;
}

void Parser::parseTerm()
{
    cout<<"21"<<endl;
    Token t = peek();
    if(t.token_type == ID)
    {
        parseMonomial_list();
    }
    else if(t.token_type == NUM)
    {
        parseCoefficient();
        t = peek();
        if(t.token_type == ID)
        {
            parseMonomial_list();
        }
        else
        {
            return;
        }
    }
    else
    {
        syntax_error();
    }
    cout<<"22"<<endl;
}

void Parser::parseMonomial_list()
{
    cout<<"23"<<endl;
    parseMonomial();
    Token t = peek();
    if(t.token_type == ID)
    {
        parseMonomial_list();
    }
    else
    {
        return;
    }
    cout<<"24"<<endl;
}

void Parser::parseMonomial()
{
    cout<<"25"<<endl;
    expect(ID);
    cout<<"jj"<<endl;
    Token t = peek();
    if(t.token_type == POWER)
    {
        parseExponent();
    }
    else
    {
        return;
    }
    cout<<"26"<<endl;
}

void Parser::parseExponent()
{
    cout<<"27"<<endl;
    expect(POWER);
    expect(NUM);
    cout<<"28"<<endl;
}

void Parser::parseAdd_operator()
{
    cout<<"29"<<endl;
    Token t = peek();
    if(t.token_type == PLUS || t.token_type == MINUS)
    {
        t = lexer.GetToken();
    }
    else
    {
        syntax_error();
    }
    cout<<"30"<<endl;
}

void Parser::parseCoefficient()
{
    cout<<"31"<<endl;
    expect(NUM);
    cout<<"32"<<endl;
}

void Parser::parseStart()
{
    cout<<"33"<<endl;
    expect(START);
    parseStatement_list();
    cout<<"34"<<endl;
}

void Parser::parseStatement_list()
{
    cout<<"35"<<endl;
    parseStatement();
    Token t = peek();
    if(t.token_type == ID || t.token_type == INPUT)
    {
        parseStatement_list();
    }
    else
    {
        return;
    }
    cout<<"36"<<endl;
}

void Parser::parseStatement()
{
    cout<<"37"<<endl;
    Token t = peek();
    if(t.token_type == INPUT)
    {
        cout<<"37A"<<endl;
        parseInput_statement();
    }
    else if(t.token_type == ID)
    {
        cout<<"37B"<<endl;
        parsePoly_evaluation_statement();
    }
    else
    {
        cout<<"37C"<<endl;
        syntax_error();
    }
    cout<<"38"<<endl;
}

void Parser::parsePoly_evaluation_statement()
{
    cout<<"39"<<endl;
    parsePolynomial_evaluation();
    expect(SEMICOLON);
    cout<<"40"<<endl;
}

void Parser::parseInput_statement()
{
    cout<<"41"<<endl;
    expect(INPUT);

//variable memory allocation
    Token t = peek();

    bool variableExists = false;

    for(auto begin : symbolTable)
    {
        if(begin.variable == t.lexeme)
        {
            variableExists = true;
            break;
        }
    } 

    if(!variableExists)
    {

     table->variable = t.lexeme;
     table->tableIndex = next_available;
     symbolTable.push_back(*table);
     memory.push_back(0);
     next_available++;

    }

//continues normal parser operations
    expect(ID);
    expect(SEMICOLON);
    cout<<"42"<<endl;
}

void Parser::parsePolynomial_evaluation()
{
    cout<<"43"<<endl;
    parsePolynomial_name();
    expect(LPAREN);
    parseArgument_list();
    expect(RPAREN);
    cout<<"44"<<endl;
}

void Parser::parseArgument_list()
{
    cout<<"45"<<endl;
    parseArgument();
    Token t = peek();
    if(t.token_type == COMMA)
    {
        t = lexer.GetToken();
        parseArgument_list();           
    }
    else
    {
        return;
    }
    cout<<"46"<<endl;
}

void Parser::parseArgument()
{
    cout<<"47"<<endl;
    Token t = peek();
    if(t.token_type == NUM || t.token_type == ID)
    {
        t = lexer.GetToken();
    }
    else 
    {
        parsePolynomial_evaluation();
    }
    cout<<"48"<<endl;
}

int main()
{
    Parser parse;
    parse.parseInput();


}
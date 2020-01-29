/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek(); //get unget
  public:
    void parseInput();
    void parseProgram();
    void parseInputs();
    void parsePoly_decl_section();
    void parseStart();
    void parsePoly_decl();
    void parsePolynomial_header();
    void parsePolynomial_body();
    void parsePolynomial_name();
    void parseId_list();
    void parseTerm_list();
    void parseTerm();
    void parseAdd_operator();
    void parseMonomial_list();
    void parseCoefficient();
    void parseMonomial();
    void parseExponent();
    void parseStatement_list();
    void parseStatement();
    void parseInput_statement();
    void parsePoly_evaluation_statement();
    void parsePolynomial_evaluation();
    void parseArgument_list();
    void parseArgument();



};

#endif


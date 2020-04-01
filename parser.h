#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include <iostream>
#include "lexer.h"
#include <map>
using namespace std;

class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek(); //get unget
  public:

    struct InstructionNode* program();
    int var_section();
    int id_list();
    struct InstructionNode* body();
    struct InstructionNode* stmt_list();
    struct InstructionNode* stmt();
    struct InstructionNode* assign_stmt();
    int expr(struct InstructionNode*);
    int primary();
    ArithmeticOperatorType op();
    struct InstructionNode* output_stmt();
    struct InstructionNode* input_stmt();
    struct InstructionNode* while_stmt();
    struct InstructionNode* if_stmt();
    int condition(struct InstructionNode*);
    int relop(struct InstructionNode*);
    struct InstructionNode* switch_stmt();
    struct InstructionNode* for_stmt();
    struct InstructionNode* case_list(Token, struct InstructionNode*);
    struct InstructionNode* case_(Token, struct InstructionNode*);
    struct InstructionNode* default_case(struct InstructionNode*);
    int parse_inputs();
    int num_list();


};


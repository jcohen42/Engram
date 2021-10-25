/*
Jake Cohen
ASU Thesis
Engram Programming Language
*/

#include <vector>
#include <string>
#include <unordered_map>

#include "lexer.h"
#include "compiler.h"

using namespace std;

class Parser {
    public:
        Parser() {}

        void consumeAllInput();

        //Structs used for data representation
        struct operation;
        struct condition;
        struct if_block;
        struct function;
        struct findFunctionName;

        //Parsing functions
        struct InstructionNode* parse_start();
        void parse_function_section();
        struct function* parse_function();
        void parse_return_section();
        Token parse_variable();
        struct InstructionNode* parse_program_section();
        struct InstructionNode* parse_statement_list();
        struct InstructionNode* parse_statement();
        struct InstructionNode* parse_if_statement();
        struct if_block* parse_if_block();
        struct InstructionNode* parse_else_block();
        struct InstructionNode* parse_while_statement();
        struct condition* parse_condition();
        ConditionalOperator parse_relop();
        struct InstructionNode* parse_assign_statement();
        struct operation* parse_operation();
        ArithmeticOperator parse_operator();
        struct InstructionNode* parse_input_statement();
        struct InstructionNode* parse_output_statement();
        struct InstructionNode* parse_function_statement();
        struct InstructionNode* parse_function_statement_simple();
        struct InstructionNode* parse_function_statement_return();

    private:
        LexicalAnalyzer lexer;
        Token expect(TokenType expected_type);
        int location(string);

        //For memory allocation
        unordered_map<string, int> symbolTable;

        //Memory allocation within the scope of functions
        vector<function*> functionList;

        //Error functions
        void syntaxError(Token);
        void variableNotInitialized(Token);
        void functionIDMismatch(Token, Token);
        void functionRedeclaration(Token);
        void functionNotFound(Token);
};
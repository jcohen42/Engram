/*
Jake Cohen
ASU Thesis
Pure-ENG Programming Language
*/

#include <vector>
#include <string>

using namespace std;

extern int mem[1024];
extern int nextAvailable;
extern string fileName;

typedef enum {
    OPERATOR_NONE = 100,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_TIMES,
    OPERATOR_DIV,
    OPERATOR_MOD
} ArithmeticOperator;


typedef enum {
    CONDITION_GREATER = 150,
    CONDITION_LESS,
    CONDITION_NOTEQUAL,
    CONDITION_EQUAL
} ConditionalOperator;

typedef enum {
    NOOP = 200,
    IN,
    OUT,
    ASSIGN,
    CJMP,
    JMP
} InstructionType;

struct InstructionNode {
    InstructionNode() {}

    InstructionType type;

    union {
        struct {
            int leftHandSideIndex;
            int operand1Index;
            int operand2Index;

            ArithmeticOperator op;
        } assign_inst;

        struct {
            int varIndex;
        } input_inst;

        struct {
            int varIndex;
            bool outputIsStr;
            char* str;
        } output_inst;

        struct {
            ConditionalOperator conditionOp;
            int operand1Index;
            int operand2Index;

            struct InstructionNode* target; 
        } cjmp_inst;

        struct {
            struct InstructionNode* target;
        } jmp_inst;
    };

    struct InstructionNode* next;
};

struct InstructionNode * parseProgram();

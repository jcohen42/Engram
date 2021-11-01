/*
Jake Cohen
ASU Thesis
Engram Programming Language
*/

#include "compiler.h"

#include <iostream>
#include <istream>
#include <cctype>
#include <cstring>
#include <vector>

using namespace std;

int mem[1024];
int nextAvailable = 0;
int memIndex;
string fileName;
bool previousOutput = false;

//Check if a given string is a number
bool isNumber(string str) {
    int i = 0;
    if(str[i] == '-') i++; //Check for the negative sign
    for(i = i; i < (int)str.length(); i++) {
        if(!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

//ERROR FUNCTIONS
void divideBy0() {
    cout << "Error: Divide by 0.";
    exit(1);
}

void modBy0() {
    cout << "Error: Modulo by 0.";
    exit(1);
}

void integerOverflow() {
    cout << "Error: The integer you entered was too large for "
            << "the program to recognize. Exiting program.";
    exit(1);
}

//WARNING FUNCTIONS
void inputNotInteger() {
    cout << "Warning: Please make sure that you typed in a number, with no "
            << "letters or punctations marks. Please enter a new number:\n";
}

void executeProgram(struct InstructionNode* program) {
    struct InstructionNode* pc = program; //pc is the program counter
    // struct InstructionNode* returnNode;
    int op1, op2, result;
    string userInput;
    //Iterate through the program one node at a time
    while(pc != NULL) {
        //Check the type of the instruction node
        //We need to move the program counter if necessary
        switch(pc->type) {
            case NOOP: //Noop nodes are for utility
                pc = pc->next;
                break;
            case IN: //Input a number from the keyboard
                cout << endl; //so that the user can input on a blank line
                cin >> userInput;

                //Ensure that the user typed in a number rather than a word
                while(!isNumber(userInput)) {
                    //Get user input again
                    inputNotInteger();
                    cin >> userInput;
                }

                //Check for integer overflow
                try {
                    stoi(userInput);
                } catch(out_of_range& e) {
                    integerOverflow();
                }

                //Assign the numerical value to memory
                mem[pc->input_inst.varIndex] = stoi(userInput);
                previousOutput = false; //to avoid a blank line
                pc = pc->next;
                break;
            case OUT: //Output the number/string stored in memory
                if(previousOutput) cout << endl;
                if(!pc->output_inst.outputIsStr) {
                    printf("%d", mem[pc->output_inst.varIndex]);
                } else {
                    cout << pc->output_inst.str;
                }
                previousOutput = true;
                pc = pc->next;
                break;
            case ASSIGN: //For assignment statements, we need to check if there is an operation
                switch(pc->assign_inst.op) {
                    case OPERATOR_PLUS:
                        op1 = mem[pc->assign_inst.operand1Index];
                        op2 = mem[pc->assign_inst.operand2Index];
                        result = op1 + op2;
                        break;
                    case OPERATOR_MINUS:
                        op1 = mem[pc->assign_inst.operand1Index];
                        op2 = mem[pc->assign_inst.operand2Index];
                        result = op1 - op2;
                        break;
                    case OPERATOR_TIMES:
                        op1 = mem[pc->assign_inst.operand1Index];
                        op2 = mem[pc->assign_inst.operand2Index];
                        result = op1 * op2;
                        break;
                    case OPERATOR_DIV:
                        op1 = mem[pc->assign_inst.operand1Index];
                        op2 = mem[pc->assign_inst.operand2Index];
                        //Check for divide by 0
                        if(op2 == 0) { divideBy0(); }
                        result = op1 / op2;
                        break;
                    case OPERATOR_MOD:
                        op1 = mem[pc->assign_inst.operand1Index];
                        op2 = mem[pc->assign_inst.operand2Index];
                        //Check for divide by 0
                        if(op2 == 0) { modBy0(); }
                        result = op1 % op2;
                        break;
                    case OPERATOR_NONE:
                        op1 = mem[pc->assign_inst.operand1Index];
                        result = op1;
                        break;
                }
                mem[pc->assign_inst.leftHandSideIndex] = result;
                pc = pc->next;
                break;
            case CJMP: //Conditional jumps are for loops
                if(pc->cjmp_inst.target == NULL) {
                    printf("Error: cjmp target is NULL. Exiting.\n");
                    exit(1);
                }

                op1 = mem[pc->assign_inst.operand1Index];
                op2 = mem[pc->assign_inst.operand2Index];

                switch(pc->cjmp_inst.conditionOp) {
                    case CONDITION_GREATER:
                        if(op1 > op2) {
                            pc = pc->next;
                        } else {
                            pc = pc->cjmp_inst.target;
                        }
                        break;
                    case CONDITION_LESS:
                        if(op1 < op2) {
                            pc = pc->next;
                        } else {
                            pc = pc->cjmp_inst.target;
                        }
                        break;
                    case CONDITION_NOTEQUAL:
                        if(op1 != op2) {
                            pc = pc->next;
                        } else {
                            pc = pc->cjmp_inst.target;
                        }
                        break;
                    case CONDITION_EQUAL:
                        if(op1 == op2) {
                            pc = pc->next;
                        } else {
                            pc = pc->cjmp_inst.target;
                        }
                        break;
                }
                break;
            case JMP: //Jumps are for if/else statements
                if(pc->jmp_inst.target == NULL) {
                    printf("Error: jmp target is NULL. Exiting.\n");
                    exit(1);
                }
                pc = pc->jmp_inst.target;
                break;
            case PARAM: //For the parameter of a function
                mem[pc->param_inst.parameterIndex] = mem[memIndex];
                pc = pc->next;
                break;
            case FUNC: //For a function call
                memIndex = pc->func_inst.callerIndex;
                executeProgram(pc->func_inst.funcHead);
                pc = pc->next;
                break;
            default:
                printf("Error: Invalid program counter type. Exiting.\n");
                exit(1);
                break;
        }
    }
}

//Start the program. Parse and compile
int main(int argc, char* argv[]) {
    //Get the name of the input file from the command line
    fileName = argv[1];
    
    //Parse and execute the program
    struct InstructionNode* program = parseProgram();
    executeProgram(program);

    return 0;
}
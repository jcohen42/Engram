/*
Jake Cohen
ASU Thesis
Engram Programming Language
*/

#include "parser.h"

#include <iostream>
#include <istream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

extern int mem[1024];
extern int nextAvailable;

#define DEBUG 0

//Check to make sure the next token's type is as expected
//Return the token if it is
Token Parser::expect(TokenType expected) {
    Token tok = lexer.get();
    if(tok.type != expected) {
        syntaxError(tok);
    }
    return tok;
}

//Find the location of an ID in memory
int Parser::location(string varName) {
    if(DEBUG) { 
        cout << "DEBUG: looking for " << varName << endl;
        cout << "DEBUG: current table:\n";
        for (auto i : symbolTable) {
			cout << "\t{" << i.first << ": " << i.second << "}\n";
		}
    }
	//Look for the variable in symbolTable
	if(symbolTable.find(varName) != symbolTable.end()) {
        if(DEBUG) { cout << "DEBUG: found " << varName << endl; }
		//Return the index of the variable in mem if it is found
		return symbolTable.find(varName)->second;
	} else {
        if(DEBUG) { cout << "DEBUG: NOT found " << varName << endl; }
		//If the variable name does not exist, return -1
		return -1;
	}
}

/*
DATA FUNCTIONS
*/
struct Parser::operation {
    string primary1;
    ArithmeticOperator oper;
    string primary2;
};

struct Parser::condition {
	string operand1;
	ConditionalOperator cond;
	string operand2;
};

struct Parser::if_block {
    struct condition* cond;
    struct InstructionNode* body;
};

struct Parser::function {
    string name;
    struct InstructionNode* head;
    bool hasReturn;
    string returnVariable;
};

struct findFunctionName : unary_function<Parser::function, bool> {
    string name;
    findFunctionName(string name):name(name) {}
    bool operator()(Parser::function const& f) const {
        return f.name == name;
    }
};

/*
ERROR FUNCTIONS
*/
void Parser::syntaxError(Token tok) {
    cout << "Syntax error encountered on line "
         << tok.lineNum << "!\n";
    exit(1);
}

void Parser::variableNotInitialized(Token var) {
    cout << "Error: Variable " << var.lexeme << " was not initialized before it was used"
     << " on line " << var.lineNum << ".\nPlease ensure that this variable is declared beforehand.";
    exit(1);
}

void Parser::functionIDMismatch(Token id1, Token id2) {
    cout << "Error: Function " << id1.lexeme << " does not match its end specifier.\n" <<
        "We expected to receive " << id1.lexeme << " on line " << id2.lineNum << " but got " << id2.lexeme << ".";
    exit(1);
}

void Parser::functionRedeclaration(Token id) {
    cout << "Error: Function " << id.lexeme << " was declared more than once.\n" <<
        "To fix this error, either rename the repeat definition or delete it entirely.";
    exit(1);
}

void Parser::functionNotFound(Token id) {
    cout << "Error: Function " << id.lexeme << ", which was called in line " << id.lineNum << 
    ", was not found.\nPlease make sure that this function exists.";
    exit(1);
}

/*
PARSING FUNCTIONS
*/
struct InstructionNode* Parser::parse_start() {
    struct InstructionNode* program;
    if(lexer.peek(1).type == BEGIN && lexer.peek(2).type == ID) {
        parse_function_section();
    }
    program = parse_program_section();
    expect(END_OF_FILE);
    if(DEBUG) { cout << "DEBUG: done parsing" << endl; }
    return program;
}

void Parser::parse_function_section() {
    struct function* fun = parse_function();

    //Check if there is another function definition
    //If so, call this function recursively
    if(lexer.peek(1).type == BEGIN && lexer.peek(2).type == ID) {
        parse_function_section();
    }
}

struct Parser::function* Parser::parse_function() {
    struct function* fun = new function;
    fun->hasReturn = false;

    expect(BEGIN);
    Token id1 = expect(ID);
    fun->name = id1.lexeme;
    //Check if the function is already in the list
    for(function* f : functionList) {
        if((f->name).compare(id1.lexeme) == 0)
            functionRedeclaration(id1); //Display error
    }
    expect(WITH);
    expect(ARGUMENT);

    struct InstructionNode* param = new InstructionNode;
    param->type = PARAM;
    fun->head = param;
    functionList.push_back(fun);

    //Get the function argument and assign it to memory
    Token argument = expect(ID);
    //TEMPORARY: add the function arguemnt to the symbol table
    if(location(argument.lexeme) == -1) {
		//If the variable is not found, add it to mem and to the symbol table
		symbolTable[argument.lexeme] = nextAvailable;
		mem[nextAvailable] = 0;
		nextAvailable++;
	}
    param->param_inst.parameterIndex = location(argument.lexeme);
    expect(COLON);

    //Parse the body of the function
    param->next = fun->head->next;
    fun->head->next = parse_statement_list();

    //Check if this function returns a value
    if(lexer.peek(1).type == RETURN_UPPER) {
        fun->hasReturn = true;
        fun->returnVariable = parse_return_section();
    }
    expect(END);
    Token id2 = expect(ID);
    if((id1.lexeme).compare(id2.lexeme) != 0) {
        functionIDMismatch(id1, id2); //Display error
    }
    expect(PERIOD);

    return fun;
}

string Parser::parse_return_section() {
    //Get the name of the return variable
    expect(RETURN_UPPER);
    string var = parse_variable().lexeme;
    expect(PERIOD);

    return var;
}

Token Parser::parse_variable() {
    //Check to see if the value is an ID or NUM and store it in memory
    Token t = lexer.get();
    if(DEBUG) { cout << "DEBUG: parsing " << t.lexeme << endl; }
    if(t.type == ID) {  
        //If the token is an ID, we need to check if it exists in memory
        if(location(t.lexeme) == -1) { variableNotInitialized(t); }
        //If it does, we can parse the variable
        return t;
    } else if (t.type == NUM) {
        //Look for the constant to see if it's stored in the symbol table
        if(location(t.lexeme) == -1) {
			//If the variable is not found, add it to mem and to the symbol table
			symbolTable[t.lexeme] = nextAvailable;
			//Assign memory for the constant
			mem[nextAvailable] = stoi(t.lexeme);
			nextAvailable++;
		}
		//If the variable was found, we don't need to add it to the symbol table or mem
    } else {
        syntaxError(t);
    }
    return t;
}

struct InstructionNode* Parser::parse_program_section() {
    expect(BEGIN);
    expect(PROGRAM);
    expect(COLON);
    struct InstructionNode* program = parse_statement_list();
    expect(END);
    expect(PROGRAM);
    expect(PERIOD);

    return program;
}

struct InstructionNode* Parser::parse_statement_list() {
    struct InstructionNode* stmt = parse_statement();
    struct InstructionNode* stmt2;

    //Check if there is another statement
    //If so, call this function recursively
    Token t = lexer.peek(1);
    if(t.type == IF_UPPER || t.type == WHILE_UPPER || t.type == SET_UPPER
            || t.type == INPUT || t.type == OUTPUT || t.type == CALL) {
        stmt2 = parse_statement_list();
        if(DEBUG) { cout << "DEBUG: appending\n"; }
        //Iterate through to the end of stmt1
        struct InstructionNode* tracker = stmt;
        while(tracker->next != NULL) {
            if(DEBUG) { cout << "DEBUG: appending3\n"; }
            tracker = tracker->next;
        }
        if(DEBUG) { cout << "DEBUG: appending4\n"; }
        //Append stmt2 to the end of stmt
        tracker->next = stmt2;
        if(DEBUG) { cout << "DEBUG: appending5\n"; }
    }

    return stmt;
}

struct InstructionNode* Parser::parse_statement() {
    //Create a new instruction node representing a statement
    struct InstructionNode* stmt = new InstructionNode;
    Token t = lexer.peek(1);

    if(t.type == IF_UPPER) {
        stmt = parse_if_statement();
    } else if(t.type == WHILE_UPPER) {
        stmt = parse_while_statement();
    } else if(t.type == SET_UPPER) {
        stmt = parse_assign_statement();
    } else if(t.type == INPUT) {
        stmt = parse_input_statement();
    } else if(t.type == OUTPUT) {
        stmt = parse_output_statement();
    } else if(t.type == CALL) {
        stmt = parse_function_statement();
    } else {
        syntaxError(t);
    }

    return stmt;
}

struct InstructionNode* Parser::parse_if_statement() {
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = CJMP;

    struct if_block* block = parse_if_block();

    stmt->cjmp_inst.conditionOp = block->cond->cond;
    stmt->cjmp_inst.operand1Index = location(block->cond->operand1);
    stmt->cjmp_inst.operand2Index = location(block->cond->operand2);
    stmt->next = block->body;

    //Set the very end of the if body as the target
    struct InstructionNode* tracker = block->body;
	while(tracker->next != NULL) {
		tracker = tracker->next;
	}

    //Create a noop node
	struct InstructionNode* no_op = new InstructionNode;
	no_op->type = NOOP;
	no_op->next = NULL;

	//Append the noop node to the body of the if
	tracker->next = no_op;

    //Check to see if there is an Else declaration
    if(lexer.peek(1).type == ELSE) {
        //If there is, we need to set the target of the jcmp instance
        //to the beginnging of the else block
        struct InstructionNode* elseBody = parse_else_block();

        stmt->cjmp_inst.target = elseBody;

        struct InstructionNode* tracker2 = elseBody;
	    while(tracker2->next != NULL) {
		    tracker2 = tracker->next;
	    }
        tracker2->next = no_op;
    } else {
        //If there is no else, set the target of the CJMP as no_op
        stmt->cjmp_inst.target = no_op;
    }

    expect(END);
    expect(IF_LOWER);
    expect(PERIOD);

    return stmt;
}

struct Parser::if_block* Parser::parse_if_block() {
    struct if_block* block = new if_block;

    expect(IF_UPPER);
    struct condition* c = parse_condition();
    expect(COLON);
    struct InstructionNode* ifBody = parse_statement_list();

    block->cond = c;
    block->body = ifBody;

    return block;
}

struct InstructionNode* Parser::parse_else_block() {
    expect(ELSE);
    expect(COLON);
    struct InstructionNode* elseBody = parse_statement_list();
    return elseBody;
}

struct InstructionNode* Parser::parse_while_statement() {
    struct InstructionNode* whileLoop = new InstructionNode;
    whileLoop->type = CJMP;

    expect(WHILE_UPPER);
    struct condition* c = parse_condition();
    expect(COLON);

    //Assign CJMP condition variables
    whileLoop->cjmp_inst.conditionOp = c->cond;
    whileLoop->cjmp_inst.operand1Index = location(c->operand1);
    whileLoop->cjmp_inst.operand2Index = location(c->operand2);

    struct InstructionNode* body = parse_statement_list();
    whileLoop->next = body;

    //Create a jmp node which will be used to go back to
    //the condition at the end of the body
	struct InstructionNode* jumpNode = new InstructionNode;
	jumpNode->type = JMP;
	jumpNode->jmp_inst.target = whileLoop;

	//Create a noop node
	struct InstructionNode* no_op = new InstructionNode;
	no_op->type = NOOP;
	no_op->next = NULL;

    //Set the very end of the body as the target
	struct InstructionNode* tracker = body;
	while(tracker->next != NULL) {
		tracker = tracker->next;
	}
	//Append the jump node to the body of the while loop
	tracker->next = jumpNode;
	jumpNode->next = no_op;

	whileLoop->cjmp_inst.target = no_op;

    expect(END);
    expect(WHILE_LOWER);
    expect(PERIOD);

    return whileLoop;
}

struct Parser::condition* Parser::parse_condition() {
    struct condition* c = new condition;

    c->operand1 = parse_variable().lexeme;
    c->cond = parse_relop();
    c->operand2 = parse_variable().lexeme;

    return c;
}

ConditionalOperator Parser::parse_relop() {
    ConditionalOperator relop = CONDITION_NOTEQUAL; //placeholder value for relop

    expect(IS);
    Token t = lexer.get();
    if(t.type == GREATER) {
        expect(THAN);
        relop = CONDITION_GREATER;
    } else if(t.type == LESS) {
        expect(THAN);
        relop = CONDITION_LESS;
    } else if(t.type == EQUAL) {
        expect(TO);
        relop = CONDITION_EQUAL;
    } else if(t.type == NOT) {
        expect(EQUAL);
        expect(TO);
        relop = CONDITION_NOTEQUAL;
    } else {
        syntaxError(t);
    }

    return relop;
}

struct InstructionNode* Parser::parse_assign_statement() {
    if(DEBUG) { cout << "DEBUG: parsing assign\n"; }
    //Create a new assign instruction node
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = ASSIGN;

    expect(SET_UPPER);

    Token leftHandSide = expect(ID);
    //Look for the ID to see if it's stored in the symbol table
    if(location(leftHandSide.lexeme) == -1) {
        //If the variable is not found, add it to mem and to the symbol table
		symbolTable[leftHandSide.lexeme] = nextAvailable;
		mem[nextAvailable] = 0;
		nextAvailable++;
    }
    stmt->assign_inst.leftHandSideIndex = location(leftHandSide.lexeme);

    expect(EQUAL);
    expect(TO);
    Token t1 = lexer.peek(1);
    Token t2 = lexer.peek(2);
    //Check to see if there are 1 or two operands
    //Parsing the right hand side of the equation
    if((t1.type == NUM || t1.type == ID) && 
            (t2.type == PLUS || t2.type == MINUS || t2.type == TIMES || 
             t2.type == DIVIDED || t2.type == MODULO)) {
        //If there is an operand, we can assign both of them
        struct operation* op1 = parse_operation();
        stmt->assign_inst.operand1Index = location(op1->primary1);
        stmt->assign_inst.operand2Index = location(op1->primary2);
        stmt->assign_inst.op = op1->oper; //Assigning the operator type
    } else if(t1.type == ID || t1.type == NUM) { //Otherwise, there is only one operand
        stmt->assign_inst.operand1Index = location(parse_variable().lexeme);
        stmt->assign_inst.op = OPERATOR_NONE;
    } else {
        syntaxError(t1);
    }
    expect(PERIOD);

    stmt->next = NULL;
    return stmt;
}

struct Parser::operation* Parser::parse_operation() {
    struct operation* op = new operation;
    op->primary1 = parse_variable().lexeme;
    op->oper = parse_operator();
    op->primary2 = parse_variable().lexeme;

    return op;
}

ArithmeticOperator Parser::parse_operator() {
    ArithmeticOperator oper = OPERATOR_NONE; //placeholder value for op
    Token t = lexer.get();
    if(t.type == PLUS) {
        oper = OPERATOR_PLUS;
    } else if(t.type == MINUS) {
        oper = OPERATOR_MINUS;
    } else if(t.type == TIMES) { 
        oper = OPERATOR_TIMES;
    } else if(t.type == DIVIDED) {
        expect(BY);
        oper = OPERATOR_DIV;
    } else if(t.type == MODULO) {
        oper = OPERATOR_MOD;
    } else {
        syntaxError(t);
    }

    return oper;
}

struct InstructionNode* Parser::parse_input_statement() {
    if(DEBUG) { cout << "DEBUG: parsing input\n"; }
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = IN;

    expect(INPUT);

    Token userInput = expect(ID);
    //Look for the ID to see if it's stored in the symbol table
    if(location(userInput.lexeme) == -1) {
        //If the variable is not found, add it to mem and to the symbol table
		symbolTable[userInput.lexeme] = nextAvailable;
		mem[nextAvailable] = 0;
		nextAvailable++;
    }
    stmt->input_inst.varIndex = location(userInput.lexeme);
    stmt->next = NULL;
    expect(PERIOD);

    return stmt;
}

struct InstructionNode* Parser::parse_output_statement() {
    if(DEBUG) { cout << "DEBUG: parsing output\n"; }
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = OUT;

    expect(OUTPUT);
    Token t = lexer.get();
    if(t.type == ID) {
        //Check to see if the ID exists in the symbol table
        if(location(t.lexeme) == -1) { variableNotInitialized(t); }
        //If we got here, the variable does exist and we can update the varIndex
        stmt->output_inst.varIndex = location(t.lexeme);
        // stmt->output_inst.str = "???";
        stmt->output_inst.outputIsStr = false;
    } else if(t.type == STRING) {
        stmt->output_inst.varIndex = -1;
        string outputStr = (t.lexeme).substr(1, (t.lexeme).size() - 2);
        char* outputChar = new char[outputStr.size() + 1];
        memcpy(outputChar, outputStr.c_str(), outputStr.size() + 1);
        stmt->output_inst.str = outputChar;
        stmt->output_inst.outputIsStr = true;
    } else {
        syntaxError(t);
    }
    expect(PERIOD);

    stmt->next = NULL;
    return stmt;
}

struct InstructionNode* Parser::parse_function_statement() {
    struct InstructionNode* stmt = new InstructionNode;

    Token t = lexer.peek(6);
    if(t.type == PERIOD) {
        stmt = parse_function_statement_simple();
    } else if(t.type == COMMA) {
        stmt = parse_function_statement_return();
    } else {
        syntaxError(lexer.peek(1));
    }

    return stmt;
}

struct InstructionNode* Parser::parse_function_statement_simple() {
    if(DEBUG) { cout << "DEBUG: parsing simple function call\n"; }
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = FUNC;

    expect(CALL);
    Token funcName = expect(ID);

    //See if the function actually exists
    bool hasFuncBeenFound = false;
    struct function* func = NULL;
    for(function* f : functionList) {
        if((f->name).compare(funcName.lexeme) == 0) {
            func = f;
            hasFuncBeenFound = true;
            break;
        }
    }
    //If it doesn't, display an error
    if(!hasFuncBeenFound) { functionNotFound(funcName); }

    //Parse the argument that will be passed into the function
    expect(WITH);
    expect(ARGUMENT);
    Token argument = expect(ID);
    stmt->func_inst.callerIndex = location(argument.lexeme);
    expect(PERIOD);

    //Set the instruction node to the beginning of the function
    stmt->func_inst.funcHead = func->head;

    //We need a node to go to after the function completes
    //So we'll set the function call's next node as a NOOP
    struct InstructionNode* stmt2 = new InstructionNode;
    stmt2->type = NOOP;
    stmt2->next = NULL;

    stmt->next = stmt2;

    return stmt;
}

struct InstructionNode* Parser::parse_function_statement_return() {
    if(DEBUG) { cout << "DEBUG: parsing function call with return\n"; }
    struct InstructionNode* stmt = new InstructionNode;
    stmt->type = FUNC;

    expect(CALL);
    Token funcName = expect(ID);
    //See if the function the actually exists
    bool hasFuncBeenFound = false;
    struct function* func = NULL;
    for(function* f : functionList) {
        if((f->name).compare(funcName.lexeme) == 0) {
            func = f;
            hasFuncBeenFound = true;
            break;
        }
    }
    //If it doesn't, display an error
    if(!hasFuncBeenFound) { functionNotFound(funcName); }

    expect(WITH);
    expect(ARGUMENT);
    Token argument = expect(ID);
    stmt->func_inst.callerIndex = location(argument.lexeme);

    //Set the instruction node to the beginning of the function
    stmt->func_inst.funcHead = func->head;

    expect(COMMA);
    expect(SET_LOWER);
    expect(RETURN_LOWER);
    expect(TO);
    Token returnVar = expect(ID);
    expect(PERIOD);

    //If the return variable doesn't exist, we can add it to the symbol table
    if(location(returnVar.lexeme) == -1) {
        //If the variable is not found, add it to mem and to the symbol table
		symbolTable[returnVar.lexeme] = nextAvailable;
		mem[nextAvailable] = 0;
		nextAvailable++;
    }

    //Create an assignment node to set the return value
    struct InstructionNode* stmt2 = new InstructionNode;
    stmt2->type = ASSIGN;
    stmt2->next = NULL;

    stmt2->assign_inst.leftHandSideIndex = location(returnVar.lexeme);
    stmt2->assign_inst.operand1Index = location(func->returnVariable);
    stmt2->assign_inst.op = OPERATOR_NONE;

    stmt->next = stmt2;

    return stmt;
}

struct InstructionNode* parseProgram() {
    Parser parser;
    struct InstructionNode* program = parser.parse_start();
    return program;
}

/*
Jake Cohen
ASU Thesis
Pure-ENG Programming Language
*/

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"

using namespace std;

#define KEYWORDS_COUNT 30

string reserved[] = {"END_OF_FILE",
    "BEGIN", "WITH", "ARGUMENT", "END", "RETURN_UPPER", "RETURN_LOWER", "PROGRAM", 
    "IF_UPPER", "IF_LOWER", "ELSE", "WHILE_UPPER", "WHILE_LOWER", 
    "IS", "GREATER", "LESS", "EQUAL", "NOT", "THAN", "TO", 
    "PLUS", "MINUS", "TIMES", "DIVIDED", "BY", "MODULO", 
    "INPUT", "OUTPUT", "CALL", "SET_UPPER", "SET_LOWER", 
    "PERIOD", "COMMA", "COLON",
    "NUM", "ID", "STRING", "COMMENT", "ERROR"};

//print the token, its type, and the line number
void Token::print() {
    cout << "{" << this->lexeme << " , "
    << reserved[(int) this->type] << " , "
    << this->lineNum << "}\n";
}

//Constructor for a lexical analyzer
LexicalAnalyzer::LexicalAnalyzer() {
    input.initBuffer();

    this->lineNum = 1;

    //Set up the temporary token
    temp.lexeme = "";
    temp.lineNum = 1;
    temp.type = ERROR;

    Token tok = getTokenMain();
    index = 0;

    //Consume all tokens and put them in tokenList
    while(tok.type != END_OF_FILE) {
        tokenList.push_back(tok);
        tok = getTokenMain();
    }
}

//Ignore spaces in the input stream
void LexicalAnalyzer::skipSpace() {
    char c;

    input.getChar(c);
    lineNum += (c == '\n'); //skip newlines

    while(!input.endOfInput() && isspace(c)) {
        input.getChar(c); //consume the space character
        lineNum += (c == '\n'); //increase the line number if newline
    }

    if(!input.endOfInput()) {
        input.ungetChar(c);
    }
}

//Iterate through each reserved word and check if it matches the input string
int LexicalAnalyzer::findKeywordIndex(string str) {
    string keywordArray[] = {"Begin", "with", "argument", "End", "Return", "return", "program", 
    "If", "if", "Else", "While", "while", 
    "is", "greater", "less", "equal", "not", "than", "to", 
    "plus", "minus", "times", "divided", "by", "modulo", 
    "Input", "Output", "Call", "Set", "set"};
    int i;
    for(i = 0; i < KEYWORDS_COUNT; i++) {
        if(str == keywordArray[i]) {
            return ++i;
        }
    }
    return -1;
}

//If a double quote is found, scan until another double quote is found
//The token type will be STRING
Token LexicalAnalyzer::scanString() {
    char c;
    input.getChar(c);

    if(c == '\"') {
        temp.lexeme = "\"";
        input.getChar(c);
        lineNum += (c == '\n'); //skip newlines
        while(!input.endOfInput() && c != '\"') {
            temp.lexeme += c;
            input.getChar(c);
            lineNum += (c == '\n'); //skip newlines
        }
        //Get the final double quote
        if(c == '\"') {
            temp.lexeme += c;
            input.getChar(c);
        } else {
            //If there is no final double quote, produce an ERROR token
            temp.type = ERROR;
            temp.lineNum = lineNum;
            return temp;
        }
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Set the token
        temp.lineNum = lineNum;
        temp.type = STRING;
    } else {
        //If c is not a quote, unget the character
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Produce an ERROR token
        temp.type = ERROR;
        temp.lineNum = lineNum;
    }
    return temp;
}

//If a left parenthesis is found, scan until a right parenthesis is found
//The token type will be COMMENT
Token LexicalAnalyzer::scanComment() {
    char c;
    input.getChar(c);

    int extraLParensEncountered = 0; //account for embedded parentheses

    if(c == '(') {
        temp.lexeme = "(";
        input.getChar(c);
        lineNum += (c == '\n'); //skip newlines
        while(!input.endOfInput() && (c != ')' || extraLParensEncountered > 0)) {
            if(c == '(')
                extraLParensEncountered++;
            else if(c == ')')
                extraLParensEncountered--;
            temp.lexeme += c;
            input.getChar(c);
            lineNum += (c == '\n'); //skip newlines
        }
        //Get the final double quote
        if(c == ')') {
            temp.lexeme += c;
            input.getChar(c);
        } else {
            //If there is no final double quote, produce an ERROR token
            temp.type = ERROR;
            temp.lineNum = lineNum;
            return temp;
        }
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Set the token
        temp.lineNum = lineNum;
        temp.type = COMMENT;
    } else {
        //If c is not a quote, unget the character
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Produce an ERROR token
        temp.type = ERROR;
        temp.lineNum = lineNum;
    }
    return temp;
}

//Consume a character and check to see if it is a number
Token LexicalAnalyzer::scanNum() {
    char c;

    input.getChar(c);
    if(isdigit(c)) {
        if(c == '0') {
            //numbers should not start with '0'
            temp.lexeme = "0";
        } else {
            temp.lexeme = "";
            while(!input.endOfInput() && isdigit(c)) {
                temp.lexeme += c;
                input.getChar(c);
            }
            if(!input.endOfInput()) {
                input.ungetChar(c);
            }
        }
        temp.type = NUM;
        temp.lineNum = lineNum;
    } else if(c == '-') { //for recognizing negative numbers
        temp.lexeme = "-";
        input.getChar(c);
        while(!input.endOfInput() && isdigit(c)) {
            temp.lexeme += c;
            input.getChar(c);
        }
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        temp.type = NUM;
        temp.lineNum = lineNum;
    } else {
        //If c is not a digit, unget the character
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Produce an ERROR token
        temp.lexeme = "";
        temp.type = ERROR;
        temp.lineNum = lineNum;
    }
    return temp;
}

//Consume a character and check to see if it is an ID or keyword
Token LexicalAnalyzer::scanKeyword() {
    char c;
    input.getChar(c);
    
    if(isalpha(c)) {
        temp.lexeme = "";
        while(!input.endOfInput() && isalnum(c)) {
            temp.lexeme += c;
            input.getChar(c);
            //lineNum += (c == '\n'); //skip newlines
        }
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        temp.lineNum = lineNum;
        int keywordIndex = findKeywordIndex(temp.lexeme);
        if(keywordIndex != -1) {
            temp.type = (TokenType)keywordIndex;
        } else {
            temp.type = ID;
        }
    } else {
        //If c is not a letter, unget the character
        if(!input.endOfInput()) {
            input.ungetChar(c);
        }
        //Produce an ERROR token
        temp.lexeme = "";
        temp.type = ERROR;
        temp.lineNum = lineNum;
    }
    return temp;
}

//Read tokens from tokenList and return a token object
Token LexicalAnalyzer::get() {
    Token tok;

    if(index == (int)tokenList.size()) {
        //If we've reached the end of the input stream, return and EOF Token
        tok.lexeme = "";
        tok.lineNum = lineNum;
        tok.type = END_OF_FILE;
    } else {
        //Otherwise, consume the token from the list
        tok = tokenList[index];
        index++;
    }

    return tok;
}

//Look ahead in tokenList and return the type of token numToPeek tokens ahead
Token LexicalAnalyzer::peek(int numToPeek) {
    int peekIndex = index + numToPeek - 1;
    //Make sure we aren't peekeing past the end of file marker
    if(peekIndex > ((int)tokenList.size() - 1)) {
        Token tok;
        tok.lexeme = "";
        tok.lineNum = lineNum;
        tok.type = END_OF_FILE;
        return tok;
    } else {
        return tokenList[peekIndex];
    }
}

//Check the first chracter of a new token to assign its type
Token LexicalAnalyzer::getTokenMain() {
    char c;
    skipSpace();
    temp.lexeme = "";
    temp.lineNum = lineNum;
    input.getChar(c);

    //Check for single-character tokens
    switch(c) {
        case ':':   temp.type = COLON;  return temp;
        case '.':   temp.type = PERIOD;  return temp;
        case ',':   temp.type = COMMA;  return temp;
        default:
            if(c == '\"') {
                //If the character is a double quote, scan for a string
                input.ungetChar(c);
                return scanString();
            } else if(c == '(') {
                //If the character is a left parenthesis, scan for a comment
                input.ungetChar(c);
                return scanComment();
            } else if(isdigit(c) || c == '-') {
                //If the character is a digit, scan the whole number
                input.ungetChar(c);
                return scanNum();
            } else if(isalpha(c)) {
                //Check if the variable is a letter
                input.ungetChar(c);
                return scanKeyword();
            } else if(input.endOfInput()) {
                //Check for EOF
                temp.type = END_OF_FILE;
            } else {
                //If the token is no known type, return error
                temp.type = ERROR;
            }
            return temp;
    }
}
/*
Jake Cohen
ASU Thesis
Pure-ENG Programming Language
*/

#include <vector>
#include <string>
#include "inputBuffer.h"

using namespace std;

typedef enum { END_OF_FILE = 0,
    BEGIN, WITH, ARGUMENT, END, RETURN_UPPER, RETURN_LOWER, PROGRAM, 
    IF_UPPER, IF_LOWER, ELSE, WHILE_UPPER, WHILE_LOWER, 
    IS, GREATER, LESS, EQUAL, NOT, THAN, TO, 
    PLUS, MINUS, TIMES, DIVIDED, BY, MODULO, 
    INPUT, OUTPUT, CALL, SET_UPPER, SET_LOWER, 
    PERIOD, COMMA, COLON, 
    NUM, ID, STRING, ERROR
} TokenType;

class Token {
    public:
        void print();

        string lexeme;
        TokenType type;
        int lineNum;
};

class LexicalAnalyzer {
    public:
        Token get();
        Token peek(int);
        LexicalAnalyzer();

    private:
        vector<Token> tokenList;
        Token getTokenMain();
        int lineNum;
        int index;
        Token temp;
        InputBuffer input;
        bool skipSpace();
        int findKeywordIndex(string);
        Token scanKeyword();
        Token scanNum();
        Token scanString();
};
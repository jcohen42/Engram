/*
Jake Cohen
ASU Thesis
Pure-ENG Programming Language
*/

#include "inputBuffer.h"

#include <iostream>
#include <istream>
#include <cstdio>

using namespace std;

extern string fileName;

//Open the file stored in the extern
void InputBuffer::initBuffer() {
    const char* f = fileName.c_str();
    file = fopen(f, "r");
    if(!file) {
        cout << "Error: the specified file was not found. Exiting.\n";
        exit(1);
    }
}

//consume a character from the input stram and return it
void InputBuffer::getChar(char& c) {
    if(!inputBuffer.empty()) {
        c = inputBuffer.back();
        inputBuffer.pop_back();
    } else {
        c = (char)fgetc(file);
        
        //If the read character is the value for EOF, set the variable as true
        if((int)c == EOF) {
            eof = true;
        }
    }
}

//push a consumed character back to the top of the stream
char InputBuffer::ungetChar(char c) {
    if(c != EOF) {
        inputBuffer.push_back(c);
        eof = false;
    }
    return c;
}

//push back a particular string
string InputBuffer::ungetString(string str) {
    int i;
    for(i = 0; i < (int)str.size(); i++) {
        int translatedSize = str.size() - i - 1;
        inputBuffer.push_back(str[translatedSize]);
    }
    return str;
}

//check if we have reached the end of the input stream
bool InputBuffer::endOfInput() {
    if(inputBuffer.empty()) {
        return eof;
    } else {
        return false;
    }
}
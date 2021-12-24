/*
Jake Cohen
ASU Thesis
Pure-ENG Programming Language
*/

#include <vector>
#include <string>

using namespace std;

class InputBuffer {
    public:
        void initBuffer();
        void getChar(char &);
        char ungetChar(char);
        string ungetString(string);
        bool endOfInput();
    
    private:
        vector<char> inputBuffer;
        FILE* file;
        bool eof = false;
};
OBJS	= inputBuffer.o lexer.o parser.o compiler.o
SOURCE	= inputBuffer.cpp lexer.cpp parser.cpp compiler.cpp
HEADER	= inputBuffer.h lexer.h parser.h compiler.h
OUT	= engram.exe
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

inputBuffer.o: inputBuffer.cpp
	$(CC) $(FLAGS) inputBuffer.cpp

lexer.o: lexer.cpp
	$(CC) $(FLAGS) lexer.cpp

parser.o: parser.cpp
	$(CC) $(FLAGS) parser.cpp

compiler.o: compiler.cpp
	$(CC) $(FLAGS) compiler.cpp


clean:
	rm -f $(OBJS) $(OUT)

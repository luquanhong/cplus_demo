
CFLAG := -Iturbo/inc

all: main.o MessageLoop.o
	gcc -o main main.o MessageLoop.o

main.o:
	g++ -o main.o -c main.cpp $(CFLAG)

MessageLoop.o: MessageLoop.cpp
	g++ -o MessageLoop.o -c MessageLoop.cpp $(CFLAG)


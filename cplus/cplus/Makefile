
CFLAG := -Iturbo/inc

all: main.o MessageLoop.o
	g++ -o main main.o MessageLoop.o -lpthread -g

main.o:
	g++ -o main.o -c main.cpp $(CFLAG) -g

MessageLoop.o: MessageLoop.cpp
	g++ -o MessageLoop.o -c MessageLoop.cpp $(CFLAG) -g


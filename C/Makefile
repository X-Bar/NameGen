
CC = gcc -g
INCLUDE = /usr/local/include
LIBS = -lboost_system -lboost_date_time -lboost_thread
binaries = NameGen 

default: clean NameGen

clean:
	$(RM) NameGen

NameGen: NameGen.cpp
	g++ NameGen.cpp $(LIBS) -o NameGen




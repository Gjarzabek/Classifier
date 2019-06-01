CC = g++
CFLAGS = -Wall -fexceptions -c -std=c++14
LDFLAGS = -Wall -fexceptions -std=c++14

output: DataTable.o Classifier.o DecisionTree.o main.o
	$(CC) $(LDFLAGS) main.o DecisionTree.o Classifier.o DataTable.o -o output

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Classifier.o: Classifier.cpp Classifier.hpp
	$(CC) $(CFLAGS) Classifier.cpp

DataTable.o: DataTable.cpp DataTable.hpp
	$(CC) $(CFLAGS) DataTable.cpp

DecisionTree.o: DecisionTree.cpp DecisionTree.hpp
	$(CC) $(CFLAGS) DecisionTree.cpp

clean:
	rm *.o output *.gch

#	CC = gcc
#	CFLAGS = -Wall --pedantic -c
#	LDFLAGS = -Wall --pedantic
#
#	OBJECTS = err.o # pliki naglowkowe
#	ALL = signals cleanup ctrl-c child
#
#	all: $(ALL)
#
#	%.o : %.c %.h
#		$(CC) $(CFLAGS) $< # co znaczy $< , $@, $^ ,*~
#
#	$(ALL) : % : %.o $(OBJECTS)
#		$(CC) $(LDFLAGS) -o $@ $^
#
#	clean:
#		rm -rf *.o $(ALL) *~

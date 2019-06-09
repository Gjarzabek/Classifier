CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -c -std=c++14 -pthread
LDFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -std=c++14 -pthread

app: ClassfiyShell.o DataTable.o Classifier.o DecisionTree.o main.o
	$(CC) $(LDFLAGS) main.o DecisionTree.o Classifier.o DataTable.o -o app

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

ClassfiyShell.o: ClassfiyShell.cpp ClassfiyShell.hpp classifier_exception.hpp ClassfiyShell.hpp
	$(CC) $(CFLAGS) ClassfiyShell.cpp ClassfiyShell.hpp classifier_exception.hpp

Classifier.o: Classifier.cpp Classifier.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) Classifier.cpp Classifier.hpp classifier_exception.hpp

DataTable.o: DataTable.cpp DataTable.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) DataTable.cpp DataTable.hpp classifier_exception.hpp

DecisionTree.o: DecisionTree.cpp DecisionTree.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) DecisionTree.cpp DecisionTree.hpp classifier_exception.hpp

clean:
	rm *.o *.gch app

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

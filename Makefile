CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -c -std=c++14
LDFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -std=c++14

classifier: DataTable.o Classifier.o DecisionTree.o ClassfiyShell.o main.o
	$(CC) $(LDFLAGS) main.o DecisionTree.o Classifier.o DataTable.o ClassfiyShell.o -o classifier

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

ClassfiyShell.o: ClassfiyShell.cpp ClassfiyShell.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) ClassfiyShell.cpp ClassfiyShell.hpp

Classifier.o: Classifier.cpp Classifier.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) Classifier.cpp Classifier.hpp classifier_exception.hpp

DataTable.o: DataTable.cpp DataTable.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) DataTable.cpp DataTable.hpp classifier_exception.hpp

DecisionTree.o: DecisionTree.cpp DecisionTree.hpp classifier_exception.hpp
	$(CC) $(CFLAGS) DecisionTree.cpp DecisionTree.hpp classifier_exception.hpp

clean:
	rm *.o *.gch classifier
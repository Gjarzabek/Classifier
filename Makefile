CC = g++
CFLAGS = -Wall -fexceptions -c -std=c++14
LDFLAGS = -Wall -fexceptions -std=c++14

exe_output: Classifier_1R.o main.o
	$(CC) $(LDFLAGS) main.o Classifier_1R.o -o exe_output

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Classifier_1R.o: Classifier_1R.cpp Classifier_1R.h
	$(CC) $(CFLAGS) Classifier_1R.cpp

clean:
	rm *.o exe_output

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

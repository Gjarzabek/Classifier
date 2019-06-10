#ifndef CLASSIFYSHELL_H
#define CLASSIFYSHELL_H

#include "Classifier.hpp"


class ClassfiyShell {

private:

    static void print_help(std::string str);
    static void input_parser(int argc, char const *argv[]);
    ClassfiyShell() = delete;
    ClassfiyShell(const ClassfiyShell & b) = delete;

public:

    static void start(int argc, char const *argv[]) {
        input_parser(argc, argv);
    }
};



#endif
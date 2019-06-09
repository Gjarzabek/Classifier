#include "ClassfiyShell.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <memory>

#define TREEPRINT "--tree"

void ClassfiyShell::print_help(std::string str) {
    std::cout << "** Classifier 1.0 **\n";
    if (!str.empty())
        std::cout << "not recognized: " << str << std::endl;
    std::cout << "usage: ./classifier <src_file> <test_files>";
    std::cout << "\n       [--tree] -- prints built tree\n";
}


void ClassfiyShell::input_parser(int argc, char const *argv[]) {
    if (argc < 3) {
        print_help("");
        return;
    }
    bool show = false;
    std::list<std::string> files;
    for (int i = 1; i < argc; ++i) {
        std::string str(argv[i]);
        if (str == TREEPRINT) {
            show = true;
        }
        else if (str.length() < 5 || str.substr(str.size() - 4, 4) != ".txt") {
            print_help(str);
            return;
        }
        else
            files.push_back(str);
    }
    std::unique_ptr<Classifier> C(new Classifier(files.front()));
    C->learn();
    if (show)
        C->show();
    files.pop_front();
    for (auto file: files) {
        try {
            C->txt_proc(file);
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << "YOLO\n";
            return;
        }
    }
}
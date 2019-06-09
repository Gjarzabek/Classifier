#include "Classifier.hpp"

void Classifier::txt_proc(std::string testfile, std::string outf/* = "" */) {
    if (outf.empty()) {
        outf = testfile.substr(0,testfile.length() - 4) + POSTFIX;
    }
    std::pair<int, int> colrow_size;
    tree->get_exptected_size(colrow_size);
    DataTable test_data;
    try {
        test_data.txt_load(testfile);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return;
    }
    if (test_data[0].size() == colrow_size.first - 1) {
        test_data.add_record(0, "Answer: ");
    }
    else if (test_data[0].size() != colrow_size.first) {
        throw Classfier_except("ERROR: file:" + testfile + " wrong rows number - cannot parse");
    }
    for (unsigned i = 1; i < test_data.get_len(); ++i) {
        std::string answer;
        try {
            answer = tree->answer(test_data[i]) ? "yes" : "no";
        }
        catch(const std::exception & e){
            answer = e.what();
        }
        test_data.add_record(i, answer);
    }
    try {
        test_data.txt_save(outf);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return;
    }
    std::cout << testfile + " result ---> " + outf << std::endl;
}

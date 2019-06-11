#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "DecisionTree.hpp"
#include "DataTable.hpp"
#include <assert.h>
#include "classifier_exception.hpp"
#include <fstream>

#define POSTFIX "_out.txt"

/** Tasks:
 *  --help - print description
 *  learn() - build tree from .txt
 *  ask() - input .txt file with datatable ---> output .txt file with answers
 *  show() - showup constructed tree
 */

class Classifier {
  
   public:
  
    Classifier(std::string src) : src_file(src), tree(nullptr) {}
    ~Classifier() {
      if (tree)
        delete tree;
    }

    void show() const {
      if (tree == nullptr)
        std::cout << "Tree empty.\n";
      else
        tree->print();
    }

    void learn() {
      assert(!src_file.empty());
      try {
        tree = new DecisionTree(src_file);
      }
      catch (const std::exception & e) {
        if (tree)
          delete tree;
        throw Classfier_except(e.what());
      }
    }

    // klasyfikuje plik txt o nazwie inf, wyniki zapisuje w pliku o nazwie outf
    void txt_proc(std::string testfile, std::string outf = "");

  private:

    Classifier() = default;
    std::string src_file;
    DecisionTree * tree;

};

#endif

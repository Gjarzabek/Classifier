#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "DecisionTree.hpp"
#include "DataTable.hpp"

/** Tasks:
 *  --help - print description
 *  learn() - build tree from .txt
 *  ask() - input .txt file with datatable ---> output .txt file with answers
 *  show() - showup constructed tree
 *  ask_single() - 
 *  class detects wrong inputs and infrom about them
 */

class Classifier {
  
   public:
  
    Classifier(): dst_file(""), src_file(""), tree(nullptr) {}
    Classifier(std::string src, std::string dst = "output.txt") : dst_file(dst), src_file(src), tree(nullptr) {}
    ~Classifier() {
      if (tree)
        delete tree;
    }

    void show() const{
      tree->print();
    }

    void learn(std::string filename = "") {
      if (filename.empty())
        filename = src_file;
      tree = new DecisionTree(new DataTable(), filename);
      tree->build();
    }

    bool get_answer(const std::vector<std::string> & tab) const {
      return tree->answer(tab);
    }  

  private:
  
    std::string dst_file;
    std::string src_file;
    DecisionTree * tree;

};

#endif

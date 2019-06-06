#ifndef DECISIONTREE_H
#define DECISIONTREE_H

/**
  description:
  Main task of the class is to take @DataTable object
  and construct decisiontree associated with it.
*/

/*
  Todo:
    *napisać funkcje budującą drzewo
    *napisać funkcje nizszącą drzewo
    *napisać Klase Classifier która uzywa DataTable i DecisionTree
    *
*/


#include "DataTable.hpp"
#include <map>
#include <mutex>
#include <unordered_map>
#include <list>
#include <utility>

#define YES "yes"
#define NO "no"

class DecisionTree {
  private:
    class TreeNode {
      public:
        friend DecisionTree;

        TreeNode(): cat_id(0), value("") {}
        TreeNode(const TreeNode & x) = delete;
        TreeNode(int c_id, std::string nm) : cat_id(c_id), value(nm) {}

        void set(const int & ci, const std::string & n) {
          cat_id = ci;
          value = n;
        }

        const auto & get_children() const;
        std::string get_value() const;
        TreeNode * add_edge(const std::string & edge);
        TreeNode * choose_edge(std::string choice_val);

      private:
        int cat_id;
        std::string value;
        // nazwy typow kategorii
        // np dla pogoda
        // nazwy moga byc: slonecznie, pochmurno, deszczowo itd..
        std::unordered_map<std::string, TreeNode*> children;
    };

    friend TreeNode;
    TreeNode * root;
    // key - info_gain, value - category id in categories vector

    void delete_tree();
    static void print_walk(TreeNode * root, int indenation);
    static void calculate_info_gain(const DataTable & dt, const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result);
    static void column_calculation(double set_ent, double n_pos_rows, const std::list<int> & col_id, const std::list<int> & row_id, const DataTable & dt, std::pair<int, std::string> & result);
    static double set_entropy(int x, int y);
    static bool is_positive(std::string s);
    static bool is_number(const std::string & s);
    static void walk(const DataTable & dt, DecisionTree::TreeNode * root, std::list<int> col_id, std::list<int> row_id);

  public:

    DecisionTree() :root(new TreeNode()) {}
    ~DecisionTree() {
      //delete_tree();
    }
    void build(const DataTable & dt);
    bool ask(std::vector<std::string> v) const;
    void print() const;

};

#endif

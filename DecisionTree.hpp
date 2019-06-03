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
    *napisać Klase Classifier która uzywa DataTable i DecisionTre6e
    *
*/


#include "DataTable.hpp"
#include <map>
#include <mutex>
#include <unordered_map>
#include <utility>

#define YES "yes"
#define NO "no"

class DecisionTree {
  public:

    DecisionTree() :root(nullptr) {}
    ~DecisionTree() {
      delete_tree();
    };
    void build(const DataTable & dt);
    bool ask(std::vector<std::string> v) const;

  private:
    class TreeNode {
      public:

        TreeNode(): cat_id(0), value("");
        TreeNode(const TreeNode & x) = delete;
        TreeNode(int c_id, std::string nm) : cat_id(c_id), value(nm) {}

        void set(const int & ci, const std::string & n) {
          cat_id = ci;
          name = n;
        }

        const auto & get_children() const;
        std::string get_value() const;
        template <typename T>
        void add_edge(const std::string & edge, int id, std::string node_val);
        template <typename T>
        auto choose_edge(T choice_val);

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

    void calculate_info_gain(const DataTable & dt);
    void delete_tree();
    void column_calculation(double seten, double pn, int p, int q, const DataTable & dt);
    static double set_entropy(int x, int y);
    static bool is_positive(std::string s);
    static bool is_number(const std::string & s);
    static void walk(const DataTable & dt, TreeNode * root, std::list<int> indiecies);

};

#endif

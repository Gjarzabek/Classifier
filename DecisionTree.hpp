#ifndef DECISIONTREE_H
#define DECISIONTREE_H

/**
  description:
  Main task of the class is to take @DataTable object
  and construct decisiontree associated with it.
*/

#include "DataTable.hpp"
#include <list>
#include <unordered_map>

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
        enum Type {
          CONDITION,
          CATEGORY
        };

        TreeNode() = delete;
        TreeNode(const TreeNode & x) = delete;
        TreeNode(int c_id, std::string nm) : cat_id(c_id), node_type(CATEGORY), name(nm), value(0) {}
        TreeNode(int c_id, int w) : cat_id(c_id), node_type(CONDITION), name(""),value(w) {}

        const auto & get_children() const;
        int get_value() const;
        std::string get_name() const;
        void add_edge(const std::string & s, TreeNode * child);
        template <typename T>
        auto choose_edge(T choice_val);

      private:
        const int cat_id;
        const Type node_type;
        const std::string name;
        const int value;
        std::unordered_map<std::string, TreeNode*> children;
    };
    TreeNode * root;
    std::list<int> categories_ids;
    // funkcja inicjalizuje @categories_ids indeksami kategorii
    // "przyrost informacji" kolejnych kategorii maleje
    void calculate_info_gain(const DataTable & dt);
    void delete_tree();
};

#endif
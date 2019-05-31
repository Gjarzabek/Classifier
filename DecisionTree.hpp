#ifndef DECISIONTREE_H
#define DECISIONTREE_H

/**
  description:
  Main task of the class is to take @DataTable object
  and construct decisiontree associated with it.
*/

#include "DataTable.hpp"

class DecisionTree {
  public:

    DecisionTree() :root(nullptr) {}
    ~DecisionTree() {
      delete_tree();
    };
    void construct(const DataTable & dt);
    bool ask(std::vector<std::string> v) const;

  private:
    class TreeNode {
      public:
        enum Type {
          CONDITION,
          CATEGORY
        };

        TreeNode() = 0;
        TreeNode(TreeNode x) = 0;
        TreeNode(int c_id, std::string nm) : cat_id(c_id), node_type(CATEGORY), name(nm) {}
        TreeNode(int c_id, int w) : cat_id(c_id), node_type(CONDITION), value(w) {}

        const auto & get_children() const;
        int get_value() const;
        std::string get_name() const;
        void add_edge(const std::string & s, TreeNode * child);

        template <typename T>
        auto choose_edge(T choice_val);
        // dodatkowo funkcja pozwalajaca wybrac odpowiednia galaz
        // w drodze do liscia ?
      private:
        const int cat_id;
        const Type node_type;
        const std::string name;
        const int value;
        std::vector<std::pair<std::string, TreeNode *>> children;
    };
    TreeNode * root;

    void delete_tree();
}

#endif

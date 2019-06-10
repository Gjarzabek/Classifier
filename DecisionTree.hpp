#ifndef DECISIONTREE_H
#define DECISIONTREE_H

/**
  description:
  Main task of the class is to take @DataTable object
  and construct decisiontree associated with it.
*/

#include "DataTable.hpp"
#include <unordered_map>
#include <list>
#include <utility>



class DecisionTree {
  
  private:
  
    class TreeNode {
      public:
        friend DecisionTree;

        TreeNode(): cat_id(0), value("") {}
        TreeNode(const TreeNode & x) = delete;
        TreeNode(int c_id, std::string nm) : cat_id(c_id), value(nm) {}
        TreeNode * add_edge(const std::string & edge);
        TreeNode * choose_edge(std::string choice_val);
        
        const std::unordered_map<std::string, TreeNode*> & get_children() const;
        std::string get_value() const;

        void n_set(const int & ci, const std::string & n) {
          cat_id = ci;
          value = n;
        }
        int get_cat() {
          return cat_id;
        }

      private:
        int cat_id;
        std::string value;
        std::unordered_map<std::string, TreeNode*> children;
    };

    friend TreeNode;

    //DataTable from which tree is built
    DataTable * dt;
    TreeNode * root;
    
    DecisionTree() = default;
    DecisionTree(const DecisionTree & b) = default;
    
    void delete_tree(TreeNode * root);
    
    void print_walk(TreeNode * node, int indenation) const;
    
    void calculate_info_gain(const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result);
    
    void column_calculation(double set_ent, double n_pos_rows, const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result);
    
    void walk(TreeNode * root, std::list<int> col_id, std::list<int> row_id);
    
    std::pair<std::string, std::pair<int, int>> balance(int col_id, std::list<int> rows, int positive_records);
    
    static double set_entropy(double x, double y);
    
    static bool is_positive(std::string s);
    
    static bool is_number(const std::string & s);

    static bool answer_walk(const std::vector<std::string> & v, TreeNode * node);
  
  public:

    DecisionTree(std::string fn) :root(new TreeNode()) {
      dt = new DataTable();
      dt->txt_load(fn);
      build();
    }
    ~DecisionTree() {
      if (dt)
        delete dt;
      if (root)
        delete_tree(root);
    }
    void build();
    bool answer(const std::vector<std::string> & v) const;
    void print() const;
    bool empty() const {
      return root->get_children().size() == 0;
    }
    void get_exptected_size(std::pair<int, int> & colrow) {
      colrow.second = dt->get_len();
      colrow.first = (*dt)[0].size();
    }
};

#endif

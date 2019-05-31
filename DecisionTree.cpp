#include "DecisionTree.h"

const auto & DecisionTree::TreeNode::get_children() const {
  return children;
}

int DecisionTree::TreeNode::get_value() const {
  if (node_type == CONDITION) {
    return value;
  }
  else return -1;
}

std::string DecisionTree::TreeNode::get_name() const {
  if (node_type == CATEGORY)
    return name;
  else return "";
}

void DecisionTree::TreeNode::add_edge(const std::string & s, TreeNode * child) {
  children.insert({s, children});
}

template <typename T>
auto DecisionTree::TreeNode::choose_edge(T choice_val) {
  if (node_type == CONDITION) {
    if (typeid(choice_val) != typeid(value))
      return nullptr;
  }
  else {
    if (typeid(choice_val) != typeid(name))
      return nullptr;
  }
  if (children.find(choice_val) == children.end())
    return nullptr;
  else
    return children[choice_val];
}

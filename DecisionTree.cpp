#include "DecisionTree.hpp"
#include <algorithm>
#include <cctype>
#include <math.h>
#include <string>
#include <list>
#include <exception>

#define YES "yes"
#define NO "no"
#define MISS 0.96


const std::unordered_map<std::string, DecisionTree::TreeNode*> & DecisionTree::TreeNode::get_children() const {
  return children;
}


std::string DecisionTree::TreeNode::get_value() const {
    return value;
}


DecisionTree::TreeNode * DecisionTree::TreeNode::add_edge(const std::string & s) {
  TreeNode * child = new TreeNode();
  children.insert(std::make_pair(s, child));
  return child;
}


DecisionTree::TreeNode * DecisionTree::TreeNode::choose_edge(std::string choice_val) {
  if (!is_number(choice_val))
    return children[choice_val];
  else if (is_number(value)){
    int chv = std::stoi(choice_val);
    bool greater = std::stoi(value) >= chv;
    return greater ? children[YES] : children[NO];
  }
  else return nullptr;
}


bool DecisionTree::is_number(const std::string& s) {
    if (s.empty())
      return false;
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


bool DecisionTree::is_positive(std::string s) {
  if (s.empty())
    return false;
  else if (is_number(s)) {
    return s == "1";
  }
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s == YES || s == "true";
}


double DecisionTree::set_entropy(double positive_num, double row_num) {
  if (positive_num == row_num || positive_num == 0)
    return 0;
  double x = static_cast<double>(positive_num / row_num);
  double y = static_cast<double>((positive_num - row_num) / row_num);
  x = std::abs(x);
  y = std::abs(y);
  if (x == 0 || y == 0) {
    return 0;
  }
  double result = -(x * log10(std::abs(x)) + y * log10(std::abs(y)));
  return result;
}

bool DecisionTree::answer_walk(const std::vector<std::string> & v, TreeNode * node) {
  if (node) {
    if (node->get_cat() < 0) {
      return is_positive(node->get_value());
    }
    else {
      std::string next_cat = v[node->get_cat()];
      auto sibilings = node->get_children();
      if (!sibilings.empty() && !is_number(next_cat) && sibilings.find(next_cat) == sibilings.end())
        throw Classfier_except("MISSING RECORD: " + next_cat);
      return answer_walk(v, node->choose_edge(next_cat));
    }
  }
  return false;
}

bool DecisionTree::answer(const std::vector<std::string> & v) const {
  return answer_walk(v, root);
}


void DecisionTree::calculate_info_gain(const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result) {
  if ((*dt).get_len() < 1) {
    result.first = -1;
    result.second = "ERROR: empty container (*dt)/row_id";
    return;
  }
  unsigned positive_num = 0;
  for (int j: row_id) {
    if (is_positive(((*dt)[j])[(*dt)[j].size() - 1])) {
      ++positive_num;
    }
  }
  double pos = static_cast<double>(positive_num) / row_id.size();
  double neg = static_cast<double>(row_id.size() - positive_num) / (double)row_id.size();
  if (pos >=  MISS) {
    result.first = -1;
    result.second = YES;
  }
  else if (neg >= MISS) {
    result.first = -1;
    result.second = NO;
  }
  else {
    double set_i = set_entropy(positive_num, row_id.size());
    column_calculation(set_i, positive_num, col_id, row_id, result);
  }
}

std::pair<std::string, std::pair<int, int>> DecisionTree::balance(int col_id, std::list<int> rows, int positive_records) {
  std::vector<std::pair<int, bool>> records;
  for (int i: rows)
    records.push_back(std::make_pair(std::stoi(((*dt)[i])[col_id]), is_positive(((*dt)[i])[(*dt)[i].size() - 1])));
  std::sort(records.begin(), records.end(), [](const std::pair<int, bool> & a, const std::pair<int, bool> & b) -> bool {
      return a.first < b.first;
  });
  auto lbalance = [] (int lp, int ln, int rp, int rn) -> int { return std::abs(lp-ln) + std::abs(rp-rn); };
  int l_pos = 0, l_neg = 0, r_pos = positive_records, r_neg = records.size() - positive_records;
  int max_balance = 0;
  int max_balance_id = 0;
  bool positive_rightside = false;
  int x, y;
  for (unsigned i = 0; i < records.size(); ++i) {
    if (records[i].second) {
      ++l_pos;
      --r_pos;
    }
    else {
      ++l_neg;
      --r_neg;
    }
    int cb = lbalance(l_pos, l_neg, r_pos, r_neg);
    if (cb >= max_balance) {
      max_balance = cb;
      max_balance_id = i;
      positive_rightside = r_pos > r_neg;
      x = l_neg+r_pos, y = l_pos+r_neg;
      if (!positive_rightside)
        std::swap(x, y);
    }
  }
  std::string result_record;
  if (max_balance_id + 1 < records.size()) {
    int divider = (records[max_balance_id].first + records[max_balance_id + 1].first) / 2;
    result_record = std::to_string(divider);
  }
  else {
    result_record = std::to_string(records[max_balance_id].first);
  }
  return std::make_pair(result_record, std::make_pair(x, y));
}

void DecisionTree::column_calculation(double set_ent, double n_pos_rows, const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result)  {
  double best_i_gain = 0;
  std::string cat_name;
  int best_id = 0;
  for(int p: col_id) {
    std::unordered_map<std::string, std::pair<int, int>> partition; // categorie name + pair<pozytywne przypadki, negatyw>
    bool comparsion_node = is_number((*dt)[1][p]);
    if (!comparsion_node) {
      for (int i: row_id)  {
        const auto & curr_row = (*dt)[i];
        bool positive = is_positive(curr_row[curr_row.size() - 1]);
        if (partition.find(((*dt)[i])[p]) != partition.end())
          positive ? partition[((*dt)[i])[p]].first += 1 : partition[((*dt)[i])[p]].second += 1;
        else
          partition[((*dt)[i])[p]] = std::make_pair(positive, !positive);
      }
    }
    else {
        partition.insert(balance(p, row_id, n_pos_rows));
    }
    double entropy = 0;
    for(auto el: partition) {
      double c = set_entropy(el.second.first, el.second.second + el.second.first);
      entropy += c * (el.second.second + el.second.first) / (*dt).get_len();
    }
    double i_gain = set_ent - entropy;
    if (i_gain > best_i_gain) {
      best_i_gain = i_gain;
      best_id = p;
      cat_name = comparsion_node ? partition.begin()->first : (*dt)[0][p];
    }
  }
  result.first = best_id;
  result.second = cat_name;
}

void DecisionTree::walk(DecisionTree::TreeNode * root, std::list<int> col_id, std::list<int> row_id) {
  if (root && !row_id.empty()) {
    std::pair<int, std::string> result;
    result.first = -1;
    calculate_info_gain(col_id, row_id, result);
    if (result.first < 0 || result.second.empty()) {
      root->n_set(-1, result.second);
    }
    else if (is_number(result.second)) {
      root->n_set(result.first, result.second);
      col_id.remove(result.first);
      std::list<int> pos, neg;
      for (int i: row_id) {
        if (std::stoi(result.second) >= std::stoi((*dt)[i][result.first])) {
          pos.push_back(i);
        }
        else {
          neg.push_back(i);
        }
      }
      TreeNode * child = root->add_edge(NO);
      walk(child, col_id, neg);
      child = root->add_edge(YES);
      walk(child, col_id, pos);
    }
    else {
      root->n_set(result.first, result.second);
      col_id.remove(result.first);
      std::unordered_map<std::string, std::list<int>> used;
      for (int i : row_id) {
        if (used.find(((*dt)[i])[result.first]) == used.end()) {
          used[((*dt)[i])[result.first]] = {i};
        }
        else {
          used[((*dt)[i])[result.first]].push_back(i);
        }
      }
      for (auto p: used) {
        TreeNode * child = root->add_edge(p.first);
        walk(child, col_id, p.second);
      }
    }
  }
}

void DecisionTree::delete_tree(DecisionTree::TreeNode * node) {
  if (node) {
    for (auto child: node->get_children()) {
      delete_tree(child.second);
    }
    delete node;
  }
}

void DecisionTree::build() {
  std::list<int> row_indiecies;
  for (unsigned i = 1; i < (*dt).get_len(); ++i)
    row_indiecies.push_back(i);
  std::list<int> col_indieces;
  for (unsigned i = 0; i < (*dt)[0].size() - 1; ++i) {
    col_indieces.push_back(i);
  }
  walk(root, col_indieces, row_indiecies);
}

void DecisionTree::print_walk(TreeNode * node, int indenation) const{
  if (node) {
    for (int i = 0; i < indenation; ++i)
      std::cout << "\t";
    if (node->get_cat() < 0) {
      std::cout<< "answer: " << node->get_value() << ".\n";
    }
    else {
      std::cout << "Node<-";
      is_number(node->get_value()) ? std::cout<< (*dt)[0][node->get_cat()]  << " ? (" << node->get_value()<<" >= x)\n" : std::cout << node->get_value() << '\n';
      for (auto ch: node->get_children()) {
         for (int i = 0; i < indenation; ++i)
            std::cout << '\t';
          std::cout << "|~~~~~~~~ "<< ch.first << '\n';
          print_walk(ch.second, indenation + 1);
      }
    }
  }
}

void DecisionTree::print() const {
  if (empty()) {
    std::cout << "Tree empty.\n";
    return;
  }
  std::cout << "#### DecisionTree ####\n";
  print_walk(root, 0);
}

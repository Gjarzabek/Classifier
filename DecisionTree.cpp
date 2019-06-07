#include "DecisionTree.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <math.h>
#include <string>
#include <list>
#include <exception>

#define THREADS 4

const auto & DecisionTree::TreeNode::get_children() const {
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
    bool greater = chv > std::stoi(value);
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


double DecisionTree::set_entropy(int positive_num, int row_num) {
  double result=0;
  double x = abs(positive_num / row_num);
  double y = abs((positive_num - row_num) / row_num);
  result = -(x * log10(x) + y * log10(y));
  return result;
}

// oblicza najlepszego kandydata sposrod kategorii S na kolejny wezel
// zwraca jej [indeks, nazwe]
void DecisionTree::calculate_info_gain(const DataTable & dt, const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result) {
  if (dt.get_len() < 1) {
    return;
  }
  unsigned line_len = col_id.size();
  unsigned row_num = row_id.size();
  unsigned positive_num = 0;
  for (int j: row_id) {
    if (is_positive((dt[j])[line_len])) {
      ++positive_num;
    }
  }
  if (positive_num == row_num) {
    result.first = -1;
    result.second = YES;
  }
  else if (positive_num == 0) {
    result.first = -1;
    result.second = NO;
  }
  else {
    double set_i = set_entropy(positive_num, row_num);
    std::cout<<"set_i = " << set_i << "\n";
    column_calculation(set_i, positive_num, col_id, row_id, dt, result);
  }
}

//chosing best coulmn
void DecisionTree::column_calculation(double set_ent, double n_pos_rows, const std::list<int> & col_id, const std::list<int> & row_id, const DataTable & dt, std::pair<int, std::string> & result) {
  int best_i_gain = 0;
  std::string cat_name;
  int best_id = 0;
  for(int p: col_id) {
    std::unordered_map<std::string, std::pair<int, int>> partition; // categorie name + pair<pozytywne przypadki, negatyw>
    int max_bi = 0;
    bool comparsion_node = is_number(dt[1][p]);
    if (!comparsion_node) {
      for (int i: row_id) {
        const auto & curr_row = dt[i];
        bool positive = is_positive(curr_row[curr_row.size() - 1]);
        if (partition.find((dt[i])[p]) != partition.end())
          positive ? partition[(dt[i])[p]].first += 1 : partition[(dt[i])[p]].second += 1;
        else
          partition[(dt[i])[p]] = std::make_pair(positive, positive);
      }
    }
    else {
        std::vector<std::pair<int, bool>> nums;
        for (unsigned i = 1; i < dt.get_len(); ++i)
          nums.push_back(std::make_pair(std::stoi((dt[i])[p]), is_positive((dt[i])[p])));
        std::sort(nums.begin(), nums.end(), [](const std::pair<int, bool> & a, const std::pair<int, bool> & b) -> bool {
            return a.first < b.first;
        });
        auto balance = [] (int lp, int ln, int rp, int rn) -> int { return std::abs(lp-ln) + std::abs(rp-rn); };
        int l_pos = 0, l_neg = 0, r_pos = n_pos_rows, r_neg = nums.size() - n_pos_rows;
        int max_balance = 0;
        bool positive_rightside = false;
        for (int i: row_id) {
          if (nums[i].second) {
            ++l_pos;
            --r_pos;
          }
          else {
            ++l_neg;
            --r_neg;
          }
          int cb = balance(l_pos, l_neg, r_pos, r_neg);
          if (cb > max_balance) {
            max_balance = cb;
            max_bi = i;
            positive_rightside = r_pos > r_neg;
          }
        }
        int x,y;
        if (positive_rightside) { x=l_neg+r_pos; y=l_pos+r_neg; }
        else { x=l_pos+r_neg; y=l_neg+r_pos; }
        partition.insert(std::make_pair(std::to_string(nums[max_bi].first), std::make_pair(x, y)));
    }
    // entropy calculation
    double entropy = 0;
    for(auto el: partition) {
      double c = set_entropy(el.second.first, el.second.second + el.second.first);
      entropy += c * (el.second.second + el.second.first) / dt.get_len();
    }
    int i_gain = set_ent - entropy;
    if (i_gain > best_i_gain) {
      best_i_gain = i_gain;
      best_id = p;
      cat_name = comparsion_node ? partition.begin()->first : dt[0][p];
    }
  }
  result.first = best_id;
  result.second = cat_name;
  ;
}

//recursive creation
void DecisionTree::walk(const DataTable & dt, DecisionTree::TreeNode * root, std::list<int> col_id, std::list<int> row_id) {
  if (root) {
    std::pair<int, std::string> result;
    result.first = -1;
    calculate_info_gain(dt, col_id, row_id, result);
    std::cout << "Wezel: " << result.second << std::endl;
    if (result.first < 0 || result.second.empty()) {
      root->set(-1, result.second);
    }
    else {
      root->set(result.first, result.second);
      col_id.remove(result.first);
      std::unordered_map<std::string, std::list<int>> used;
      for (int i = 1; (unsigned)i < dt.get_len(); ++i) {
        if (used.find((dt[i])[result.first]) == used.end()) {
          used[(dt[i])[result.first]] = {i};
        }
        else {
          used[(dt[i])[result.first]].push_back(i);
        }
      }
      for (auto p: used) {
        TreeNode * child = root->add_edge(p.first);
        walk(dt, child, col_id, row_id);
      }
    }
  }
}


// linked lista z dostepnymi indeksami kategorii
void DecisionTree::build(const DataTable & dt) {
  std::list<int> col_indieces, row_indiecies;
  for (unsigned i = 1; i < dt.get_len(); ++i)
    row_indiecies.push_back(i)  ;
  for (unsigned i = 0; i < dt[0].size() - 1; ++i) {
    col_indieces.push_back(i);
  }
  walk(dt, root, col_indieces, row_indiecies);
}

void DecisionTree::print_walk(TreeNode * node, int indenation) {
  if (node) {
    for (int i = 0; i < indenation; ++i)
      std::cout << "  ";
    std::cout << ":";
    is_number(node->value) ? std::cout<<node->value<<">\n" : std::cout << node->value << '\n';
    for (auto ch: node->get_children()) {
        for (int i = 0; i < indenation; ++i)
          std::cout << ' ';
        std::cout << ch.first << '\n';
        print_walk(ch.second, indenation + 1);
    }
  }
}

void DecisionTree::print() const {
  print_walk(root, 0);
}

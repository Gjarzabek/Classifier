YES#include "DecisionTree.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <string>
#include <list>

#define THREADS 4

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


DecisionTree::TreeNode * DecisionTree::TreeNode::add_edge(const std::string & s) {
  TreeNode * child = new TreeNode();
  children.insert(std::make_pair(s, child));
  return child;
}


auto DecisionTree::TreeNode::choose_edge(std::string choice_val) {
  if (!is_number(choice_val))
    return children[choice_val];
  else {
    int chv = std::stoi(choice_val)
    bool greater = chv > value;
    return greater ? children[YES] : children[NO];
  }
}


bool DecisionTree::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


bool DecisionTree::is_positive(std::string s) {
  if (is_number(s)) {
    return s == "1";
  }
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s == YES || s == "true";
}


double DecisionTree::set_entropy(int pn, int c_len) {
  double result = -((double)(pn/c_len) * log(pn/c_len) + ((c_len-pn)/c_len) * log((c_len-pn)/c_len));
  return result;
}

// oblicza najlepszego kandydata sposrod kategorii S na kolejny wezel
// zwraca jej [indeks, nazwe]
void DecisionTree::calculate_info_gain(const DataTable & dt, const auto & col_id, const auto & row_id, auto & result) {
  categories_ids.clear();
  if (dt.get_len() < 1) {
    return;
  }
  unsigned line_len = (dt[0]).size();
  unsigned col_len = s.size();
  unsigned positive_num = 0;
  for (int j: s) {
    if (is_positive(dt[j][line_len - 1]))
      ++positive_num;
  }
  // is leaf
  if (positive_num == col_len) {
    result.first = -1;
    result.second = YES;
    return;
  }
  else if (positive_num == 0) {
    result.first = -1;
    result.second = NO;
    return;
  }
  double set_i = set_entropy(positive_num, col_len-1);
  column_calculation(set_i, positive_num, col_id, row_id, dt, result);
}

//chosing best coulmn
void DecisionTree::column_calculation(double set_ent, double n_pos_rows, const auto & col_id, const auto & row_id, const DataTable & dt, auto & result) {
  int best_i_gain = 0;
  string cat_name = "";
  int best_id = 0;
  for(int p: col_id) {
    // ta haszmapa przydalaby sie do obliczania krawedzi
    std::unordered_map<std::string, std::pair<int, int>> partition(dt.get_len()); // categorie name + pair<pozytywne przypadki, negatyw>
    int max_bi = 0;
    bool comparsion_node = is_number(dt[1][p]);
    if (!comparsion_node) {
      for (int i: row_id) {
        bool positive = is_positive(dt[i][dt.get_len() - 1]);
        if (partition.find(dt[i][p]) != partition.end())
          positive ? partition[dt[i][p]].first += 1 : partition[dt[i][p]].second += 1;
        else
          partition[dt[i][p]] = std::make_pair(positive, positive);
      }
    }
    else {
        std::vector<std::pair<int, bool>> nums(dt.get_len() - 1);
        for (unsigned i = 1; i < dt.get_len(); ++i)
          nums[i] = std::make_pair(std::stoi(dt[i][p]), is_positive(dt[i][p]));
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
      // jesli c 1 lub 0 to znaczy ze ma gotowego liscia z odpowiedzia tak lub nie
      // else musze sprawdzic kolejna kategorie
      entropy += c * (el.second.second + el.second.first) / dt.get_len();
    }
    make_pair(set_ent - entropy, std::make_pair(p, max_bi)));
    int i_gain = set_ent - entropy;
    if (i_gain > best_i_gain) {
      best_i_gain = i_gain;
      best_id = p;
      cat_name = comparsion_node ? partition.begin()->first : partition[0][p];
    }
  }
  result.first = best_id;
  result.second = cat_name;
}

//recursive creation
void DecisionTree::walk(const DataTable & dt, auto * root, auto col_id, auto row_id) {
  if (root) {
    std::pair<int, std::string> result(-1, "");
    calculate_info_gain(dt, col_id, row_id, result);
    if (indieces.first < 0) {
      root->set(-1, indieces.second);
    }
    else {
      root->set(result.frist, result.second);
      col_id.remove(result.first);
      std::unordered_map<std::string, std::list<int>> used;
      for (unsigned i = 1; i < dt.get_len(); ++i) {
        if (used.find(dt[i][result.first]) == used.end()) {
          used[dt[i][result.first]] = {i};
        }
        else {
          used[dt[i][result.first]].push_back(i);
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
  for (unsigned i = 0; i < dt.get_len() - 1; ++i)
    row_indiecies.push_back(i);
  for (unsigned i = 0; i < dt[0].size() - 1; ++i) {
    col_indieces.push_back(i);
  }
  walk(dt, root, col_indieces, row_indiecies);
}

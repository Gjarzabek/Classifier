#include "DecisionTree.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <string>

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


void DecisionTree::TreeNode::add_edge(const std::string & s, TreeNode * child) {
  children.insert(std::make_pair(s, child));
}


template <typename T>
auto DecisionTree::TreeNode::choose_edge(T choice_val) {
  if (node_type == CATEGORY) {
    if (typeid(choice_val) != typeid(value))
      return nullptr;
    if (children.find(choice_val) == children.end())
      return nullptr;
    else
      return children[choice_val];
  }
  else {
    if (typeid(choice_val) != typeid(name))
      return nullptr;
    bool greater = choice_val > value;
    return greater ? children[YES] : children[NO];
  }

}


bool DecisionTree::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


bool DecisionTree::is_positive(std::string s){
  if (is_number(s)) {
    return s == "1";
  }
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s == "yes" || s == "true";
}


double DecisionTree::set_entropy(int pn, int c_len) {
  double result = -((double)(pn/c_len) * log(pn/c_len) + ((c_len-pn)/c_len) * log((c_len-pn)/c_len));
  return result;
}


void DecisionTree::calculate_info_gain(const DataTable & dt) {
  categories_ids.clear();
  if (dt.get_len() < 1) {
    std::cout << "Datatable.size() < 1\n";
    return;
  }
  unsigned line_len = (dt[0]).size();
  unsigned col_len = dt.get_len();
  unsigned positive_num = 0;
  for (unsigned j = 1; j < col_len; ++j) {
    if (is_positive(dt[j][line_len - 1]))
      ++positive_num;
  }
  double set_i = set_entropy(positive_num, col_len-1);
  if (line_len - 1 <= THREADS) {
    std::thread th[line_len];
    for (unsigned i = 0; i < line_len - 1; ++i)
      th[i] = std::thread(&DecisionTree::column_calculation, this, set_i, positive_num, i, i, dt);
    for (unsigned i = 0; i < line_len; ++i)
      th[i].join();
  }
  else {
    std::thread th[THREADS];
    int col_per_th = (line_len - 1)/ THREADS;
    int p = 0;
    int q = col_per_th - 1;
    int i;
    for (i = 0; i < THREADS - 1; ++i)  {
      th[i] = std::thread(&DecisionTree::column_calculation, this, set_i, positive_num, p, q, dt);
      p += col_per_th;
      q += col_per_th;
    }
    th[i] = std::thread(&DecisionTree::column_calculation, this, set_i, positive_num, p, line_len - 1, dt);
    // dodac kiedys wspolbiezne tworzenie drzewa przez ten wątek
    // by nie czekal bezczynnie
    for (unsigned i = 0; i < line_len; ++i)
      th[i].join();
  }
}


void DecisionTree::column_calculation(double set_ent, double n_pos_rows, int p, int q, const DataTable & dt) {
  while (p <= q) {
    std::unordered_map<std::string, std::pair<int, int>> partition; // 6categorie name + pair<pozytywne przypadki, negatu>
    if (!is_number(dt[1][p])) {
      for (unsigned i = 1; i < dt.get_len(); ++i) {
        bool positive = is_positive(dt[i][dt.get_len() - 1]);
        if (partition.find(dt[i][p]) != partition.end())
          positive ? partition[dt[i][p]].first += 1 : partition[dt[i][p]].second += 1;
        else
          partition[dt[i][p]] = std::make_pair(positive, positive);
      }
    }
    int max_bi = 0;
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
        for (unsigned i = 0; i < nums.size(); ++i) {
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
    for(auto p: partition) {
      double c = set_entropy(p.second.first, p.second.second + p.second.first);
      entropy += c * (p.second.second + p.second.first) / dt.get_len();
    }
    multimap_mut.lock();
    categories_ids.insert(std::make_pair(set_ent - entropy, std::make_pair(p, max_bi)));
    multimap_mut.unlock();
    ++p;
  }
}

//recursive creation
void DecisionTree::walk(const DataTable & dt) {

}

// zmienic DataTable na linked liste
// Funkcja calculate_info_gain musi byc wywolywana przy kazdej
// budowie nowego wezla
void DecisionTree::build(const DataTable & dt) {
  calculate_info_gain(dt);
}

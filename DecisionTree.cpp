#include "DecisionTree.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <utility>
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

bool DecisionTree::is_positive(std::string s) const{
  if (is_number(s)) {
    return s != "0";
  }
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s == "yes" || s == "true";
}

double DecisionTree::set_info(int pn, int c_len) {
  double result = -((double)(pn/c_len) * log(pn/c_len) + ((c_len-pn)/c_len) * log((c_len-pn)/c_len));
  return result;
}

void DecisionTree::calculate_info_gain(const DataTable & dt) {
  categories_ids.clear();
  if (dt.get_len() < 1) {
    std::cout << "Datatable.size() < 1\n";
    return;
  }
  // obliczaj info gain dla kazdej kategorii odzielnie i dodawaj do multimapy
  // multimapa posortowana
  // info jakie potrzebuje:
  // ilosc odpowiedzi twierdzacych i przeczacych
  // dla kazdej kolumny - ilosc odpowiedzi yes/no dla kazdej podkategorii
  unsigned line_len = (dt[0]).size();
  unsigned col_len = dt.get_len();
  //kontener dla pierwzego obrotu pętli
  unsigned positive_num = 0;
  for (unsigned j = 1; j < col_len; ++j) {
    if (is_positive(dt[j][line_len - 1]))
      ++positive_num;
  }
  double set_i = set_info(positive_num, col_len-1);
  // podzial dt na czesci i zlecenie kazdemu watkowi kalkulacje tej czesci
}

void DecisionTree::column_calculation(double seti, double posnum, int p, int q, const DataTable & dt) {
  while (p <= q) {
    double entropy = 0;
    std::unordered_map<std::string, std:pair<int, int>> partition; // pair<true_n, false_n>
    if (!is_number(dt[1][p])) {
      for (unsigned i = 1; i < dt.get_len(); ++i) {
        bool positive = is_positive(dt[i][dt.get_len() - 1]);
        if (partition.find(dt[i][p]) != partition.end())
          positive ? partition[dt[i][p]].first += 1 : partition[dt[i][p]].second += 1;
        else
          partition[dt[i][p]] = std:make_pair(positive, positive);
      }
    }
    else {
        std::vector<std::pair<int, bool>> nums(dt.get_len() - 1);
        for (unsigned i = 1; i < dt.get_len(); ++i)
          nums[i] = make_pair(std::stoi(dt[i][p]), is_positive(dt[i][p]));
        std::sort(nums.begin(), nums.end(), [](const std::pair<int, bool> & a, const std::pair<int, bool> & b) -> bool {
            return a.first < b.first;
        });
        // szukam indeksu w tablicy z maksymalnym wspolczynnikiem rownowagi
        // balance =
        int l_pos = 0, l_neg = 0, r_pos = posnum, r_neg = nums.size() - posnum;
        // definicja funkcji balance - lambda
        for (unsigned i = 0; i < nums.size(); ++i) {
          if (nums[i].second) {

          }
        }
    }
    ++p;
  }
}

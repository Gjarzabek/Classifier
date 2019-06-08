#include "DecisionTree.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <math.h>
#include <string>
#include <list>
#include <exception>

#define THREADS 4

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


double DecisionTree::set_entropy(double positive_num, double row_num) {
  if (positive_num == row_num || positive_num == 0)
    return 0;
  double result=0;
  double x = static_cast<double>(positive_num / row_num);
  double y = static_cast<double>((positive_num - row_num) / row_num);
  x = std::abs(x);
  y = std::abs(y);
  if (x == 0 || y == 0) {
    return 0;
  }
  result = -(x * log10(std::abs(x)) + y * log10(std::abs(y)));
  return result;
}

// oblicza najlepszego kandydata sposrod kategorii S na kolejny wezel
// zwraca jej [indeks, nazwe]
void DecisionTree::calculate_info_gain(const DataTable & dt, const std::list<int> & col_id, const std::list<int> & row_id, std::pair<int, std::string> & result) {
  if (dt.get_len() < 1) {
    return;
  }
  unsigned positive_num = 0;
  for (int j: row_id) {
    if (is_positive((dt[j])[dt[j].size() - 1])) {
      ++positive_num;
    }
  }
  if (positive_num == row_id.size()) {
    result.first = -1;
    result.second = YES;
  }
  else if (positive_num == 0) {
    result.first = -1;
    result.second = NO;
  }
  else {
    double set_i = set_entropy(positive_num, row_id.size());
    column_calculation(set_i, positive_num, col_id, row_id, dt, result);
  }
}


// para inddeksu z najlepszym balancem i liczba przypadkow TRUE i FALSE
std::pair<std::string, std::pair<int, int>> DecisionTree::balance(const DataTable & dt, int col_id, std::list<int> rows, int positive_records) {
  std::vector<std::pair<int, bool>> records;
  for (int i: rows)
    records.push_back(std::make_pair(std::stoi((dt[i])[col_id]), is_positive((dt[i])[dt[i].size() - 1])));
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
    if (cb > max_balance) {
      max_balance = cb;
      max_balance_id = i;
      positive_rightside = r_pos > r_neg;
      x = l_neg+r_pos, y = l_pos+r_neg;
      if (!positive_rightside)
        std::swap(x, y);
    }
  }
  return std::make_pair(std::to_string(records[max_balance_id].first), std::make_pair(x, y));
}

//chosing best coulmn
void DecisionTree::column_calculation(double set_ent, double n_pos_rows, const std::list<int> & col_id, const std::list<int> & row_id, const DataTable & dt, std::pair<int, std::string> & result)  {
  // Czemu wybiera 85 jako pierwszy wezel
  double best_i_gain = 0;
  std::string cat_name;
  int best_id = 0;
  for(int p: col_id) {
    std::unordered_map<std::string, std::pair<int, int>> partition; // categorie name + pair<pozytywne przypadki, negatyw>
    bool comparsion_node = is_number(dt[1][p]);
    if (!comparsion_node) {
      for (int i: row_id)  {
        const auto & curr_row = dt[i];
        bool positive = is_positive(curr_row[curr_row.size() - 1]);
        if (partition.find((dt[i])[p]) != partition.end())
          positive ? partition[(dt[i])[p]].first += 1 : partition[(dt[i])[p]].second += 1;
        else
          partition[(dt[i])[p]] = std::make_pair(positive, !positive);
      }
    }
    else {
        partition.insert(balance(dt, p, row_id, n_pos_rows));
    }
    // entropy calculation
    double entropy = 0;
    for(auto el: partition) {
      double c = set_entropy(el.second.first, el.second.second + el.second.first);
      entropy += c * (el.second.second + el.second.first) / dt.get_len();
    }
    double i_gain = set_ent - entropy;
    if (i_gain > best_i_gain) {
      best_i_gain = i_gain;
      best_id = p;
      cat_name = comparsion_node ? partition.begin()->first : dt[0][p];
    }
  }
  result.first = best_id;
  result.second = cat_name;
}

//recursive creation
void DecisionTree::walk(const DataTable & dt, DecisionTree::TreeNode * root, std::list<int> col_id, std::list<int> row_id) {
  if (root) {
    //static int NUMB = 0;
    //std::cout << NUMB << std::endl;
    std::pair<int, std::string> result;
    result.first = -1;
    calculate_info_gain(dt, col_id, row_id, result);
    if (result.first < 0 || result.second.empty()) {
      root->n_set(-1, result.second);
    }
    else if (is_number(result.second)) {
      root->n_set(result.first, result.second);
      col_id.remove(result.first);
      std::list<int> pos, neg;
      for (int i: row_id) {
        if (std::stoi(result.second) >std::stoi(dt[i][result.first])) {
          pos.push_back(i);
        }
        else {
          neg.push_back(i);
        }
      }
      TreeNode * child = root->add_edge(NO);
      walk(dt, child, col_id, neg);
      child = root->add_edge(YES);
      walk(dt, child, col_id, pos);
    }
    else {
      root->n_set(result.first, result.second);
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
        walk(dt, child, col_id, p.second);
      }
    }
  }
}


// linked lista z dostepnymi indeksami kategorii
void DecisionTree::build(const DataTable & dt) {
  std::list<int> row_indiecies;
  for (unsigned i = 1; i < dt.get_len(); ++i)
    row_indiecies.push_back(i);
  std::list<int> col_indieces;
  for (unsigned i = 0; i < dt[0].size() - 1; ++i) {
    col_indieces.push_back(i);
  }
  try {
    walk(dt, root, col_indieces, row_indiecies);
  }
  catch(std::exception e) {
    std::cerr << "build(): " << e.what() << std::endl;
  }
}

void DecisionTree::print_walk(const DataTable & dt, TreeNode * node, int indenation) {
  if (node) {
    for (int i = 0; i < indenation; ++i)
      std::cout << "\t";
    if (node->get_cat() < 0) {
      std::cout<< "Leaf-> " << node->get_value() << ".\n";
    }
    else {
      std::cout << "Node-> ";
      is_number(node->get_value()) ? std::cout<< dt[0][node->get_cat()]  << " " << node->get_value()<<" > x\n" : std::cout << node->get_value() << '\n';
      for (auto ch: node->get_children()) {
         for (int i = 0; i < indenation; ++i)
            std::cout << '\t';
          std::cout << "  edge: "<< ch.first << '\n';
          print_walk(dt, ch.second, indenation + 1);
      }
    }
  }
}

void DecisionTree::print(const DataTable & dt) const {
  std::cout << "\n#### Builded tree ####\n";
  print_walk(dt, root, 0);
}

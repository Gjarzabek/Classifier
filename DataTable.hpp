#ifndef DATATABLE_H
#define DATATABLE_H

#include <fstream>
#include <vector>
#include <iostream>

/**
  2D table which stores strings
*/
class DataTable {
  public:
    DataTable(): data(new std::vector<std::vector<std::string>>(0)) {}
    ~DataTable() {
      if (data)
        delete data;
    }
    void txt_load(const std::string & filename);
    void txt_save(const std::string & table) const;
    void print() const;
    unsigned get_len() const {
      return data->size();
    }
    friend std::ostream& operator<<(std::ostream& os, const DataTable & dt);
    const std::vector<std::string> & operator[] (int x) const;
    

  private:
    std::vector<std::vector<std::string>> * data;

};
/**
std::ostream& operator<<(std::ostream& os, const DataTable & dt) {
  unsigned len = (dt.data)->end()->size();
  for (unsigned i = 0 ; i < len; ++i) {
    os << "---";
  }
  os << '\n';

  for (auto line: *(dt.data)) {
    for (auto i = line.begin(); i != line.end(); ++i)
      os << *i << "  ";
    os << "\n";
  }

  for (unsigned i = 0 ; i < len; ++i) {
    os << "---";
  }
  os << '\n';
  return os;
}*/

#endif

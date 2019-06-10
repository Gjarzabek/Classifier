#ifndef DATATABLE_H
#define DATATABLE_H

#include <fstream>
#include <vector>
#include <iostream>
#include "classifier_exception.hpp"

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
    void add_record(int i, std::string str) {
      (*data)[i].push_back(str);
    }
    

  private:
    std::vector<std::vector<std::string>> * data;

};

#endif

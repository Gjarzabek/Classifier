#include "DataTable.hpp"
#include <assert.h>

const std::vector<std::string> & DataTable::operator[] (int index) const{
  assert(data->size() > index);
  return (*data)[index];
}


void DataTable::print() const {
  unsigned len = (data)->size();
  for (unsigned i = 0 ; i < len; ++i) {
    std::cout<< "---";
  }
  std::cout<< '\n';

  for (auto line: *(data)) {
    for (auto i = line.begin(); i != line.end(); ++i)
      std::cout<< *i << "  ";
    std::cout<< "\n";
  }

  for (unsigned i = 0 ; i < len; ++i) {
    std::cout<< "---";
  }
  std::cout<< '\n';
}


void DataTable::txt_save(const std::string & filename) const {
  std::fstream file;
  file.open(filename, std::ios::out);
  if (!file.is_open())
    throw Classfier_except("error: cannot open " + filename);
  for (auto word: (*data)[0]) {
    file << word << ' ';
  }
  file << '\n';
  for (auto it = data->begin() + 1; it != data->end(); it++) {
    for (auto word: *it) {
      file << word << '\t';
    }
    file << '\n';
  }
  file.close();
}

void DataTable::txt_load(const std::string & filename) {
  std::fstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open())
    throw Classfier_except("error: cannot open " + filename);
  data->clear();
  bool in_word = false;
  std::vector<std::string> line;
  std::string word;
  char ch = file.get();
  while (!(file.eof())) {
    switch (ch) {
      case '\t':
      case ' ':
        if (in_word) {
          line.push_back(word);
        }
        word.clear();
        in_word = false;
        break;
      case '\n':
        if (in_word) {
          in_word = false;
          line.push_back(word);
          word.clear();
        }
        if (!(line.empty()))
          data->push_back(line);
        line.clear();
        break;
      default:
        in_word = true;
        word += ch;
    }
    ch = file.get();
  }
  file.close();
}

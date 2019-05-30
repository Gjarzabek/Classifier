#include "DataTable.hpp"

void DataTable::txt_save(const std::string & filename) const {
  std::fstream file;
  file.open(filename, std::ios::out);
  if (!file.is_open())
    throw "txt_save(): " + filename + ": open error";
  for (auto line: *data) {
    for (auto word: line) {
      file << word << " ";
    }
    file << '\n';
  }
}

void DataTable::txt_load(const std::string & filename) {
  std::fstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open())
    throw "load_file(): " + filename + ": open error";
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
        data->push_back(line);
        line.clear();
        break;
      default:
        in_word = true;
        word += ch;
    }
    ch = file.get();
  }
}

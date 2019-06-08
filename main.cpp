#include <iostream>
#include <vector>
#include "DecisionTree.hpp"
#include "Classifier.hpp"
#include "DataTable.hpp"

using namespace std;

// Klasyfikator-1R uczymy na przekazanej tabeli a potem wykorzystujemy do
// poszczegolnych obiektow,
// mozliwosc klasyfikowania pojedynczo lub całych tabel

// Uczenie polega na przekazaniu Klasyfikatorowi tabeli danych wraz
// z oczekiwanymi wynikami dla poszczegolnych obiektow w tabeli
// kontener <obiekt, ocena>

//Klasyfikator na podstawie przekazanych danych sporzadza odpowiednie drzewo decyzycjne
//Ktore nastepnie wykorzystuje do klasyfikacji danych.

int main() {
  DataTable a;
  try {
    a.txt_load("data.txt");
  }
  catch (std::string str) {
    std::cerr << str << std::endl;
    return 0;
  }
  DecisionTree b;
  DataTable a2;
  DecisionTree b2;

  b.build(a);
  b.print(a);
    try {
    a2.txt_load("data2.txt");
  }
  catch (std::string str) {
    std::cerr << str << std::endl;
  }
  b2.build(a2);
  b2.print(a2);
  return 0;
}

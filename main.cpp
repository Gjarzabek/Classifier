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
  cout << "Hello!\n";
  DataTable a;
  a.txt_load("data.txt");
  a.print();
  DecisionTree b;
  b.build(a);
  b.print();
  
  return 0;
}

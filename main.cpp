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

//Klasyfikator na podstawie przekazanych danych sporzadza odpowiednie drzewo decyzycjne
//Ktore nastepnie wykorzystuje do klasyfikacji danych.

/*
  Todo:
    *napisać Klase Classifier która uzywa DataTable i DecisionTree
    *dodac atrybut Datatable do drzewa
    *dodać swoja klasę exepction do trhowowania
    *dodać wspolbieżne wyliczanie drzewa
*/

int main() {
  
  Classifier c("data.txt");
  c.learn();
  std::cout << c.get_answer({"rainy" , "71" , "91" , "TRUE"}) << std::endl;
  std::cout << "Po odpowiedzi";
  return 0;
}

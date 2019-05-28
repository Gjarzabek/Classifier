#ifndef CLASSIFIER_1R_H
#define CLASSIFIER_1R_H


// Klasyfikator-1R uczymy na przekazanej tabeli a potem wykorzystujemy do
// poszczegolnych obiektow,
// mozliwosc klasyfikowania pojedynczo lub całych tabel

// Uczenie polega na przekazaniu Klasyfikatorowi tabeli danych wraz
// z oczekiwanymi wynikami dla poszczegolnych obiektow w tabeli
// kontener <obiekt, ocena>

//Klasyfikator na podstawie przekazanych danych sporzadz odpowiednie drzewo decyzycjne
//Ktore nastepnie wykorzystuje do klasyfikacji danych.

template <class T, class N>
class Classifier1R {
  public:
    Classifier1R();
  private:
    int fittest_col;

};

#endif CLASSIFIER_1R_H

# Classifier
induction of the decision tree based on data from the table in the txt file

  Program zaliczeniowy nr3 - Indukcja drzewa decyzyjnego z tabeli danych, klasyfikacja za jego pomocą
  wersja: 1.1
  Data: 10.06.2019
  
  autor: Grzegorz Jarząbek
  
  przedmiot: Programowanie Obiektowe
  
  program kompilujemy za pomoca przygotwanego Makefile'a
  
  *** Dzialanie ***
  
  Program uruchamiamy z wiersza polecen,
  
  ./classifier <tabela_danych_treningowych.txt> [lista plikow  .txt do sklasyfikowania] [--tree]
  
  Wyniki klasyfikacji zapisywane są do plików txt o nazwie @nazwa_pliku_z_danymi_out.txt,
  
  opcjonalna flaga [--tree] powoduje wydruk na wyjscie(stdout) skontruowanego drzewa.
  
  ## Konstrukcja drzewa
  Przy wyborze kolejnej "najlepszej" kategorii do aktualnego wezła
  program korzysta z heurystyki polegającej na obliczaniu entriopii i szacowaniu potencjalnego przyrostu informacjii
  dla danej kategorii.
  
  ## Struktura drzwa
  Każdy węzeł posiada:
   *node_value - nazwa jednej z kategorii lub warunek logiczny (np 70 > x)
   *id_kategorii -- numer kolumny ktorej wartosc reprezentuje, dla liścia -1
   *children - hasz tablica gdzie kluczami są możliwe wyniki testu przeprowadzonego na @node_value - "krawedzie grafu"
               wartosci tablicy to kolejne wierzchołki.

  ## Zadania poszczególnych klas
  ClassfiyShell - Obsługa stdin oraz błędów, przetwarza komendy od użytkownaika i odpowienio na nie reaguje
  DataTable -     Odczyt i zapis tabeli danych z/do plików tekstowych, przechowywanie tabeli w pamięci RAM
  DecisionTree -  Tworzenie i indukcja drzewa decyzyjnego z danej DataTable
  Classifier -    Zarządzanie drzewem, przekazywanie informacji z ClassfiyShell
  
  ## Oczekiwana postać dancyh w pilku .txt  
  NazwaKolumny1%NazwaKolumny2% ....... %NazwaKolumnyN
  var1%var2%....%varN
  ...
  ..
  Wartość kolumnyN powinna być zero jedynkowa("yes"/"no", "true"/"false", 0/1 ...).
  
  W ów porządku wypisywane są również pliki z rezultatami testów.

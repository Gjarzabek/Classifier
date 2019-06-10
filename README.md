# DecisionTree-1R

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
  ## Zadania poszczególnych klas
  ClassfiyShell - Obsługa stdin oraz błędów, przetwarza komendy od użytkownaika i odpowienio na nie reaguje
  DataTable -     Odczyt i zapis tabeli danych z/do plików tekstowych, przechowywanie tabeli w pamięci RAM
  DecisionTree -  Tworzenie i indukcja drzewa decyzyjnego z danej DataTable
  Classifier -    Zarządzanie drzewem, przekazywanie informacji z ClassfiyShell

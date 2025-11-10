//
// Created by ervin on 10/25/25.
//
#ifndef SQL_ENGINE_APPLICATION_H
#define SQL_ENGINE_APPLICATION_H
#include "Catalog.h"
#include "Table.h"
#include <cctype>
#include <iostream>

std::string *parse_column(const std::string &column) {
  int noOfFields = 1;
  for (int i = 0; i < column.length(); i++) {
    if (column[i] == ',') {
      noOfFields++;
    }
  }
  if (noOfFields != 4) {
    std::cout << "Wrong statement! name/type/size/def_val";
    return nullptr;
  }

  int k = 0;
  std::string *fields = new std::string[noOfFields];

  for (int i = 0; i < column.length(); i++) {
    if (column[i] == ',') {
      k++;
    } else {
      fields[k] += column[i];
    }
  }

  return fields;
}

auto *catalog = new Catalog;

class Application {
private:
  std::string *words;
  int noOfWords;
  std::string s;

public:
  Application() {
    this->words = nullptr;
    this->noOfWords = 0;
  }

  ~Application() { delete[] this->words; }

  void setQuery(std::string *words, int noOfWords, std::string s) {
    this->s = s;
    this->words = words;
    this->noOfWords = noOfWords;
  }

  void parse_command() {
    std::string firstWord = words[0];
    if (firstWord == "create") {
      std::string secondWord = words[1];
      if (secondWord == "table") {
        create_table();
      } else if (secondWord == "index") {
        create_index();
      } else {
        std::cout << "Wrong statement! You can create a table or an index.";
      }
    } else if (firstWord == "drop") {
      std::string secondWord = words[1];
      if (secondWord == "table") {
        drop_table();
      } else if (secondWord == "index") {
        drop_index();
      } else {
        std::cout << "Wrong statement! You can drop a table or an index.";
      }
    } else if (firstWord == "display") {
      display_table();
    } else if (firstWord == "insert") {
      create_table();
    } else if (firstWord == "select") {
      create_table();
    } else if (firstWord == "delete") {
      delete_from();
    } else if (firstWord == "update") {
    } else {
      std::cout << "Command is wrong";
    }
  }

  void create_table() {
    if (noOfWords >= 3 && words[3] == "if") {
      if (words[4] != "not" || words[5] != "exists") {
        std::cout << "Syntax error!";
        return;
      }
      std::string tableName = words[2];
      if (catalog->already_exists(tableName)) {
        std::cout << "Table already exists.";
        return;
      }

      if (noOfWords != 7) { // daca n are field introdus sau daca mai are ceva
                            // introdus dupa field/field-uri
        std::cout << "Too many spaces";
        return;
      }
      if (words[6][0] != '(' or words[6][1] != '(' or
          words[6][words[6].length() - 1] != ')' or
          words[6][words[6].length() - 2] != ')') {
        std::cout << "Invalid format primele 2";
        return;
      }

      for (int i = 0; i < words[6].length(); i++) {
        if (std::isalnum(words[6][i]) || words[6][i] == ',' ||
            words[6][i] == '(' || words[6][i] == ')') {
          // okey
        } else {
          std::cout
              << "Invalid format!!!!!!"; // se accepta doar ((x,z3,f,d)), fara
                                         // spatii in plus sau alte caractere
          return;
        }
      }

      int noOfColumns = 1;

      for (int i = 2; i < words[6].length() - 2; i++) {

        if (words[6][i] == ')' and
            (words[6][i + 1] != ',' or words[6][i + 2] != '(')) {
          std::cout << "Invalid format separator"; // "separatorul" de coloane
                                                   // este "),("
          return;
        }

        if (words[6][i] == ')' and words[6][i + 1] == ',' and
            words[6][i + 2] == '(') {
          i += 2; // sarim peste separator
          noOfColumns++;
        }
        //   if (words[6][i] == '(') {
        //   std::cout << "Invalid format ( in plus" << i;// '('
        //   return;
        // }
      }

      auto *columns = new std::string[noOfColumns + 1];
      int k = 0;

      for (int i = 2; i < words[6].length() - 1; i++) {
        if (words[6][i] == ')') {
          k++;
          i += 2;
        } else {
          columns[k] += words[6][i];
        }
      }

      for (int j = 0; j < noOfColumns; j++) {
        std::string *fields = parse_column(columns[j]);

        for (int i = 0; i < 4; i++)
          std::cout << fields[i] << ' ';
        std::cout << std::endl;
      }

      // for (int i =0; i<k; i++) {
      //   std::cout << columns[i]<<std::endl;
      // }

      // we create the table
      Table *table = new Table(2, tableName);
      for (int i = 0; i < 2; i++) {
        // table->setColumn(i, words[]);
      }

      catalog->add_table(table);
      return;
    }
    try {
      std::string tableName;
      Table *table = new Table(2, , tableName);
      catalog->add_table(table);
    } catch (std::exception &e) {
      std::cout << e.what();
    }
  }

  void create_index() {
    if (noOfWords >= 6 && words[2] == "if") {
      if (words[3] != "not" || words[4] != "exists" || words[6] != "on") {
        std::cout << "Syntax error!";
        return;
      }
      std::string indexName = words[5];
      std::string tableName = words[7];
      if (catalog->already_exists(
              indexName)) { // trebuie modificat, nu putem folosi catalog
        std::cout << "Table already exists.";
        return;
      }

      if (words[6][0] != '(' || words[6][words[6].length() - 1] != ')') {
        std::cout << "Invalid format paranteze";
      }

      std::string columnName;
      for (int i = 1; i < words[6].length() - 1; i++) {
        columnName += words[6][i];
      }

      // if (coloana exists in tableName) {
      //     *create index in tableName*
      // }
    }
  }

  void drop_table() { std::cout << "Dropped table succesfully."; }

  void drop_index() { std::cout << "Dropped index successfully."; }

  void display_table() {}

  void update_table() { std::cout << "Updated table successfully."; }

  void delete_from() { std::cout << "Deleted successfully."; }
};

#endif // SQL_ENGINE_APPLICATION_H

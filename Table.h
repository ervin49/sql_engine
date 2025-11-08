//
// Created by ervin on 26.10.2025.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <string>

class Table {
private:
    int noOfColumns = 0;
    std::string name;
    std::string *columns = nullptr;

public:
    Table(int noOfColumns, std::string name) {
        this->name = name;
        this->noOfColumns = noOfColumns;
        columns = new std::string[this->noOfColumns];
    }

    Table(const Table &other) {
        name = other.name;
        noOfColumns = other.noOfColumns;
        columns = new std::string[noOfColumns];
        for (int i = 0; i < noOfColumns; i++) {
            columns[i] = other.columns[i];
        }
        //adaugam tabela in catalog
    }

    void setColumn(int index, std::string columnName) {
        if (index < 0 || index >= noOfColumns) {
            throw std::out_of_range("Index out of range");
        }
        columns[index] = columnName;
    }

    std::string getName() {
        return this->name;
    }

    void print_table() {
      for (int i = 0; i < noOfColumns; i++) {
        std::cout<<columns[i]<<"-----";
      }
      std::cout<<std::endl;
      //print rows
    }
};

#endif //UNTITLED_TABLE_H

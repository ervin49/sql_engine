//
// Created by ervin on 26.10.2025.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <string>

class Table {
private:
    int noOfColumns = 0;
    int noOfRows = 0;
    std::string name;
    std::string *columns = nullptr;
    std::string **rows = nullptr;

public:
    Table(int noOfColumns, int noOfRows, std::string name) {
        this->name = name;
        this->noOfColumns = noOfColumns;
        this->noOfRows = noOfRows;
        columns = new std::string[this->noOfColumns];
        rows = new std::string *[this->noOfRows];
        for (int i = 0; i < noOfColumns; i++) {
            rows[i] = new std::string[noOfColumns];
        }
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

    void addRow(std::string *newRow) {
        std::string **newRows = new std::string *[noOfRows + 1];

        //transfer the values from old rows array to the newRows array
        for (int i = 0; i < noOfRows; i++) {
            newRows[i] = rows[i];
        }

        newRows[noOfRows] = newRow;
        rows = newRows;
        for (int i = 0; i < noOfRows; i++) {
            delete newRows[i];
        }
        delete[] newRows;
        noOfRows++;
        newRows = nullptr;
    }

    std::string getName() {
        return this->name;
    }

    void print_table() {
        for (int i = 0; i < noOfColumns; i++) {
            std::cout << columns[i] << "-----";
        }
        std::cout << std::endl;
        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfRows; j++) {
                std::cout << rows[i][j];
            }
            std::cout << std::endl;
        }
    }
};

#endif //UNTITLED_TABLE_H

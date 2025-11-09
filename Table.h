//
// Created by ervin on 26.10.2025.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <string>
#include <iostream>

class Table {
private:
    int noOfColumns = 0;
    int noOfRows = 0;
    std::string name;
    std::string *columns = nullptr;
    std::string **rows = nullptr;

public:
    Table(int noOfColumns, std::string name) {
        this->name = name;
        this->noOfColumns = noOfColumns;
        this->noOfRows = 0;
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

    void setNoOfColumns(int noOfColumns) {
        if (noOfColumns <= 0) {
            throw "You need a valid number of columns!";
        }
        this->noOfColumns = noOfColumns;
    }

    void setNoOfRows(int noOfRows) {
        if (noOfRows <= 0) {
            throw "You need a valid number of rows!";
        }
        this->noOfRows = noOfRows;
        rows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            rows[i] = new std::string[noOfColumns];
        }
    }

    int getNoOfColumns() {
        return noOfColumns;
    }

    int getNoOfRows() {
        return noOfRows;
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

        //add the new row
        newRows[noOfRows] = newRow;
        rows = newRows;
        delete[] newRows;
        this->noOfRows++;
        newRows = nullptr;
    }

    std::string getName() {
        return this->name;
    }

    void print_table() {
        int *maxLengthOnColumn = new int[noOfColumns];
        //we initialize all the max lenghts with 0
        for (int i = 0; i < noOfColumns; i++) {
            maxLengthOnColumn[i] = 0;
        }

        //we find the max length of each column
        for (int j = 0; j < noOfColumns; j++) {
            for (int i = 0; i < noOfRows; i++) {
                if (rows[i][j].length() > maxLengthOnColumn[j]) {
                    maxLengthOnColumn[j] = rows[i][j].length();
                }
            }
        } //seg fault

        //we display the column names
        for (int i = 0; i < noOfColumns - 1; i++) {
            std::cout << columns[i];
            for (int j = 0; j < maxLengthOnColumn[i]; j++) {
                std::cout << '-';
            }
        }
        std::cout << columns[noOfColumns - 1] << std::endl; //seg fault

        //we display the rows
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < noOfRows; j++) {
                std::cout << rows[i][j] << ' ';
            }
            std::cout << std::endl;
        } //seg fault

        delete[] maxLengthOnColumn;
    }
};

#endif //UNTITLED_TABLE_H

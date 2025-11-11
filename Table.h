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
    int noOfRows;
    std::string name;
    std::string *columns = nullptr;
    std::string **rows;

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

    Table();

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
        std::string **newRows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            newRows[i] = new std::string[noOfColumns];
            for (int j = 0; j < noOfRows; j++) {
                newRows[i][j] = rows[i][j];
            }
        }
        for (int i = 0; i < noOfRows; i++) {
            delete[] rows[i];
        }
        delete[] rows;
        rows = newRows;
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

    void addRow(std::string newRow[]) {
        std::string **newRows = new std::string *[noOfRows + 1];
        for (int i = 0; i < noOfRows; i++) {
            newRows[i] = new std::string[noOfColumns];
        }

        //transfer the values from old rows array to the newRows array
        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                newRows[i][j] = rows[i][j];
            }
        }

        newRows[noOfRows] = newRow;
        rows = newRows;
        this->noOfRows++;
        newRows = nullptr;
    }

    std::string getName() {
        return this->name;
    }

    void print_table() {
        std::cout << name << std::endl;
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
        }
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i].length() > maxLengthOnColumn[i]) {
                maxLengthOnColumn[i] = columns[i].length();
            }
        }

        //offset for displaying the values
        int offset = 10;

        //we display the column names
        for (int i = 0; i < noOfColumns - 1; i++) {
            std::cout << columns[i];
            if (columns[i].length() == maxLengthOnColumn[i]) {
                for (int k = 0; k < offset; k++) {
                    std::cout << '-';
                }
            } else {
                for (int k = 0; k < maxLengthOnColumn[i] - columns[i].length() + offset; k++) {
                    std::cout << '-';
                }
            }
        }
        std::cout << columns[noOfColumns - 1] << std::endl;

        //we display the rows
        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                std::cout << rows[i][j];
                if (rows[i][j].length() == maxLengthOnColumn[j]) {
                    for (int k = 0; k < offset && j < noOfColumns - 1; k++) {
                        std::cout << ' ';
                    }
                } else {
                    for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length() + offset && j < noOfColumns - 1; k
                         ++) {
                        std::cout << ' ';
                    }
                }
            }
            std::cout << std::endl;
        }

        delete[] maxLengthOnColumn;
    }
};

#endif //UNTITLED_TABLE_H

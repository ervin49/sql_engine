//
// Created by ervin on 26.10.2025.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <string>
#include <iostream>

#include "globals.h"
#include "Index_Catalog.h"
#define OFFSET 10

class Table {
private:
    int noOfColumns = 0;
    int noOfRows;
    std::string tableName;
    std::string *columns = nullptr;
    std::string **rows;
    std::string indexName;
    std::string columnOfTheIndex;

public:
    Table(int noOfColumns, std::string tableName) {
        this->tableName = tableName;
        this->noOfColumns = noOfColumns;
        this->noOfRows = 0;
        columns = new std::string[this->noOfColumns];
        rows = nullptr;
    }

    Table(const Table &other) {
        tableName = other.tableName;
        noOfColumns = other.noOfColumns;
        noOfRows = other.noOfRows;
        columns = new std::string[noOfColumns];
        for (int i = 0; i < noOfColumns; i++) {
            columns[i] = other.columns[i];
        }

        rows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            rows[i] = new std::string[noOfColumns];
        }

        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                rows[i][j] = other.rows[i][j];
            }
        }
    }

    Table() = default;

    void setNoOfColumns(int noOfColumns) {
        if (noOfColumns <= 0) {
            throw "You need a valid number of columns!";
        }
        this->noOfColumns = noOfColumns;
    }

    void setNoOfRows(int noOfRows) {
        if (noOfRows < 0) {
            throw "You need a valid number of rows!";
        }

        this->noOfRows = noOfRows;
        std::string **newRows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            newRows[i] = new std::string[noOfColumns];
            for (int j = 0; j < noOfColumns; j++) {
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

    bool column_exists(std::string columnName) {
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i] == columnName) {
                return true;
            }
        }

        return false;
    }

    int setIndex(std::string indexName, std::string columnOfTheIndex) {
        if (indexName != "" && indexCatalog->has_index(columnOfTheIndex) == true) {
            this->indexName = indexName;
            this->columnOfTheIndex = columnOfTheIndex;
            return 0;
        }
        return -1;
    }


    std::string getColumnOfTheIndex() {
        return columnOfTheIndex;
    }

    void add_row(std::string newRow[]) {
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
    }

    std::string getName() {
        return this->tableName;
    }

    void print_table() {
        std::cout << '[' << tableName << ']' << std::endl;
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

        //we display the column names
        for (int i = 0; i < noOfColumns - 1; i++) {
            std::cout << columns[i];

            if (columns[i].length() == maxLengthOnColumn[i]) {
                for (int k = 0; k < OFFSET; k++) {
                    std::cout << '-';
                }
            } else {
                for (int k = 0; k < maxLengthOnColumn[i] - columns[i].length() + OFFSET; k++) {
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
                    for (int k = 0; k < OFFSET && j < noOfColumns - 1; k++) {
                        std::cout << ' ';
                    }
                } else {
                    for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length() + OFFSET && j < noOfColumns - 1; k
                         ++) {
                        std::cout << ' ';
                    }
                }
            }
            std::cout << std::endl;
        }

        delete[] maxLengthOnColumn;
    }

    void setName(std::string name) {
        if (name.empty() || name.length() < 3) {
            std::cout << "Invalid syntax!";
            return;
        }

        this->tableName = name;
    }

    int return_index_of_column_by_name(std::string columnName) {
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i] == columnName) {
                return i;
            }
        }

        return -1;
    }

    void remove_row(int index) {
        if (index == 0 && noOfRows == 1) {
            setNoOfRows(0);
            return;
        }

        for (int i = index; i < noOfRows - 1; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                rows[i][j] = rows[i + 1][j];
            }
        }

        setNoOfRows(noOfRows - 1);
    }

    int delete_from(std::string columnName, std::string nameOfValue) {
        int indexOfColumn;
        if ((indexOfColumn = return_index_of_column_by_name(columnName)) == -1) {
            std::cout << "Table " << tableName << " does not have the column " << columnName << "!" << std::endl;
            return -1;
        }

        for (int i = 0; i < noOfRows; i++) {
            std::cout << rows[i][indexOfColumn] << std::endl;
            if (rows[i][indexOfColumn] == nameOfValue) {
                remove_row(i);
                i--;
            }
        }

        return 0;
    }
};

#endif //UNTITLED_TABLE_H

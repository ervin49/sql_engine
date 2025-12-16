#pragma once
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
    std::string *indexNames;

public:
    Table(int noOfColumns, const std::string &tableName) {
        this->tableName = tableName;
        this->noOfColumns = noOfColumns;
        this->noOfRows = 0;
        columns = new std::string[this->noOfColumns];
        rows = nullptr;
        indexNames = nullptr;
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

        this->indexNames = other.indexNames;
    }

    Table() {
        this->rows = nullptr;
        this->noOfRows = 0;
        this->indexNames = nullptr;
        this->columns = nullptr;
        this->noOfColumns = 0;
        this->tableName = "";
    }

    bool operator==(Table const &table) const {
        if (this->noOfColumns != table.noOfColumns ||
            this->noOfRows != table.noOfRows ||
            this->tableName != table.tableName)
            return false;

        for (int i = 0; i < noOfColumns; i++) {
            if (this->columns[i] != table.columns[i])
                return false;
        }

        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                if (this->rows[i][j] != table.rows[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    ~Table() {
        if (columns != nullptr) {
            delete[] columns;
        }
        if (rows != nullptr) {
            for (int i = 0; i < noOfRows; i++) {
                delete[] rows[i];
            }
            delete[] rows;
        }
    }

    Table& operator=(const Table& other) {
        if (this != &other) {
            // Pas A: Curatam memoria veche a obiectului curent
            if (columns != nullptr) delete[] columns;
            if (rows != nullptr) {
                for (int i = 0; i < noOfRows; i++) delete[] rows[i];
                delete[] rows;
            }

            this->tableName = other.tableName;
            this->noOfColumns = other.noOfColumns;
            this->noOfRows = other.noOfRows;

            this->columns = new std::string[noOfColumns];
            for (int i = 0; i < noOfColumns; i++) {
                this->columns[i] = other.columns[i];
            }

            this->rows = new std::string*[noOfRows];
            for (int i = 0; i < noOfRows; i++) {
                this->rows[i] = new std::string[noOfColumns];
                for (int j = 0; j < noOfColumns; j++) {
                    this->rows[i][j] = other.rows[i][j];
                }
            }
            this->indexNames = other.indexNames;
        }
        return *this;
    }

    operator int() const {
        return this->noOfRows;
    }

    Table &operator+=(std::string *row) {
        this->add_row(row);
        return *this;
    }

    //table -= column
    Table &operator-=(const std::string &columnName) {
        this->remove_column(columnName);
        return *this;
    }

    std::string* operator[](int index) const {
        if (index >= 0 && index < noOfRows) {
            return rows[index];
        }
        return nullptr;
    }

    bool operator!() const {
        return noOfRows == 0;
    }

    friend std::ostream &operator<<(std::ostream &out, const Table &table) {
        table.print_table();
        return out;
    }

    bool operator!=(const Table &table) const {
        if (*this == table) {
            return false;
        }
        return true;
    }

    std::string *getColumns() {
        auto *newColumns = new std::string[noOfColumns];
        for (int i = 0; i < noOfColumns; i++) {
            newColumns[i] = columns[i];
        }
        return newColumns;
    }

    void setRows(std::string **newRows) {
        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                rows[i][j] = newRows[i][j];
            }
        }
    }

    std::string **getRows() {
        std::string **newRows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            newRows[i] = new std::string[noOfColumns];
        }
        for (int i = 0; i < noOfRows; i++) {
            for (int j = 0; j < noOfColumns; j++) {
                newRows[i][j] = rows[i][j];
            }
        }
        return newRows;
    }

    void setNoOfRows(int newNoOfRows) {
        if (newNoOfRows < 0) {
            statusManager->print(StatusManager::Error, "Number of rows has to be at least 0!");
            return;
        }

        int oldNoOfRows = noOfRows;

        auto **newRows = new std::string *[newNoOfRows];
        for (int i = 0; i < newNoOfRows; i++) {
            newRows[i] = new std::string[noOfColumns];

            if (i < oldNoOfRows) {
                for (int j = 0; j < noOfColumns; j++) {
                    newRows[i][j] = rows[i][j];
                }
            }
        }

        for (int i = 0; i < oldNoOfRows; i++) {
            delete[] rows[i];
        }
        delete[] rows;
        rows = newRows;
        noOfRows = newNoOfRows;
    }

    int getNoOfColumns() const {
        return noOfColumns;
    }

    int getNoOfRows() const {
        return noOfRows;
    }

    void setColumn(const int index, const std::string &columnName) const {
        if (index < 0 || index >= noOfColumns) {
            statusManager->print(StatusManager::Error, "Index out of range!");
        }
        columns[index] = columnName;
    }

    bool column_exists(const std::string &columnName) const {
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i] == columnName) {
                return true;
            }
        }

        return false;
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
        delete[] rows;
        rows = newRows;
        this->noOfRows++;
    }

    std::string getName() {
        return this->tableName;
    }

    void print_table() const {
        std::cout << std::endl;

        if (!tableName.empty()) {
            std::cout << '[' << tableName << ']' << std::endl;
        }
        int *maxLengthOnColumn = new int[noOfColumns];

        //we initialize all the max lengths with 0
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

        //find the sum of all lengths
        int sum = 0;
        for (int i = 0; i < noOfColumns; i++) {
            sum += maxLengthOnColumn[i];
            sum += OFFSET;
        }
        sum -= OFFSET;
        sum += 2 * (OFFSET / 2 - 1);

        //we display the first line
        std::cout << '|';
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                std::cout << '-';
            }
            if (i < noOfColumns - 1) {
                std::cout << '|';
            }
        }
        std::cout << '|' << std::endl << '|';

        for (int i = 0; i < OFFSET / 2 - 1; i++) {
            std::cout << ' ';
        }

        //we display the column names
        for (int i = 0; i < noOfColumns; i++) {
            std::cout << columns[i];

            if (columns[i].length() == maxLengthOnColumn[i]) {
                for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++) {
                    std::cout << ' ';
                }
                if (i < noOfColumns - 1) {
                    std::cout << '|';
                }
                for (int k = OFFSET / 2 + 1; k < OFFSET; k++) {
                    std::cout << ' ';
                }
                if (i == noOfColumns - 1) {
                    std::cout << ' ';
                }
            } else {
                for (int k = 0; k < maxLengthOnColumn[i] - columns[i].length(); k++) {
                    std::cout << ' ';
                }
                for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++) {
                    std::cout << ' ';
                }
                if (i < noOfColumns - 1) {
                    std::cout << '|';
                }
                if (i == noOfColumns - 1) {
                    for (int k = OFFSET / 2; k < OFFSET; k++) {
                        std::cout << ' ';
                    }
                } else {
                    for (int k = OFFSET / 2 + 1; k < OFFSET; k++) {
                        std::cout << ' ';
                    }
                }
            }
        }

        std::cout << '|' << std::endl << '|';

        //we display the line below the column names
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                std::cout << '-';
            }
            if (i < noOfColumns - 1) {
                std::cout << '|';
            }
        }

        std::cout << '|' << std::endl;

        //we display the rows
        for (int i = 0; i < noOfRows; i++) {
            std::cout << '|';
            for (int k = 0; k < OFFSET / 2 - 1; k++) {
                std::cout << ' ';
            }
            for (int j = 0; j < noOfColumns; j++) {
                std::cout << rows[i][j];

                if (rows[i][j].length() == maxLengthOnColumn[j]) {
                    for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++) {
                        std::cout << ' ';
                    }
                    if (j < noOfColumns - 1) {
                        std::cout << '|';
                    }
                    if (j == noOfColumns - 1) {
                        for (int k = 0; k < OFFSET / 2; k++) {
                            std::cout << ' ';
                        }
                    } else {
                        for (int k = 0; k < OFFSET / 2 - 1; k++) {
                            std::cout << ' ';
                        }
                    }
                } else {
                    for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length(); k++) {
                        std::cout << ' ';
                    }
                    for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++) {
                        std::cout << ' ';
                    }
                    if (j < noOfColumns - 1) {
                        std::cout << '|';
                    }
                    if (j == noOfColumns - 1) {
                        for (int k = 0; k < OFFSET / 2; k++) {
                            std::cout << ' ';
                        }
                    } else {
                        for (int k = 0; k < OFFSET / 2 - 1; k++) {
                            std::cout << ' ';
                        }
                    }
                }
            }
            std::cout << '|' << std::endl;
        }

        //we display the last line
        std::cout << '|';
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                std::cout << '-';
            }
            if (i < noOfColumns - 1) {
                std::cout << '|';
            }
        }
        std::cout << '|' << std::endl << std::endl;

        delete[] maxLengthOnColumn;
    }

    void setName(const std::string &name) {
        if (name.empty() || name.length() < 3) {
            statusManager->print(StatusManager::Error, "Name needs to be at least 3 characters long!");
            return;
        }

        this->tableName = name;
    }

    int return_index_of_column_by_name(const std::string &columnName) const {
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i] == columnName) {
                return i;
            }
        }

        return -1;
    }

    void remove_column(const std::string &columnName) {
        if (noOfColumns <= 1) {
            statusManager->print(StatusManager::Error, "You need at least 1 column!");
        }
        auto *newColumns = new std::string[noOfColumns - 1];

        int k = 0;
        for (int i = 0; i < noOfColumns; i++) {
            if (columns[i] == columnName) {
                continue;
            }
            newColumns[k++] = columns[i];
        }
        for (int i = 0; i < noOfColumns; i++) {
            columns[i] = newColumns[i];
        }
        this->setNoOfColumns(noOfColumns - 1);
        delete[] newColumns;
    }

    void setNoOfColumns(int newNoOfColumns) {
        if (newNoOfColumns <= 1) {
            statusManager->print(StatusManager::Error, "You need at least 1 column!");
        }
        this->noOfColumns = newNoOfColumns;
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

    void index_table(const std::string &indexName) const {
        Index *index = indexCatalog->getIndex(indexName);
        for (int i = 0; i < noOfRows; i++) {
            for (int j = i + 1; j < noOfColumns; j++) {
            }
        }
        //to be written
        delete index;
    }

    void swap_rows(int firstRowIndex, int secondRowIndex) {
    }

    int delete_from(const std::string &columnName, const std::string &nameOfValue) {
        int indexOfColumn;
        if ((indexOfColumn = return_index_of_column_by_name(columnName)) == -1) {
            statusManager->print(StatusManager::Error,
                                 "Table \"" + tableName + "\" does not have the column \"" + columnName + "\"!");
            return -1;
        }

        for (int i = 0; i < noOfRows; i++) {
            if (rows[i][indexOfColumn] == nameOfValue) {
                remove_row(i);
                i--;
            }
        }
        return 0;
    }

    std::string getTableName() {
        return tableName;
    }
};

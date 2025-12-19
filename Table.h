#pragma once
#include <string>
#include <iostream>

#include "globals.h"
#include "Index_Catalog.h"
#define OFFSET 10

class Table {
private:
    int noOfColumns;
    int noOfRows;
    int noOfIndexes;
    std::string tableName;
    std::string *columns;
    std::string **rows;
    std::string *indexNames;

public:
    Table(int noOfColumns, const std::string &tableName) {
        this->tableName = tableName;
        this->noOfColumns = noOfColumns;
        noOfRows = 0;
        noOfIndexes = 0;
        columns = new std::string[this->noOfColumns];
        rows = nullptr;
        indexNames = nullptr;
    }


    Table(const Table &other) {
        tableName = other.tableName;
        noOfColumns = other.noOfColumns;
        noOfRows = other.noOfRows;
        columns = new std::string[noOfColumns];
        noOfIndexes = other.noOfIndexes;
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
        this->noOfIndexes = 0;
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

    Table &operator=(const Table &other) {
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

            this->rows = new std::string *[noOfRows];
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

    std::string *operator[](int index) const {
        if (index >= 0 && index < noOfRows) {
            return rows[index];
        }
        return nullptr;
    }

    bool operator!() const {
        return noOfRows == 0;
    }

    friend std::ostream &operator<<(std::ostream &out, const Table &table) {
        table.print_table(out);
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

    void setRows(std::string **newRows, const int noOfRows, const int noOfColumns) {
        if (noOfRows < 0 || noOfColumns <= 0) {
            throw std::runtime_error("Invalid no. of rows or columns!");
        }

        for (int i = 0; i < this->noOfRows; i++) {
            delete[] rows[i];
        }
        delete[] rows;

        rows = new std::string *[noOfRows];
        for (int i = 0; i < noOfRows; i++) {
            rows[i] = new std::string[noOfColumns];
            for (int j = 0; j < noOfColumns; j++) {
                rows[i][j] = newRows[i][j];
            }
        }

        this->noOfRows = noOfRows;
        this->noOfColumns = noOfColumns;
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

    std::string getTableName() {
        return this->tableName;
    }

    void print_table(std::ostream &out) const {
        out << std::endl;

        if (!tableName.empty()) {
            out << '[' << tableName << ']' << std::endl;
        }
        int *maxLengthOnColumn = new int[noOfColumns];

        //initialize all the max lengths with 0
        for (int i = 0; i < noOfColumns; i++) {
            maxLengthOnColumn[i] = 0;
        }

        //find the max length of each column
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

        //display the first line
        out << '+';
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                out << '-';
            }
            if (i < noOfColumns - 1) {
                out << '+';
            }
        }
        out << '+' << std::endl << '|';

        for (int i = 0; i < OFFSET / 2 - 1; i++) {
            out << ' ';
        }

        //display the column names
        for (int i = 0; i < noOfColumns; i++) {
            out << columns[i];

            if (columns[i].length() == maxLengthOnColumn[i]) {
                for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++) {
                    out << ' ';
                }
                if (i < noOfColumns - 1) {
                    out << '|';
                }
                for (int k = OFFSET / 2 + 1; k < OFFSET; k++) {
                    out << ' ';
                }
                if (i == noOfColumns - 1) {
                    out << ' ';
                }
            } else {
                for (int k = 0; k < maxLengthOnColumn[i] - columns[i].length(); k++) {
                    out << ' ';
                }
                for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++) {
                    out << ' ';
                }
                if (i < noOfColumns - 1) {
                    out << '|';
                }
                if (i == noOfColumns - 1) {
                    for (int k = OFFSET / 2; k < OFFSET; k++) {
                        out << ' ';
                    }
                } else {
                    for (int k = OFFSET / 2 + 1; k < OFFSET; k++) {
                        out << ' ';
                    }
                }
            }
        }

        out << '|' << std::endl << '+';

        //display the line below the column names
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                out << '-';
            }
            if (i < noOfColumns - 1) {
                out << '+';
            }
        }

        out << '+' << std::endl;

        //display the rows
        for (int i = 0; i < noOfRows; i++) {
            out << '|';
            for (int k = 0; k < OFFSET / 2 - 1; k++) {
                out << ' ';
            }
            for (int j = 0; j < noOfColumns; j++) {
                out << rows[i][j];

                if (rows[i][j].length() == maxLengthOnColumn[j]) {
                    for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++) {
                        out << ' ';
                    }
                    if (j < noOfColumns - 1) {
                        out << '|';
                    }
                    if (j == noOfColumns - 1) {
                        for (int k = 0; k < OFFSET / 2; k++) {
                            out << ' ';
                        }
                    } else {
                        for (int k = 0; k < OFFSET / 2 - 1; k++) {
                            out << ' ';
                        }
                    }
                } else {
                    for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length(); k++) {
                        out << ' ';
                    }
                    for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++) {
                        out << ' ';
                    }
                    if (j < noOfColumns - 1) {
                        out << '|';
                    }
                    if (j == noOfColumns - 1) {
                        for (int k = 0; k < OFFSET / 2; k++) {
                            out << ' ';
                        }
                    } else {
                        for (int k = 0; k < OFFSET / 2 - 1; k++) {
                            out << ' ';
                        }
                    }
                }
            }
            out << '|' << std::endl;
        }

        //display the last line
        out << '+';
        for (int i = 0; i < noOfColumns; i++) {
            for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++) {
                out << '-';
            }
            if (i < noOfColumns - 1) {
                out << '+';
            }
        }
        out << '+' << std::endl << std::endl;

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

    std::string *getIndexNames() const {
        const auto newIndexNames = new std::string[noOfIndexes];
        for (int i = 0; i < noOfIndexes; i++) {
            newIndexNames[i] = indexNames[i];
        }

        return newIndexNames;
    }

    int getNoOfIndexes() const {
        return noOfIndexes;
    }

    void setIndexNames(std::string *indexNames, int noOfIndexes) {
        std::string *newIndexNames = new std::string[noOfIndexes];
        for (int i = 0; i < noOfIndexes; i++) {
            newIndexNames[i] = indexNames[i];
        }

        delete[] this->indexNames;
        this->indexNames = newIndexNames;
        this->noOfIndexes = noOfIndexes;
    }

    void add_index(std::string indexName) {
        auto newIndexNames = new std::string[noOfIndexes + 1];
        for (int i = 0; i < noOfIndexes; i++) {
            newIndexNames[i] = indexNames[i];
        }

        newIndexNames[noOfIndexes] = indexName;
        delete[] indexNames;
        indexNames = newIndexNames;
        noOfIndexes++;
    }
};

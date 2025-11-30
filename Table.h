#pragma once
#include <string>
#include <iostream>

#include "globals.h"
#include "Index_Catalog.h"
#define OFFSET 10

class Table
{
private:
    int noOfColumns = 0;
    int noOfRows;
    std::string tableName;
    std::string* columns = nullptr;
    std::string** rows;
    std::string* indexNames;

public:
    Table(int noOfColumns, const std::string& tableName)
    {
        this->tableName = tableName;
        this->noOfColumns = noOfColumns;
        this->noOfRows = 0;
        columns = new std::string[this->noOfColumns];
        rows = nullptr;
        indexNames = nullptr;
    }


    Table(const Table& other)
    {
        tableName = other.tableName;
        noOfColumns = other.noOfColumns;
        noOfRows = other.noOfRows;
        columns = new std::string[noOfColumns];
        for (int i = 0; i < noOfColumns; i++)
        {
            columns[i] = other.columns[i];
        }

        rows = new std::string*[noOfRows];
        for (int i = 0; i < noOfRows; i++)
        {
            rows[i] = new std::string[noOfColumns];
        }

        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                rows[i][j] = other.rows[i][j];
            }
        }

        this->indexNames = other.indexNames;
    }

    Table() = default;

    bool operator==(Table const& table) const
    {
        if (this->noOfColumns != table.noOfColumns ||
            this->noOfRows != table.noOfRows ||
            this->tableName != table.tableName)
            return false;

        for (int i = 0; i < noOfColumns; i++)
        {
            if (this->columns[i] != table.columns[i])
                return false;
        }

        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                if (this->rows[i][j] != table.rows[i][j])
                {
                    return false;
                }
            }
        }

        return true;
    }

    //table += row;
    Table& operator+=(std::string* row)
    {
        this->add_row(row);
        return *this;
    }

    //table -= column
    Table& operator-=(const std::string& columnName)
    {
        this->remove_column(columnName);
        return *this;
    }


    friend std::ostream& operator<<(std::ostream& out, const Table& table)
    {
        table.print_table();
        return out;
    }

    bool operator!=(const Table& table) const
    {
        if (*this == table)
        {
            return false;
        }
        return true;
    }

    void setNoOfRows(int newNoOfRows)
    {
        if (newNoOfRows < 0)
        {
            statusManager->print(StatusManager::Error, "Number of rows has to be at least 0!");
            return;
        }

        this->noOfRows = newNoOfRows;
        std::string** newRows = new std::string*[newNoOfRows];
        for (int i = 0; i < newNoOfRows; i++)
        {
            newRows[i] = new std::string[noOfColumns];
            for (int j = 0; j < noOfColumns; j++)
            {
                newRows[i][j] = rows[i][j];
            }
        }

        for (int i = 0; i < newNoOfRows; i++)
        {
            delete[] rows[i];
        }
        delete[] rows;
        rows = nullptr;
        rows = newRows;
    }

    int getNoOfColumns() const
    {
        return noOfColumns;
    }

    int getNoOfRows() const
    {
        return noOfRows;
    }

    void setColumn(int index, const std::string& columnName) const
    {
        if (index < 0 || index >= noOfColumns)
        {
            statusManager->print(StatusManager::Error, "Index out of range!");
        }
        columns[index] = columnName;
    }

    bool column_exists(const std::string& columnName) const
    {
        for (int i = 0; i < noOfColumns; i++)
        {
            if (columns[i] == columnName)
            {
                return true;
            }
        }

        return false;
    }

    void add_row(std::string newRow[])
    {
        std::string** newRows = new std::string*[noOfRows + 1];
        for (int i = 0; i < noOfRows; i++)
        {
            newRows[i] = new std::string[noOfColumns];
        }

        //transfer the values from old rows array to the newRows array
        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                newRows[i][j] = rows[i][j];
            }
        }

        newRows[noOfRows] = newRow;
        rows = newRows;
        this->noOfRows++;
    }

    std::string getName()
    {
        return this->tableName;
    }

    void print_table() const
    {
        std::cout << '[' << tableName << ']' << std::endl;
        int* maxLengthOnColumn = new int[noOfColumns];

        //we initialize all the max lengths with 0
        for (int i = 0; i < noOfColumns; i++)
        {
            maxLengthOnColumn[i] = 0;
        }

        //we find the max length of each column
        for (int j = 0; j < noOfColumns; j++)
        {
            for (int i = 0; i < noOfRows; i++)
            {
                if (rows[i][j].length() > maxLengthOnColumn[j])
                {
                    maxLengthOnColumn[j] = rows[i][j].length();
                }
            }
        }

        for (int i = 0; i < noOfColumns; i++)
        {
            if (columns[i].length() > maxLengthOnColumn[i])
            {
                maxLengthOnColumn[i] = columns[i].length();
            }
        }

        //we display the column names
        for (int i = 0; i < noOfColumns - 1; i++)
        {
            std::cout << columns[i];

            if (columns[i].length() == maxLengthOnColumn[i])
            {
                for (int k = 0; k < OFFSET; k++)
                {
                    std::cout << '-';
                }
            }
            else
            {
                for (int k = 0; k < maxLengthOnColumn[i] - columns[i].length() + OFFSET; k++)
                {
                    std::cout << '-';
                }
            }
        }
        std::cout << columns[noOfColumns - 1] << std::endl;

        //we display the rows
        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                std::cout << rows[i][j];

                if (rows[i][j].length() == maxLengthOnColumn[j])
                {
                    for (int k = 0; k < OFFSET && j < noOfColumns - 1; k++)
                    {
                        std::cout << ' ';
                    }
                }
                else
                {
                    for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length() + OFFSET && j < noOfColumns - 1; k
                         ++)
                    {
                        std::cout << ' ';
                    }
                }
            }
            std::cout << std::endl;
        }

        delete[] maxLengthOnColumn;
    }

    void setName(const std::string& name)
    {
        if (name.empty() || name.length() < 3)
        {
            std::cout << "Invalid syntax!";
            return;
        }

        this->tableName = name;
    }

    int return_index_of_column_by_name(const std::string& columnName) const
    {
        for (int i = 0; i < noOfColumns; i++)
        {
            if (columns[i] == columnName)
            {
                return i;
            }
        }

        return -1;
    }

    void remove_column(std::string columnName)
    {
        if (noOfColumns <= 1)
        {
            statusManager->print(StatusManager::Error, "You need at least 1 column!");
        }
        auto* newColumns = new std::string[noOfColumns - 1];

        int k = 0;
        for (int i = 0; i < noOfColumns; i++)
        {
            if (columns[i] == columnName)
            {
                continue;
            }
            newColumns[k++] = columns[i];
        }
        for (int i = 0; i < noOfColumns; i++)
        {
            columns[i] = newColumns[i];
        }
        this->setNoOfColumns(noOfColumns - 1);
        delete[] newColumns;
    }

    void setNoOfColumns(int newNoOfColumns)
    {
        if (newNoOfColumns <= 1)
        {
            statusManager->print(StatusManager::Error, "You need at least 1 column!");
        }
        this->noOfColumns = newNoOfColumns;
    }

    void remove_row(int index)
    {
        if (index == 0 && noOfRows == 1)
        {
            setNoOfRows(0);
            return;
        }

        for (int i = index; i < noOfRows - 1; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                rows[i][j] = rows[i + 1][j];
            }
        }

        setNoOfRows(noOfRows - 1);
    }

    void index_table(const std::string& indexName) const
    {
        Index* index = indexCatalog->getIndex(indexName);
        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = i + 1; j < noOfColumns; j++)
            {
            }
        }
        //to be written
        delete index;
    }

    void swap_rows(int firstRowIndex, int secondRowIndex)
    {
    }

    int delete_from(const std::string& columnName, const std::string& nameOfValue)
    {
        int indexOfColumn;
        if ((indexOfColumn = return_index_of_column_by_name(columnName)) == -1)
        {
            statusManager->print(StatusManager::Error,
                                 "Table \"" + tableName + "\" does not have the column \"" + columnName + "\"!");
            return -1;
        }

        for (int i = 0; i < noOfRows; i++)
        {
            if (rows[i][indexOfColumn] == nameOfValue)
            {
                remove_row(i);
                i--;
            }
        }
        return 0;
    }
};

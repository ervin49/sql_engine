#pragma once
#include "Table.h"

class Table_Catalog {
private:
    Table *tables;
    int noOfTables;

public:
    Table_Catalog() {
        tables = nullptr;
        noOfTables = 0;
    }

    bool operator==(const Table_Catalog &tableCatalog) const {
        if (this->noOfTables != tableCatalog.noOfTables) {
            return false;
        }

        for (int i = 0; i < noOfTables; i++) {
            if (this->tables[i] != tableCatalog.tables[i]) {
                return false;
            }
        }
        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, const Table_Catalog &table_catalog) {
        table_catalog.print_tables();
        return out;
    }


    int return_index_of_table(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName) {
                return i;
            }
        }
        return -1;
    }

    int drop_table(std::string tableName) {
        int index = return_index_of_table(tableName);
        if (index == -1) {
            std::cout << "Table does not exist!" << std::endl;
            return -1;
        }

        Table_Catalog *auxCatalog = new Table_Catalog;
        for (int i = 0; i < noOfTables; i++) {
            if (i == index)
                continue;
            auxCatalog->add_table(tables[i]);
        }

        this->tables = auxCatalog->getTables();
        this->setNoOfTables(noOfTables - 1);
        delete auxCatalog;
        return 0;
    }

    int add_table(Table newTable) {
        if (table_exists(newTable.getName())) {
            std::cout << "Table with this name already exists!";
            return -1;
        }
        //we create a new array of pointers to objects with updated size
        Table *newTables = new Table [noOfTables + 1];

        for (int i = 0; i < noOfTables; i++) {
            newTables[i] = tables[i];
        }

        //we add the new table at the end of the new array
        newTables[noOfTables] = newTable;

        delete[] tables;
        tables = newTables;
        noOfTables++;
        return 0;
    }

    int getNoOfTables() {
        return noOfTables;
    }

    //we iterate over the addresses array and compare each name
    //with the name we are checking for
    bool table_exists(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName) {
                //avem eroare
                return true;
            }
        }
        return false;
    }

    int getNumberOfColumns(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName)
                return tables[i].getNoOfColumns();
        }
        return 0;
    }

    Table *getTables() {
        Table *aux = new Table[noOfTables];
        for (int i = 0; i < noOfTables; i++) {
            aux[i] = tables[i];
        }
        return aux;
    }

    Table *getTable(std::string tableName) {
        Table *aux = nullptr;
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName) {
                aux = &tables[i];
                break;
            }
        }
        return aux;
    }

    void setTables(Table *tables) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == "") {
                std::cout << "Table names can't be empty!";
                return;
            }
        }

        this->tables = tables;
    }

    void setNoOfTables(int noOfTables) {
        if (noOfTables < 0) {
            std::cout << "Invalid!" << std::endl;
            return;
        }
        this->noOfTables = noOfTables;
    }

    //print all the tables
    void print_tables() const {
        std::cout << std::endl;
        for (Table *p = &tables[0]; p < &tables[noOfTables]; p++) {
            p->print_table();
            std::cout << std::endl << std::endl;
        }
    }
};

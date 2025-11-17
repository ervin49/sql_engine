#ifndef UNTITLED_CATALOG_H
#define UNTITLED_CATALOG_H
#include "Table.h"

class Catalog {
private:
    Table *tables;
    int noOfTables;

public:
    Catalog() {
        tables = nullptr;
        noOfTables = 0;
    }

    int return_index_of_table(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName) {
                return i;
            }
        }
        return -1;
    }

    void drop_table(std::string tableName) {
        Catalog *auxCatalog = new Catalog;
        int index = return_index_of_table(tableName);
        for (int i = 0; i < noOfTables; i++) {
            if (i == index)
                continue;
            auxCatalog->add_table(tables[i]);
        }

        this->tables = auxCatalog->getTables();
        this->setNoOfTables(noOfTables - 1);
        delete auxCatalog;
    }

    void add_table(Table newTable) {
        if (already_exists(newTable.getName())) {
            std::cout << "Table with this name already exists!";
            return;
        }
        //we create a new array of pointers to objects with updated size
        Table *newTables = new Table [noOfTables + 1];

        for (int i = 0; i < noOfTables; i++) {
            newTables[i] = tables[i];
        }

        //we add the new table at the end of the new array
        newTables[noOfTables] = newTable;
        tables = newTables;
        noOfTables++;
    }

    int getNoOfTables() {
        return noOfTables;
    }

    //we iterate over the addresses array and compare each name
    //with the name we are checking for
    bool already_exists(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i].getName() == tableName) {
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
    void print_tables() {
        std::cout << std::endl;
        for (Table *p = &tables[0]; p < &tables[noOfTables]; p++) {
            p->print_table();
            std::cout << std::endl << std::endl;
        }
    }
};

#endif //UNTITLED_CATALOG_H

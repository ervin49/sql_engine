#ifndef UNTITLED_CATALOG_H
#define UNTITLED_CATALOG_H
#include "Table.h"

class Catalog {
private:
    Table **tables;
    int noOfTables;

public:
    Catalog() {
        tables = nullptr;
        noOfTables = 0;
    }

    void add_table(Table *newTable) {
        //we create a new array of pointers to objects with updated size
        Table **newAddresses = new Table *[noOfTables + 1];
        //we initialize the new array of pointers with the pointers from the old array
        for (int i = 0; i < noOfTables; i++) {
            newAddresses[i] = tables[i]; // copiaza pointerul Table*
        }

        //we add the new table at the end of the new array
        newAddresses[noOfTables] = newTable;
        //we update the size and the addresses variable with the
        //new addresses variable we just made
        delete[] tables;
        noOfTables++;
        tables = newAddresses;
    }

    //we iterate over the addresses array and compare each name
    //with the name we are checking for
    bool already_exists(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i]->getName() == tableName) {
                return true;
            }
        }
        return false;
    }
    int getNumberOfColumns(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i]->getName() == tableName)
                return tables[i]->getNoOfColumns();
        }
        return 0;
    }

    Table* getTable(std::string tableName) {
        for (int i = 0; i < noOfTables; i++) {
            if (tables[i]->getName() == tableName) {
                return tables[i];
            }
        }
        return nullptr;
    }

    //print all the tables
    void print_tables() {
        for (Table *p = tables[0]; p < tables[noOfTables]; p++) {
            std::cout << p->getName();
            p->print_table();
            std::cout << std::endl << std::endl << std::endl;
        }
    }
};

#endif //UNTITLED_CATALOG_H

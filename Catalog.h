#ifndef UNTITLED_CATALOG_H
#define UNTITLED_CATALOG_H
#include "Table.h"

class Catalog {
private:

    Table **tables = nullptr;
    int noOfTables = 0;

public:
    void addTable(Table *newTable) {
        //we create a new array of pointers to objects with updated size
        Table **newAddresses = new Table *[noOfTables + 1];
        int i = 0;
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
    bool already_exists(std::string tableName) {
         //we iterate over the addresses array and compare each name
         //with the name we are checking for
         for (int i = 0; i < noOfTables; i++) {
             if (tables[i]->getName() == tableName) {
                 return true;
             }
         }
         return false;
     }
};




#endif //UNTITLED_CATALOG_H
//
// Created by ervin on 26.10.2025.
//

#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H
#include <string>

class Table {
private:
    int size = 0;
    std::string name;
    std::string *columns = nullptr;
    Table **addresses;

public:
    Table(int noOfColumns, std::string name) {
        this->name = name;
        columns = new std::string[noOfColumns];
        //we create a new array of pointers to objects with updated size
        Table **newAddresses = new Table *[size + 1];
        int i = 0;
        //we initialize the new array of pointers with the pointers from the old array
        for (Table **p = &(addresses[0]); p < &(addresses[size]); p++, i++) {
            newAddresses[i] = *p;
        }
        //we update the size and the addresses variable with the
        //new addresses variable we just made
        size++;
        addresses = newAddresses;
    }

    Table() {
    }

    static bool already_exists(std::string tableName) {
        //we iterate over the addresses array and compare each name
        //with the name we are checking for
        for (Table **p = &(addresses[0]); p < &(addresses[size]); p++) {
            if ((*p)->name == tableName) {
                return true;
            }
        }
        return false;
    }
};

#endif //UNTITLED_TABLE_H
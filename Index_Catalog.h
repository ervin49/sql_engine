//
// Created by ervin on 11/17/25.
//

#ifndef SQL_ENGINE_INDEX_CATALOG_H
#define SQL_ENGINE_INDEX_CATALOG_H
#include <iostream>
#include <string>

#include "Index.h"


class Index_Catalog {
private:
    Index *indexes;
    int noOfIndexes;

public:
    Index_Catalog() {
        indexes = nullptr;
        noOfIndexes = 0;
    }

    bool index_exists(std::string indexName) {
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getName() == indexName) {
                return true;
            }
        }
        return false;
    }

    int add_index(Index newIndex) {
        if (index_exists(newIndex.getName())) {
            std::cout << "Index with this name already exists!";
            return -1;
        }
        //we create a new array of pointers to objects with updated size
        Index *newIndexes = new Index [noOfIndexes + 1];

        for (int i = 0; i < noOfIndexes; i++) {
            newIndexes[i] = indexes[i];
        }

        //we add the new index at the end of the new array
        newIndexes[noOfIndexes] = newIndex;

        delete[] indexes;
        indexes = newIndexes;
        noOfIndexes++;
        return 0;
    }
};


#endif //SQL_ENGINE_INDEX_CATALOG_H

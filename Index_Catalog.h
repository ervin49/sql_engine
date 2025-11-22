//
// Created by ervin on 11/17/25.
//

#pragma once
#include <iostream>
#include <string>

#include "Index.h"
#include "Table.h"
#include "Table_Catalog.h"
#include "globals.h"

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
            if (indexes[i].getIndexName() == indexName) {
                return true;
            }
        }
        return false;
    }

    int add_index(Index newIndex) {
        if (index_exists(newIndex.getIndexName())) {
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

    void print_indexes() {
        std::cout << "Your indexes are:" << std::endl;
        for (Index *p = &indexes[0]; p < &indexes[noOfIndexes]; p++) {
            p->print_index();
        }
    }

    int drop_index(std::string indexName) {
        int position = return_position_of_index(indexName);
        if (position == -1) {
            std::cout << "Index " << indexName << " does not exist!" << std::endl;
            return -1;
        }

        Index *index = getIndex(indexName);
        std::string tableName = index->getTableName();
        Table *table = tableCatalog->getTable(tableName);
        table->setHasIndex(false);
        table->setIndex("", "");

        Index_Catalog *auxCatalog = new Index_Catalog;
        for (int i = 0; i < noOfIndexes; i++) {
            if (i == position)
                continue;
            auxCatalog->add_index(indexes[i]);
        }

        this->indexes = auxCatalog->getIndexes();
        this->setNoOfIndexes(noOfIndexes - 1);
        delete auxCatalog;
        return 0;
    }

    Index *getIndexes() {
        return this->indexes;
    }

    bool has_index(std::string columnName) {
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getColumnName() == columnName) {
                return true;
            }
        }
        return false;
    }

    Index *getIndex(std::string indexName) {
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getIndexName() == indexName)
                return &indexes[i];
        }
        return nullptr;
    }

    int return_position_of_index(std::string indexName) {
        int position = -1;
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getIndexName() == indexName) {
                position = i;
                break;
            }
        }
        return position;
    }

    void setNoOfIndexes(int noOfIndexes) {
        if (noOfIndexes < 0) {
            std::cout << "No. of indexes has to be at least 0!" << std::endl;
            return;
        }
        this->noOfIndexes = noOfIndexes;
    }
};

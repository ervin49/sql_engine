//
// Created by ervin on 11/17/25.
//

#pragma once
#include <iostream>
#include <string>

#include "Index.h"
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

    bool operator==(const Index_Catalog &indexCatalog) const {
        if (this->noOfIndexes != indexCatalog.noOfIndexes)
            return false;
        for (int i = 0; i < noOfIndexes; i++) {
            if (this->indexes[i] != indexCatalog.indexes[i])
                return false;
        }
        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, Index_Catalog indexCatalog) {
        indexCatalog.print_indexes();
        return out;
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
            std::cout << "ERROR: Index with name \"" << newIndex.getIndexName() << "\" already exists!" << std::endl;
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
        if (position != 0) {
            std::cout << "ERROR: Index \"" << indexName << "\" does not exist!" << std::endl;
            return -1;
        }
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

    bool has_index(std::string tableName, std::string columnName) {
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getTableName() == tableName && indexes[i].getColumnName() == columnName) {
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
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getIndexName() == indexName)
                return i;
        }
        return -1;
    }

    void setNoOfIndexes(int noOfIndexes) {
        if (noOfIndexes < 0) {
            std::cout << "ERROR: No. of indexes has to be at least 0!" << std::endl;
            return;
        }
        this->noOfIndexes = noOfIndexes;
    }

    int getNoOfIndexes() {
        return noOfIndexes;
    }

    int setIndex(std::string indexName, std::string columnName) {
        for (int i = 0; i < noOfIndexes; i++) {
            if (indexes[i].getIndexName() == indexName) {
                std::cout << "ERROR: Index with name \"" << indexName << "\" already exists!" << std::endl;
                return -1;
            }

            if (indexes[i].getColumnName() == columnName) {
                std::cout << "ERROR: Column \"" << columnName << "\" already has index \"" << indexes[i].getIndexName()
                        <<
                        "\"!" << std::endl;
                return -1;
            }
        }
        return 0;
    }
};

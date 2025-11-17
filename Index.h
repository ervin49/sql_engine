//
// Created by ervin on 11/17/25.
//

#ifndef SQL_ENGINE_INDEX_H
#define SQL_ENGINE_INDEX_H


class Index {
private:
    std::string indexName;
    std::string tableName;
    std::string columnName;

public:
    Index(std::string indexName, std::string tableName, std::string columnName) {
        this->indexName = indexName;
        this->tableName = tableName;
        this->columnName = columnName;
    }

    Index() = default;

    std::string getName() {
        return indexName;
    }
};


#endif //SQL_ENGINE_INDEX_H

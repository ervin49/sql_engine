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

    std::string getTableName() {
        return tableName;
    }

    std::string getIndexName() {
        return indexName;
    }

    void print_index() {
        std::cout << "Index \"" << indexName << "\" on column " << columnName << " from table " << tableName << '.' <<
                std::endl;
    }

    std::string getColumnName() {
        return columnName;
    };
};


#endif //SQL_ENGINE_INDEX_H

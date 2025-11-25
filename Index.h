#pragma once

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



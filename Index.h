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

    bool operator==(const Index &index) const {
        if (this->indexName != index.indexName ||
            this->tableName != index.tableName ||
            this->columnName != index.columnName)
            return false;

        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, const Index &index) {
        index.print_index();
        return out;
    }

    std::string getTableName() const {
        return tableName;
    }

    std::string getIndexName() const {
        return indexName;
    }

    void print_index() const {
        std::cout << "Index \"" << indexName << "\" on column " << columnName << " from table " << tableName << '.' <<
                std::endl;
    }

    std::string getColumnName() const {
        return columnName;
    }

    bool operator!=(const Index &index) const {
        if (*this == index)
            return false;
        return true;
    }
};



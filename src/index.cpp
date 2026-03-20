#include <iostream>
#include "index.hpp"

Index::Index(const std::string& indexName, const std::string& tableName, const std::string& columnName)
{
    this->indexName = indexName;
    this->tableName = tableName;
    this->columnName = columnName;
}

Index::Index() = default;

Index::Index(const Index& i)
{
    this->indexName = i.indexName;
    this->tableName = i.tableName;
    this->columnName = i.columnName;
}

Index::~Index() {}

Index& Index::operator=(const Index& i)
{
    if (this != &i)
    {
        this->indexName = i.indexName;
        this->tableName = i.tableName;
        this->columnName = i.columnName;
    }
    return *this;
}

Index::operator std::string() const { return this->indexName; }

bool Index::operator==(const Index& index) const
{
    if (this->indexName != index.indexName || this->tableName != index.tableName ||
            this->columnName != index.columnName)
        return false;

    return true;
}

bool Index::operator!() const { return indexName.empty(); }

bool Index::operator<(const Index& other) const { return this->indexName < other.indexName; }

std::ostream& operator<<(std::ostream& out, const Index& index)
{
    index.print_index();
    return out;
}

std::string Index::getTableName() const { return tableName; }

std::string Index::getIndexName() const { return indexName; }

void Index::print_index() const
{
    std::cout << "Index \"" << indexName << "\" on column \"" << columnName << "\"." << std::endl;
}

std::string Index::getColumnName() const { return columnName; }

bool Index::operator!=(const Index& index) const
{
    if (*this == index)
        return false;
    return true;
}
void Index::setTableName(const std::string& tableName)
{
    if (tableName.empty())
    {
        throw std::runtime_error("Table name can't be empty!");
    }
    this->tableName = tableName;
}
void Index::setIndexName(const std::string& indexName)
{
    if (indexName.empty())
    {
        throw std::runtime_error("Index name can't be empty!");
    }
    this->indexName = indexName;
}
void Index::setColumnName(const std::string& columnName)
{
    if (columnName.empty())
    {
        throw std::runtime_error("Column name can't be empty!");
    }
    this->columnName = columnName;
}

#pragma once
#include <iostream>

class Index
{
private:
	std::string indexName;
	std::string tableName;
	std::string columnName;

public:
	Index(std::string indexName, std::string tableName, std::string columnName)
	{
		this->indexName = indexName;
		this->tableName = tableName;
		this->columnName = columnName;
	}

	Index() = default;

	Index(const Index& i)
	{
		this->indexName = i.indexName;
		this->tableName = i.tableName;
		this->columnName = i.columnName;
	}

	~Index() {}

	Index& operator=(const Index& i)
	{
		if (this != &i)
		{
			this->indexName = i.indexName;
			this->tableName = i.tableName;
			this->columnName = i.columnName;
		}
		return *this;
	}

	operator std::string() const { return this->indexName; }

	bool operator==(const Index& index) const
	{
		if (this->indexName != index.indexName || this->tableName != index.tableName ||
			this->columnName != index.columnName)
			return false;

		return true;
	}

	bool operator!() const { return indexName.empty(); }

	bool operator<(const Index& other) const { return this->indexName < other.indexName; }

	friend std::ostream& operator<<(std::ostream& out, const Index& index)
	{
		index.print_index();
		return out;
	}

	std::string getTableName() const { return tableName; }

	std::string getIndexName() const { return indexName; }

	void print_index() const
	{
		std::cout << "Index \"" << indexName << "\" on column \"" << columnName << "\"." << std::endl;
	}

	std::string getColumnName() const { return columnName; }

	bool operator!=(const Index& index) const
	{
		if (*this == index)
			return false;
		return true;
	}
	void setTableName(const std::string& tableName)
	{
		if (tableName.empty())
		{
			throw std::runtime_error("Table name can't be empty!");
		}
		this->tableName = tableName;
	}
	void setIndexName(const std::string& indexName)
	{
		if (indexName.empty())
		{
			throw std::runtime_error("Index name can't be empty!");
		}
		this->indexName = indexName;
	}
	void setColumnName(const std::string& columnName)
	{
		if (columnName.empty())
		{
			throw std::runtime_error("Column name can't be empty!");
		}
		this->columnName = columnName;
	}
};

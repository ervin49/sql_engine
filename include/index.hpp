#pragma once
#include <iostream>
#include <string>

class Index
{
private:
	std::string indexName;
	std::string tableName;
	std::string columnName;

public:
	Index(const std::string& indexName, const std::string& tableName, const std::string& columnName);
	Index();
	Index(const Index& i);
	~Index();

	Index& operator=(const Index& i);
	operator std::string() const;
	bool operator==(const Index& index) const;
	bool operator!() const;
	bool operator<(const Index& other) const;
	bool operator!=(const Index& index) const;

	friend std::ostream& operator<<(std::ostream& out, const Index& index);

	std::string getTableName() const;
	std::string getIndexName() const;
	std::string getColumnName() const;

	void setTableName(const std::string& tableName);
	void setIndexName(const std::string& indexName);
	void setColumnName(const std::string& columnName);

	void print_index() const;
};

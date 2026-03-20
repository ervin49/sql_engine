#pragma once
#include <iostream>
#include <string>
#define OFFSET 10

class BaseTable
{
protected:
	unsigned int noOfColumns;
	unsigned int noOfRows;
	unsigned int noOfIndexes;
	std::string tableName;
	std::string* columnNames;
	std::string** rows;
	std::string* columnTypes;
	std::string* indexNames;
	unsigned int* maxColumnLengths;

public:
	BaseTable(const int noOfColumns, const std::string& tableName, const std::string* columnTypes);
	BaseTable(const BaseTable& other);
	BaseTable();
	BaseTable(int noOfColumns, const std::string& tableName);
	virtual ~BaseTable();

	bool operator==(BaseTable const& table) const;
	BaseTable& operator=(const BaseTable& other);
	operator int() const;
	BaseTable& operator+=(std::string* row);
	std::string* operator[](int index) const;
	bool operator!() const;
	friend std::ostream& operator<<(std::ostream& out, const BaseTable& table);
	bool operator!=(const BaseTable& table) const;

	bool isColumnType(const std::string& value, const std::string& columnType);
	bool isInteger(const std::string& str);
	bool isFloat(const std::string& str);

	std::string getTableName() const;
	std::string* getColumns() const;
	std::string** getRows() const;
	std::string* getColumnTypes() const;
	unsigned int getNoOfColumns() const;
	unsigned int getNoOfRows() const;
	unsigned int* getMaxColumnLengths() const;
	std::string* getIndexNames() const;
	int getNoOfIndexes() const;

	void setName(const std::string& name);
	void setIndexNames(std::string* indexNames, int noOfIndexes);
	void setColumnNames(const std::string* columnNames, int noOfColumns);
	void setColumnTypes(const std::string* columnTypes, int noOfColumns);
	void setColumn(const int index, const std::string& columnName);
	void setColumnType(const unsigned int index, const std::string& columnType);
	void setMaxColumnLengths(unsigned int* maxColumnLengths, int noOfColumns);
	void setRows(std::string** newRows, const unsigned int noOfRows, const unsigned int noOfColumns);
	void setNoOfRows(unsigned int newNoOfRows);
	void setNoOfColumns(unsigned int newNoOfColumns);

	bool columnExists(const std::string& columnName) const;

	int addRow(std::string* newRow, unsigned int noOfColumns);
	void addIndex(const std::string& indexName);

	int returnIndexOfColumnByName(const std::string& columnName) const;

	void removeRow(int index);
	void removeIndex(const std::string& indexName);
	int deleteFrom(const std::string& columnName, const std::string& nameOfValue);

	void printIndexes() const;
	void printTable(std::ostream& out, const std::string& synonymName) const;

	void indexTable(const std::string& indexName) const;
};

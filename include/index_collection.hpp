#pragma once
#include <dirent.h>
#include <iostream>
#include <string>
#include "index.hpp"
#include "globals.hpp"

class IndexCollection
{
private:
	Index* indexes;
	int noOfIndexes;

public:
	IndexCollection();
	IndexCollection(const Index* indexes, int noOfIndexes);
	IndexCollection(const IndexCollection& i);
	~IndexCollection();

	IndexCollection& operator=(const IndexCollection& i);
	operator int() const;
	bool operator==(const IndexCollection& indexCatalog) const;
	Index& operator[](int index) const;
	void operator+=(const Index& idx);

	friend std::ostream& operator<<(std::ostream& out, IndexCollection indexCatalog);


	Index* getIndex(const std::string& indexName) const;
	Index* getIndexes() const;
	int getNoOfIndexes() const;
	std::string* getColumnsOfIndexesByTableName(const std::string& tableName) const;
	int getNoOfIndexesOfTableByName(const std::string& tableName) const;

	int getPositionOfIndex(const std::string& indexName) const;

	void setNoOfIndexes(int noOfIndexes);
	int setIndex(const std::string& indexName, const std::string& columnName) const;
	void setIndexes(const Index* indexes, int noOfIndexes);

	bool indexExists(const std::string& indexName) const;

	int addIndex(const Index& newIndex);

	void printIndexes() const;

	void printIndexesOfTableByName(const std::string& tableName) const;

	void writeIndexCollectionToFile();

	int dropIndex(const std::string& indexName);

	bool hasIndex(const std::string& tableName, const std::string& columnName) const;
};
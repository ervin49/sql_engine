#pragma once
#include "table.hpp"

class TableCollection
{
    private:
        Table* tables;
        int noOfTables;

    public:
        TableCollection();
        TableCollection(const Table* tables, const int noOfTables);
        TableCollection(const TableCollection& that);
        ~TableCollection();

        TableCollection& operator=(const TableCollection& that);
        operator int() const;
        bool operator==(const TableCollection& table_catalog) const;
        Table& operator[](const int index) const;
        void operator+=(const Table& t);
        bool operator!() const;
        friend std::ostream& operator<<(std::ostream& out, const TableCollection& table_catalog);

        int addTable(const Table& newTable);
        int dropTable(const std::string& tableName);

        bool tableExists(const std::string& tableName) const;
        bool synonymExists(const std::string& synonym) const;

        std::string getActualTableName(const std::string& synonym) const;
        int getNoOfTables() const;
        int getNumberOfColumns(const std::string& tableName) const;
        int getIndexOfTable(const std::string& tableName) const;
        Table* getTables() const;
        Table* getTable(const std::string& tableName) const;

        void setTables(const Table* newTables, int newNoOfTables);
        void setNoOfTables(const int newNoOfTables);

        void printTables() const;
};

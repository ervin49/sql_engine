#pragma once
#include "Table.h"

class Table_Catalog
{
private:
    Table* tables;
    int noOfTables;

public:
    Table_Catalog()
    {
        tables = nullptr;
        noOfTables = 0;
    }
    Table_Catalog(Table* tables, int noOfTables) {
        if (noOfTables != 0 && tables != nullptr) {
            this->noOfTables = noOfTables;
            this->tables = new Table[noOfTables];
            for (int i = 0; i < noOfTables; i++) {
                this->tables[i] = tables[i];
            }
        }
    }

    Table_Catalog(const Table_Catalog& that) {
        if (that.noOfTables!=0 && that.tables!=nullptr) {
            this->noOfTables = that.noOfTables;
            this->tables = new Table[noOfTables];
            for (int i = 0; i < noOfTables; i++) {
                this->tables[i] = that.tables[i];
            }
        }
        else {
            this->noOfTables = 0;
            this->tables = nullptr;
        }
    }

    ~Table_Catalog() {
        if (tables != nullptr) {
            delete[] tables;
        }
    }

    Table_Catalog& operator=(const Table_Catalog& that) {
        if (this != &that) {
            if (this->tables != nullptr) {
                delete[] this->tables;
                this->tables = nullptr;
            }
            if (that.noOfTables!=0 && that.tables!=nullptr) {
                this->noOfTables = that.noOfTables;
                this->tables = new Table[noOfTables];
                for (int i = 0; i < noOfTables; i++) {
                    this->tables[i] = that.tables[i];
                }
            }
            else {
                this->noOfTables = 0;
            }
        }
        return *this;
    }

    operator int() const {
        return this->noOfTables;
    }

    bool operator==(const Table_Catalog& table_catalog) const
    {
        if (this->noOfTables != table_catalog.noOfTables)
        {
            return false;
        }

        for (int i = 0; i < noOfTables; i++)
        {
            if (this->tables[i] != table_catalog.tables[i])
            {
                return false;
            }
        }
        return true;
    }

    Table& operator[](const int index) const
    {
        return tables[index];
    }

    void operator+=(Table t) {
        this->add_table(t);
    }

    bool operator!() const {
        return noOfTables == 0;
    }

    friend std::ostream& operator<<(std::ostream& out, const Table_Catalog& table_catalog)
    {
        table_catalog.print_tables();
        return out;
    }


    int return_index_of_table(const std::string& tableName) const
    {
        for (int i = 0; i < noOfTables; i++)
        {
            if (tables[i].getName() == tableName)
            {
                return i;
            }
        }
        return -1;
    }

    int drop_table(const std::string& tableName)
    {
        int index = return_index_of_table(tableName);
        if (index == -1)
        {
            std::cout << "Table does not exist!" << std::endl;
            return -1;
        }

        Table_Catalog* auxCatalog = new Table_Catalog;
        for (int i = 0; i < noOfTables; i++)
        {
            if (i == index)
                continue;
            auxCatalog->add_table(tables[i]);
        }

        this->tables = auxCatalog->getTables();
        this->setNoOfTables(noOfTables - 1);
        delete auxCatalog;
        return 0;
    }

    int add_table(Table newTable)
    {
        if (table_exists(newTable.getName()))
        {
            std::cout << "Table with this name already exists!";
            return -1;
        }
        //we create a new array of pointers to objects with updated size
        Table* newTables = new Table [noOfTables + 1];

        for (int i = 0; i < noOfTables; i++)
        {
            newTables[i] = tables[i];
        }

        //we add the new table at the end of the new array
        newTables[noOfTables] = newTable;

        delete[] tables;
        tables = nullptr;
        tables = newTables;
        noOfTables++;
        return 0;
    }

    int getNoOfTables() const
    {
        return noOfTables;
    }

    //we iterate over the addresses array and compare each name
    //with the name we are checking for
    bool table_exists(const std::string& tableName) const
    {
        for (int i = 0; i < noOfTables; i++)
        {
            if (tables[i].getName() == tableName)
            {
                //we have error
                return true;
            }
        }
        return false;
    }

    int getNumberOfColumns(const std::string& tableName) const
    {
        for (int i = 0; i < noOfTables; i++)
        {
            if (tables[i].getName() == tableName)
                return tables[i].getNoOfColumns();
        }
        return 0;
    }

    Table* getTables() const
    {
        Table* aux = new Table[noOfTables];
        for (int i = 0; i < noOfTables; i++)
        {
            aux[i] = tables[i];
        }
        return aux;
    }

    Table* getTable(const std::string& tableName) const
    {
        Table* aux = nullptr;
        for (int i = 0; i < noOfTables; i++)
        {
            if (tables[i].getName() == tableName)
            {
                aux = &tables[i];
                break;
            }
        }
        return aux;
    }

    void setTables(Table* newTables)
    {
        for (int i = 0; i < noOfTables; i++)
        {
            if (newTables[i].getName() == "")
            {
                statusManager->print(StatusManager::Error, "Table names can't be empty!");
                return;
            }
        }

        this->tables = newTables;
    }

    void setNoOfTables(int newNoOfTables)
    {
        if (newNoOfTables < 0)
        {
            statusManager->print(StatusManager::Error, "Number of tables has to be at least 0!");
            return;
        }
        this->noOfTables = newNoOfTables;
    }

    //print all the tables
    void print_tables() const
    {
        std::cout << std::endl;
        for (Table* p = &tables[0]; p < &tables[noOfTables]; p++)
        {
            p->print_table();
            std::cout << std::endl << std::endl;
        }
    }
};

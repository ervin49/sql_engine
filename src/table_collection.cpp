#include "status_manager.hpp"
#include "table.hpp"
#include "table_collection.hpp"
#include "globals.hpp"

TableCollection::TableCollection()
{
    tables = nullptr;
    noOfTables = 0;
}

TableCollection::TableCollection(const Table* tables, const int noOfTables)
{
    if (noOfTables != 0 && tables != nullptr)
    {
        this->noOfTables = noOfTables;
        this->tables = new Table[noOfTables];
        for (int i = 0; i < noOfTables; i++)
        {
            this->tables[i] = tables[i];
        }
    }
}

TableCollection::TableCollection(const TableCollection& that)
{
    if (that.noOfTables != 0 && that.tables != nullptr)
    {
        this->noOfTables = that.noOfTables;
        this->tables = new Table[noOfTables];
        for (int i = 0; i < noOfTables; i++)
        {
            this->tables[i] = that.tables[i];
        }
    }
    else
    {
        this->noOfTables = 0;
        this->tables = nullptr;
    }
}

TableCollection::~TableCollection() { delete[] tables; }

TableCollection& TableCollection::operator=(const TableCollection& that)
{
    if (this != &that)
    {
        if (this->tables != nullptr)
        {
            delete[] this->tables;
            this->tables = nullptr;
        }
        if (that.noOfTables != 0 && that.tables != nullptr)
        {
            this->noOfTables = that.noOfTables;
            this->tables = new Table[noOfTables];
            for (int i = 0; i < noOfTables; i++)
            {
                this->tables[i] = that.tables[i];
            }
        }
        else
        {
            this->noOfTables = 0;
        }
    }
    return *this;
}

TableCollection::operator int() const { return this->noOfTables; }

bool TableCollection::operator==(const TableCollection& table_catalog) const
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

Table& TableCollection::operator[](const int index) const { return tables[index]; }

void TableCollection::operator+=(const Table& t) { this->addTable(t); }

bool TableCollection::operator!() const { return noOfTables == 0; }

std::ostream& operator<<(std::ostream& out, const TableCollection& table_catalog)
{
    table_catalog.printTables();
    return out;
}

int TableCollection::getIndexOfTable(const std::string& tableName) const
{
    for (int i = 0; i < noOfTables; i++)
    {
        if (tables[i].getTableName() == tableName)
        {
            return i;
        }

        if (tables[i].hasSynonym(tableName))
        {
            return i;
        }
    }
    return -1;
}

int TableCollection::dropTable(const std::string& tableName)
{
    const int index = getIndexOfTable(tableName);
    if (index == -1)
    {
        std::cout << "Table '" << tableName << "' does not exist!" << std::endl;
        return -1;
    }

    auto* auxCatalog = new TableCollection;
    for (int i = 0; i < noOfTables; i++)
    {
        if (i == index)
            continue;
        auxCatalog->addTable(tables[i]);
    }

    delete[] this->tables;
    this->tables = auxCatalog->getTables();
    this->setNoOfTables(noOfTables - 1);
    delete auxCatalog;
    return 0;
}

int TableCollection::addTable(const Table& newTable)
{
    if (tableExists(newTable.getTableName()))
    {
        std::cout << "Table with this name already exists!";
        return -1;
    }
    for (int i = 0; i < noOfTables; i++)
    {
        if (newTable.hasSynonym(tables[i].getTableName()))
        {
            std::cout << "Table with this name already exists!";
            return -1;
        }

        if (tables[i].hasSynonym(newTable.getTableName()))
        {
            std::cout << "Table with this name already exists!";
            return -1;
        }
    }
    // create a new array of pointers to objects with updated size
    Table* newTables = new Table[noOfTables + 1];

    for (int i = 0; i < noOfTables; i++)
    {
        newTables[i] = tables[i];
    }

    // add the new table at the end of the new array
    newTables[noOfTables] = newTable;

    delete[] tables;
    tables = newTables;
    noOfTables++;
    return 0;
}

int TableCollection::getNoOfTables() const { return noOfTables; }

bool TableCollection::tableExists(const std::string& tableName) const
{
    if (noOfTables == 0 || tables == nullptr)
    {
        return false;
    }
    for (int i = 0; i < noOfTables; i++)
    {
        if (tables[i].getTableName() == tableName)
        {
            return true;
        }

        if (tables[i].hasSynonym(tableName) == true)
        {
            return true;
        }
    }

    return false;
}

int TableCollection::getNumberOfColumns(const std::string& tableName) const
{
    for (int i = 0; i < noOfTables; i++)
    {
        if (tables[i].getTableName() == tableName)
            return tables[i].getNoOfColumns();
    }
    return 0;
}

Table* TableCollection::getTables() const
{
    Table* aux = new Table[noOfTables];
    for (int i = 0; i < noOfTables; i++)
    {
        aux[i] = tables[i];
    }
    return aux;
}

Table* TableCollection::getTable(const std::string& tableName) const
{
    for (int i = 0; i < noOfTables; i++)
    {
        if (tables[i].getTableName() == tableName)
        {
            return &tables[i];
        }

        if (tables[i].hasSynonym(tableName))
        {
            return &tables[i];
        }
    }

    return nullptr;
}

void TableCollection::setTables(const Table* newTables, int newNoOfTables)
{
    for (int i = 0; i < newNoOfTables; i++)
    {
        if (newTables[i].getTableName().empty())
        {
            statusManager->print(StatusManager::Error, "Table names can't be empty!");
            return;
        }
    }

    delete[] this->tables;
    this->tables = new Table[newNoOfTables];
    this->noOfTables = newNoOfTables;
    for (int i = 0; i < newNoOfTables; i++)
    {
        this->tables[i] = newTables[i];
    }
}

void TableCollection::setNoOfTables(const int newNoOfTables)
{
    if (newNoOfTables < 0)
    {
        statusManager->print(StatusManager::Error, "Number of tables has to be at least 0!");
        return;
    }
    this->noOfTables = newNoOfTables;
}

// print all the tables
void TableCollection::printTables() const
{
    std::cout << std::endl;
    for (const Table* p = &tables[0]; p < &tables[noOfTables]; p++)
    {
        p->printTable(std::cout, p->getTableName());
        std::cout << std::endl << std::endl;
    }
}

bool TableCollection::synonymExists(const std::string& synonym) const
{
    if (noOfTables == 0 || tables == nullptr)
    {
        return false;
    }

    for (int i = 0; i < noOfTables; i++)
    {
        if (tables[i].hasSynonym(synonym) == true)
        {
            return true;
        }
    }

    return false;
}

std::string TableCollection::getActualTableName(const std::string& synonym) const
{
    for (int i = 0; i < noOfTables; i++)
    {
        const Table& currTable = tables[i];
        if (currTable.hasSynonym(synonym))
        {
            return currTable.getTableName();
        }
    }

    return "";
}

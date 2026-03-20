#include <dirent.h>
#include <iostream>
#include <string>
#include <fstream>

#include "index.hpp"
#include "globals.hpp"
#include "status_manager.hpp"
#include "index_collection.hpp"

IndexCollection::IndexCollection()
{
    indexes = nullptr;
    noOfIndexes = 0;
}

IndexCollection::IndexCollection(const Index* indexes, const int noOfIndexes)
{
    if (noOfIndexes != 0 && indexes != nullptr)
    {
        this->noOfIndexes = noOfIndexes;
        this->indexes = new Index[noOfIndexes];
        for (int i = 0; i < noOfIndexes; i++)
            this->indexes[i] = indexes[i];
    }
}

IndexCollection::IndexCollection(const IndexCollection& i)
{
    if (i.indexes != nullptr && i.noOfIndexes != 0)
    {
        this->noOfIndexes = i.noOfIndexes;
        this->indexes = new Index[noOfIndexes];
        for (int j = 0; j < noOfIndexes; j++)
        {
            this->indexes[j] = i.indexes[j];
        }
    }
    else
    {
        this->noOfIndexes = 0;
    }
}

IndexCollection::~IndexCollection()
{
    if (indexes != nullptr)
    {
        delete[] indexes;
    }
}

IndexCollection& IndexCollection::operator=(const IndexCollection& i)
{
    if (this != &i)
    {
        if (this->indexes != nullptr)
        {
            delete[] this->indexes;
            this->indexes = nullptr;
        }
        if (i.indexes != nullptr && i.noOfIndexes != 0)
        {
            this->noOfIndexes = i.noOfIndexes;
            this->indexes = new Index[noOfIndexes];
            for (int j = 0; j < noOfIndexes; j++)
            {
                this->indexes[j] = i.indexes[j];
            }
        }
        else
        {
            this->noOfIndexes = 0;
        }
    }
    return *this;
}

IndexCollection::operator int() const { return this->noOfIndexes; }

bool IndexCollection::operator==(const IndexCollection& indexCatalog) const
{
    if (this->noOfIndexes != indexCatalog.noOfIndexes)
        return false;
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (this->indexes[i] != indexCatalog.indexes[i])
            return false;
    }
    return true;
}

Index& IndexCollection::operator[](int index) const { return indexes[index]; }

void IndexCollection::operator+=(const Index& idx) { this->addIndex(idx); }

std::ostream& operator<<(std::ostream& out, IndexCollection indexCatalog)
{
    indexCatalog.printIndexes();
    return out;
}

bool IndexCollection::indexExists(const std::string& indexName) const
{
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getIndexName() == indexName)
        {
            return true;
        }
    }
    return false;
}

int IndexCollection::addIndex(const Index& newIndex)
{
    if (indexExists(newIndex.getIndexName()) == true)
    {
        statusManager->print(StatusManager::Error,
                "Index with name '" + newIndex.getIndexName() + "' already exists!");
        return -1;
    }
    // we create a new array of pointers to objects with updated size
    Index* newIndexes = new Index[noOfIndexes + 1];

    for (int i = 0; i < noOfIndexes; i++)
    {
        newIndexes[i] = indexes[i];
    }

    // we add the new index at the end of the new array
    newIndexes[noOfIndexes] = newIndex;

    delete[] indexes;
    indexes = newIndexes;
    noOfIndexes++;
    return 0;
}

void IndexCollection::printIndexes() const
{
    std::cout << "Your indexes are:" << std::endl;
    for (Index* p = &indexes[0]; p < &indexes[noOfIndexes]; p++)
    {
        p->print_index();
    }
}

void IndexCollection::printIndexesOfTableByName(const std::string& tableName) const
{
    const int noOfIndexesOfTable = getNoOfIndexesOfTableByName(tableName);
    if (noOfIndexesOfTable == 1)
    {
        std::cout << "Table '" << tableName << "' has index: ";
        for (Index* p = &indexes[0]; p < &indexes[noOfIndexes]; p++)
        {
            if (p->getTableName() == tableName)
            {
                std::cout << '\'' << p->getIndexName() << "' on column '" << p->getColumnName() << "'.";
            }
        }
        std::cout << std::endl;
        return;
    }
    std::cout << "Table '" << tableName << "' has " << noOfIndexesOfTable << " indexes: ";
    int curr = 0;
    Index* p;
    for (p = &indexes[0]; p < &indexes[noOfIndexes] && curr < noOfIndexesOfTable - 1; p++)
    {
        if (p->getTableName() == tableName)
        {
            std::cout << '\'' << p->getIndexName() << "\' on column '" << p->getColumnName() << "', ";
            curr++;
        }
    }
    for (; p < &indexes[noOfIndexes]; p++)
    {
        if (p->getTableName() == tableName)
        {
            std::cout << '\'' << p->getIndexName() << "' on column '" << p->getColumnName() << "'.";
        }
    }
    std::cout << std::endl;
}

int IndexCollection::getNoOfIndexesOfTableByName(const std::string& tableName) const
{
    int noOfIndexesOfTable = 0;
    for (auto p = &indexes[0]; p < &indexes[noOfIndexes]; p++)
    {
        if (p->getTableName() == tableName)
        {
            noOfIndexesOfTable++;
        }
    }

    return noOfIndexesOfTable;
}

void IndexCollection::writeIndexCollectionToFile()
{
    // open the file(or create it if it doesn't exist already)
    DIR* dir = opendir("./index_catalog/");
    if (dir == nullptr)
    {
        system("mkdir ./index_catalog/");
    }
    else
    {
        closedir(dir);
    }
    std::ofstream file("./index_catalog/index_catalog.bin", std::ios::binary);
    file.write(reinterpret_cast<char*>(&noOfIndexes), sizeof(int));
    int len;
    for (int i = 0; i < noOfIndexes; i++)
    {
        len = indexes[i].getIndexName().length();
        file.write(reinterpret_cast<char*>(&len), sizeof(int));
        file.write(indexes[i].getIndexName().data(), len);

        len = indexes[i].getTableName().length();
        file.write(reinterpret_cast<char*>(&len), sizeof(int));
        file.write(indexes[i].getTableName().data(), len);

        len = indexes[i].getColumnName().length();
        file.write(reinterpret_cast<char*>(&len), sizeof(int));
        file.write(indexes[i].getColumnName().data(), len);
    }
    file.close();
}

int IndexCollection::dropIndex(const std::string& indexName)
{
    int position = getPositionOfIndex(indexName);
    if (position == -1)
    {
        statusManager->print(StatusManager::Error, "Index '" + indexName + "' does not exist!");
        return -1;
    }
    Index* indexes = new Index[noOfIndexes - 1];
    int k = 0;
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (i == position)
            continue;
        indexes[k++] = this->indexes[i];
    }

    delete[] this->indexes;
    this->indexes = indexes;
    noOfIndexes--;
    return 0;
}

Index* IndexCollection::getIndexes() const { return this->indexes; }

bool IndexCollection::hasIndex(const std::string& tableName, const std::string& columnName) const
{
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getTableName() == tableName && indexes[i].getColumnName() == columnName)
        {
            return true;
        }
    }
    return false;
}

Index* IndexCollection::getIndex(const std::string& indexName) const
{
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getIndexName() == indexName)
            return &indexes[i];
    }
    return nullptr;
}

int IndexCollection::getPositionOfIndex(const std::string& indexName) const
{
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getIndexName() == indexName)
            return i;
    }
    return -1;
}

void IndexCollection::setNoOfIndexes(int noOfIndexes)
{
    if (noOfIndexes < 0)
    {
        statusManager->print(StatusManager::Error, "Number of indexes has to be at least 0!");
        return;
    }
    this->noOfIndexes = noOfIndexes;
}

int IndexCollection::getNoOfIndexes() const { return noOfIndexes; }

int IndexCollection::setIndex(const std::string& indexName, const std::string& columnName) const
{
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getIndexName() == indexName)
        {
            statusManager->print(StatusManager::Error, "Index with name '" + indexName + "' already exists!");
            return -1;
        }

        if (indexes[i].getColumnName() == columnName)
        {
            statusManager->print(StatusManager::Error,
                    "Column '" + columnName + "' already has index '" + indexes[i].getIndexName() +
                    "'!");
            return -1;
        }
    }
    return 0;
}

void IndexCollection::setIndexes(const Index* indexes, int noOfIndexes)
{
    delete[] this->indexes;
    const auto newIndexes = new Index[noOfIndexes];
    for (int i = 0; i < noOfIndexes; i++)
    {
        newIndexes[i] = indexes[i];
    }
    this->indexes = newIndexes;
    this->noOfIndexes = noOfIndexes;
}

std::string* IndexCollection::getColumnsOfIndexesByTableName(const std::string& tableName) const
{
    std::string* columnsOfIndexes = new std::string[noOfIndexes];
    for (int i = 0; i < noOfIndexes; i++)
    {
        if (indexes[i].getTableName() == tableName)
        {
            columnsOfIndexes[i] = indexes[i].getColumnName();
        }
    }
    return columnsOfIndexes;
}

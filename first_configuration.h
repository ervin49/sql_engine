#pragma once
#include <dirent.h>
#include <iostream>

#include "globals.h"
#include "StatusManager.h"
#include "Table_Catalog.h"

class first_configuration
{
private:
    std::string target_path = "./tables/";

public:
    first_configuration() = default;

    first_configuration(std::string path)
    {
        this->target_path = path;
    }

    first_configuration(const first_configuration& other)
    {
        this->target_path = other.target_path;
    }

    ~first_configuration() = default;

    first_configuration& operator=(const first_configuration& other)
    {
        if (this != &other)
        {
            this->target_path = other.target_path;
        }
        return *this;
    }

    bool operator==(const first_configuration& other) const
    {
        return this->target_path == other.target_path;
    }

    bool operator!() const
    {
        return target_path.empty();
    }

    char operator[](int index) const
    {
        if (index >= 0 && index < target_path.length())
        {
            return target_path[index];
        }
        return '\0';
    }

    friend std::ostream& operator<<(std::ostream& out, const first_configuration& fc)
    {
        out << "Path: " << fc.target_path;
        return out;
    }

    operator std::string() const
    {
        return this->target_path;
    }

    void load_tables() const
    {
        DIR* dir = opendir(target_path.c_str());
        if (dir == nullptr)
        {
            return;
        }
        dirent* file;

        int noOfTables = -2; //we subtract "." and ".."
        while ((file = readdir(dir)) != nullptr)
        {
            noOfTables++;
        }
        const auto tableNames = new std::string[noOfTables];

        if (noOfTables <= 0)
        {
            closedir(dir);
            return;
        }

        closedir(dir);
        dir = opendir(target_path.c_str());

        int currIndex = 0;
        while ((file = readdir(dir)))
        {
            std::string fileName = file->d_name;
            if (fileName != "." && fileName != "..")
            {
                if (fileName.substr(fileName.length() - 4, 4) != ".bin")
                {
                    statusManager->print(StatusManager::Error,
                                         "File \"" + fileName + "\" does not have extension .bin!");
                    noOfTables--; //it's not a table
                    continue;
                }

                std::string tableName = fileName.substr(0, fileName.length() - 4);
                tableNames[currIndex++] = tableName;

                //we retrieve all data from the table file
                std::ifstream sameFile(target_path + fileName, std::ios::binary);
                if (!sameFile.is_open())
                {
                    statusManager->print(StatusManager::Error, "Can't open file!");
                    return;
                }

                Table* tempTable = read_table_from_file(target_path + fileName);
                tableCatalog->add_table(*tempTable);
                delete tempTable;
            }
        }

        if (noOfTables == 1)
        {
            std::cout << "Retrieved table \"" << tableNames[0] << "\" succesfully." << std::endl;
            return;
        }

        std::cout << "Retrieved tables ";
        for (int i = 0; i < noOfTables - 1; i++)
        {
            std::cout << '"' << tableNames[i] << "\", ";
        }

        std::cout << '"' << tableNames[noOfTables - 1] << "\" " << "successfully." << std::endl;
    }

    Table* read_table_from_file(std::string fileLocation) const
    {
        std::ifstream file(fileLocation);
        int noOfRows, noOfColumns, noOfIndexes;
        std::string *columns, *indexNames, *columnTypes;
        std::string** rows;
        unsigned int* maxColumnLengths;

        //retrieve variables
        file.read(reinterpret_cast<char*>(&noOfColumns), sizeof(int));
        file.read(reinterpret_cast<char*>(&noOfRows), sizeof(int));
        file.read(reinterpret_cast<char*>(&noOfIndexes), sizeof(int));

        int tableNameLength;
        file.read(reinterpret_cast<char*>(&tableNameLength), sizeof(int));
        std::string tableName;
        tableName.resize(tableNameLength);
        file.read(&tableName[0], tableNameLength);

        //allocate memory
        columns = new std::string[noOfColumns];
        columnTypes = new std::string[noOfColumns];
        maxColumnLengths = new unsigned int[noOfColumns];
        rows = new std::string*[noOfRows];
        for (int i = 0; i < noOfRows; i++)
        {
            rows[i] = new std::string [noOfColumns];
        }
        indexNames = new std::string[noOfIndexes];

        //retrieve arrays
        int len;
        for (int i = 0; i < noOfColumns; i++)
        {
            file.read(reinterpret_cast<char*>(&len), sizeof(int));
            columns[i].resize(len);
            file.read(&columns[i][0], len);

            file.read(reinterpret_cast<char*>(&len), sizeof(int));
            columnTypes[i].resize(len);
            file.read(&columnTypes[i][0], len);

            file.read(reinterpret_cast<char*>(&maxColumnLengths[i]), sizeof(int));
        }
        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                file.read(reinterpret_cast<char*>(&len), sizeof(int));
                rows[i][j].resize(len);
                file.read(&rows[i][j][0], len);
            }
        }
        for (int i = 0; i < noOfIndexes; i++)
        {
            file.read(reinterpret_cast<char*>(&len), sizeof(int));
            indexNames[i].resize(len);
            file.read(&indexNames[i][0], len);
        }

        //set the values
        auto table = new Table(noOfColumns, tableName);
        for (int i = 0; i < noOfColumns; i++)
        {
            table->setColumn(i, columns[i]);
        }
        table->setRows(rows, noOfRows, noOfColumns);
        table->setIndexNames(indexNames, noOfIndexes);
        table->setColumnTypes(columnTypes, noOfColumns);
        table->setMaxColumnLengths(maxColumnLengths, noOfColumns);
        return table;
    }
};

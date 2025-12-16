#pragma once
#include <dirent.h>     /* for DIR, opendir, readdir, and dirent */
#include <iostream>

#include "globals.h"
#include "StatusManager.h"
#include "Table_Catalog.h"

class first_configuration {
private:
    std::string target_path = "./tables/";

public:
    first_configuration() = default;

    first_configuration(std::string path) {
        this->target_path = path;
    }

    first_configuration(const first_configuration& other) {
        this->target_path = other.target_path;
    }

    ~first_configuration() {}

    first_configuration& operator=(const first_configuration& other) {
        if (this != &other) {
            this->target_path = other.target_path;
        }
        return *this;
    }

    bool operator==(const first_configuration& other) const {
        return this->target_path == other.target_path;
    }

    bool operator!() const {
        return target_path.empty();
    }

    char operator[](int index) const {
        if (index >= 0 && index < target_path.length()) {
            return target_path[index];
        }
        return '\0';
    }

    friend std::ostream& operator<<(std::ostream& out, const first_configuration& fc) {
        out << "Path: " << fc.target_path;
        return out;
    }

    operator std::string() const {
        return this->target_path;
    }

    void load_tables() const {
        DIR *dir = opendir(target_path.c_str());
        if (dir == nullptr) {
            statusManager->print(StatusManager::Error, "Wrong directory!");
            return;
        }
        dirent *file;

        int noOfTables = -2; //we subtract "." and ".."
        while ((file = readdir(dir)) != nullptr) {
            noOfTables++;
        }
        const auto tableNames = new std::string[noOfTables];

        if (noOfTables <= 0) {
            closedir(dir);
            return;
        }

        closedir(dir);
        dir = opendir(target_path.c_str());

        int currIndex = 0;
        while ((file = readdir(dir))) {
            std::string fileName = file->d_name;
            if (fileName != "." && fileName != "..") {
                if (fileName.substr(fileName.length() - 4, 4) != ".bin") {
                    statusManager->print(StatusManager::Error,
                                         "File \"" + fileName + "\" does not have extension .bin!");
                    continue;
                }

                std::string tableName = fileName.substr(0, fileName.length() - 4);
                tableNames[currIndex++] = tableName;

                //now we have to retrieve all data from the table file
                std::ifstream sameFile(target_path + fileName);
                Table tempTable;
                sameFile.read(reinterpret_cast<char *>(&tempTable), sizeof(Table));
                tableCatalog->add_table(tempTable);
            }
        }

        if (noOfTables == 1) {
            std::cout << "Retrieved table \"" << tableNames[0] << "\" succesfully." << std::endl;
            return;
        }

        std::cout << "Retrieved tables ";
        for (int i = 0; i < noOfTables - 1; i++) {
            std::cout << '"' << tableNames[i] << "\", ";
        }

        std::cout << '"' << tableNames[noOfTables - 1] << "\" " << "successfully." << std::endl;
    }
};

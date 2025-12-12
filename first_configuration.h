#pragma once
#include <dirent.h>     /* for DIR, opendir, readdir, and dirent */
#include <iostream>

#include "globals.h"
#include "StatusManager.h"
#include "Table_Catalog.h"

class first_configuration {
private:
    DIR *dir;
    std::string target_path = "./tables";

public:
    void load_tables() {
        dir = opendir(target_path.c_str());
        if (dir == nullptr) {
            statusManager->print(StatusManager::Error, "Wrong directory!");
            return;
        }
        dirent *file;

        int noOfTables = -2; //we subtract "." and ".."
        while ((file = readdir(dir)) != nullptr) {
            noOfTables++;
        }
        std::string *tableNames = new std::string[noOfTables];

        if (noOfTables <= 0) {
            return;
        }

        int currIndex = 0;
        while ((file = readdir(dir)) != nullptr) {
            if (file->d_name != "." && file->d_name != "..") {
                std::string fileName = file->d_name;
                if (fileName.substr(fileName.length() - 4, 4) != ".bin") {
                    statusManager->print(StatusManager::Error,
                                         "File \"" + fileName + "\" does not have extension .bin!");
                    continue;
                }

                std::string tableName = fileName.substr(0, fileName.length() - 4);
                tableNames[currIndex++] = tableName;
                tableCatalog->add_table(*tableCatalog->getTable(tableName));
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

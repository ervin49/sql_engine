#pragma once
#include <iostream>
#include "table.hpp"

class AppInitializer
{
private:
    std::string target_path = "./tables/";
    void loadIndexes() const;
    void loadTables() const;

public:
    AppInitializer();
    AppInitializer(const std::string& path);
    AppInitializer(const AppInitializer& other);
    ~AppInitializer();

    AppInitializer& operator=(const AppInitializer& other);
    bool operator==(const AppInitializer& other) const;
    bool operator!() const;
    char operator[](int index) const;
    friend std::ostream& operator<<(std::ostream& out, const AppInitializer& fc);
    operator std::string() const;


    static void printRetrievedTableMessage(const int noOfTables, const std::string* tableNames);

    static Table* readTableFromFile(const std::string& fileLocation);

    void run();
};

#pragma once
#include "app.hpp"

// indentation for status messages
#define STATUS_OFFSET 5

// this is to output bold text on the console
#define BOLD "\033[1m"

// this is to reset the bold text, otherwise
// all following text would be bold, not just the next word
#define RESET "\033[0m"


class MainMenu
{
private:
    App* app;
    bool firstEnter = true;
public:
    MainMenu();

    void displayWelcomeMenu();

    void runInteractiveMenu();

    void createIndex();

    void dropIndex();

    void displayIndexes();

    void showIndexOptions();

    void selectFrom();

    void updateTable();

    void deleteFrom();

    void dropTable();

    void createSynonym();

    void listTables();

    void insertInto();

    void showTableOptions();

    void createTable();

    void displayTable();

    static void printAvailableColumnsOfTable(const std::string& tableName);

    void printTableMenu();

    void pressAnyKeyToContinue();
};

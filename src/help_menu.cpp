#include "globals.hpp"
#include "table_collection.hpp"
#include "index_collection.hpp"
#include "help_menu.hpp"
#include <iostream>

#define BOLD "\033[1m"
#define RESET "\033[0m"

HelpMenu::HelpMenu() = default;
void HelpMenu::pressEnterToContinue()
{
    std::cout << std::endl << "Press Enter to return...";
    char c = _getch();
    while (c != '\n')
    {
        c = _getch();
    }
}

void HelpMenu::printSyntaxHelp()
{
    bool keepRunning = true;
    while (keepRunning)
    {
        clearScreen();
        std::cout << "========= SYNTAX CHEATSHEET =========" << std::endl << std::endl;
        std::cout << "Select a command to view details:" << std::endl << std::endl << std::endl;
        std::cout << "  1  CREATE TABLE" << std::endl;
        std::cout << "  2  CREATE SYNONYM" << std::endl<<std::endl;
        std::cout << "  3  SELECT" << std::endl;
        std::cout << "  4  LIST" << std::endl << std::endl;
        std::cout << "  5  INSERT INTO" << std::endl;
        std::cout << "  6  UPDATE & DELETE" << std::endl << std::endl;
        std::cout << "  7  INDEX Operations" << std::endl << std::endl;
        std::cout << "  r  Return to previous menu" << std::endl;
        std::cout << "  q  Quit" << std::endl << std::endl;
        std::cout << BOLD << "Choice: [1-6rq] " << RESET;

        switch (_getch())
        {
        case '1':
            clearScreen();
            std::cout << "--- CREATE TABLE ---" << std::endl;
            std::cout << "(!) Note: What is denoted between [] is not mandatory." << std::endl << std::endl;
            std::cout << "Format: CREATE TABLE table_name [IF NOT EXISTS] ((col_name, type, size, default), ...)"
                << std::endl;
            std::cout << "Types:  integer, text, float" << std::endl;
            std::cout << "E.g.:   CREATE TABLE students ((name, text, 20, -), (age, integer, 3, 0))" << std::endl;
            pressEnterToContinue();
            break;
        case '2':
            clearScreen();
            std::cout << "--- CREATE SYNONYM ---" << std::endl;
            std::cout << "Format: CREATE SYNONYM synonym_name FOR table_name" << std::endl;
            std::cout << "E.g.:   CREATE SYNONYM learners FOR students" << std::endl;
            pressEnterToContinue();
            break;
        case '3':
            clearScreen();
            std::cout << "--- SELECT ---" << std::endl;
            std::cout << "(!) Note: What is denoted between [] is not mandatory." << std::endl << std::endl;
            std::cout << "Format: SELECT (col1, col2, ...) | ALL FROM table_name [WHERE col = val]" << std::endl;
            std::cout << "E.g.:   SELECT name FROM students WHERE age = 21" << std::endl;
            pressEnterToContinue();
            break;
        case '4':
            clearScreen();
            std::cout << "--- LIST COMMAND ---" << std::endl;
            std::cout << "FORMAT: LIST" << std::endl;
            std::cout << "Displays all current available tables with their synonyms and indexes" << std::endl;
            pressEnterToContinue();
            break;
        case '5':
            clearScreen();
            std::cout << "--- INSERT INTO ---" << std::endl;
            std::cout << "Format: INSERT INTO table_name VALUES (val1, val2, ...)" << std::endl;
            std::cout << "E.g.:   INSERT INTO students VALUES ('Alex', 21)" << std::endl;
            pressEnterToContinue();
            break;
        case '6':
            clearScreen();
            std::cout << "--- UPDATE & DELETE ---" << std::endl;
            std::cout << "Update: UPDATE table SET col = new_val WHERE col = target_val" << std::endl;
            std::cout << "Delete: DELETE FROM table WHERE col = target_val" << std::endl;
            pressEnterToContinue();
            break;
        case '7':
            clearScreen();
            std::cout << "--- INDEX COMMANDS ---" << std::endl;
            std::cout << "Create: CREATE INDEX index_name ON table_name (column_name)" << std::endl;
            std::cout << "Drop:   DROP INDEX index_name" << std::endl;
            pressEnterToContinue();
            break;
        case 'r':
            keepRunning = false;
            break;
        case 'q':
            exit(0);
        default:
            break;
        }
    }
}

void HelpMenu::printDatatypesHelp()
{
    clearScreen();
    std::cout << "================ SUPPORTED DATA TYPES ================" << std::endl << std::endl;
    std::cout << "1. INTEGER (integer / int)" << std::endl;
    std::cout << "   - Whole numbers (e.g., 10, -5). Used for IDs, Counts." << std::endl << std::endl;

    std::cout << "2. FLOAT (float / numeric)" << std::endl;
    std::cout << "   - Decimal numbers (e.g., 10.5). Used for Prices, Averages." << std::endl << std::endl;

    std::cout << "3. TEXT (text / varchar)" << std::endl;
    std::cout << "   - Strings (e.g., 'Hello'). Used for Names, Descriptions." << std::endl << std::endl;

    std::cout << "----------------------------" << std::endl;
    pressEnterToContinue();
}

void HelpMenu::printGeneralHelp()
{
    clearScreen();
    std::cout << "================ SYSTEM INFORMATION ================" << std::endl << std::endl;

    std::cout << "[ LIVE STATISTICS ]" << std::endl;
    std::cout << "Tables loaded:  " << tableCollection->getNoOfTables() << std::endl;
    std::cout << "Indexes loaded: " << indexCollection->getNoOfIndexes() << std::endl;
    std::cout << "----------------------------------------------------" << std::endl << std::endl;

    std::cout << "[ STORAGE LOCATIONS ]" << std::endl;
    std::cout << "  - Tables  -> ./tables" << std::endl;
    std::cout << "  - Indexes -> ./indexes" << std::endl << std::endl;

    std::cout << "[ TIPS ]" << std::endl;
    std::cout << "  - Use 'q' to Quit, 'r' to Return." << std::endl;
    std::cout << "  - You can import .csv files using the IMPORT command." << std::endl;

    std::cout << "--------------------" << std::endl;
    pressEnterToContinue();
}

void HelpMenu::displayHelp()
{
    bool keepRunning = true;
    while (keepRunning)
    {
        clearScreen();
        std::cout << std::endl;
        std::cout << "Help Center" << std::endl << std::endl;
        std::cout << "  1  Syntax guide (commands & examples)" << std::endl;
        std::cout << "  2  Data types reference" << std::endl;
        std::cout << "  3  General information" << std::endl << std::endl;
        std::cout << "  r  Return to previous menu" << std::endl;
        std::cout << "  q  Quit" << std::endl << std::endl;
        std::cout << BOLD << "Choice: [123rq] " << RESET;

        switch (_getch())
        {
        case '1':
            printSyntaxHelp();
            break;
        case '2':
            printDatatypesHelp();
            break;
        case '3':
            printGeneralHelp();
            break;
        case 'r':
            keepRunning = false;
            break;
        case 'q':
            exit(0);
        default:
            break;
        }
    }
}

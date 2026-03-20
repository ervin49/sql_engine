#include <iostream>
#include <limits>
#include "app.hpp"
#include "main_menu.hpp"
#include "globals.hpp"
#include "help_menu.hpp"
#include "status_manager.hpp"
#include "app_initializer.hpp"
#include "table_collection.hpp"
#include "index_collection.hpp"

// indentation for status messages
#define STATUSoFFSET 5

// this is to output bold text on the console
#define BOLD "\033[1m"

// this is to reset the bold text, otherwise
// all following text would be bold, not just the next word
#define RESET "\033[0m"


MainMenu::MainMenu() { app = new App(); }

void MainMenu::displayWelcomeMenu()
{
    clearScreen();
    if (firstEnter == true)
    {
        AppInitializer initializer;
        initializer.run();
        firstEnter = false;
    }

    std::cout << std::endl;
    std::cout << "Welcome to Sql Engine! Please choose what you want to do." << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "(1) Run commands using the interactive menu." << std::endl << std::endl;
    std::cout << "(2) Run commands using plain old sql queries." << std::endl << std::endl;
    std::cout << "(h) Display help menu." << std::endl << std::endl;
    std::cout << "(q) Quit program." << std::endl << std::endl;
    std::cout << BOLD << "Choice: [12hq] " << RESET;

    while (true)
    {
        switch (_getch())
        {
        case '1':
            clearScreen();
            runInteractiveMenu();
            break;
        case '2':
            clearScreen();
            app->parseCommands();
            displayWelcomeMenu();
            break;
        case 'h':
            clearScreen();
            helpMenu->displayHelp();
            break;
        case 'q':
            clearScreen();
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::runInteractiveMenu()
{
    std::cout << std::endl;
    std::cout << "Welcome to the Sql Engine interactive menu! What would you like to work with?" << std::endl
        << std::endl
        << std::endl;
    std::cout << "(1) A table." << std::endl << std::endl;
    std::cout << "(2) An index." << std::endl << std::endl;
    std::cout << "(r) Return to the previous menu." << std::endl;
    std::cout << "(q) Quit program." << std::endl << std::endl;
    std::cout << BOLD << "Choice: [12rq] " << RESET;

    while (true)
    {
        switch (_getch())
        {
        case '1':
            clearScreen();
            showTableOptions();
            break;
        case '2':
            clearScreen();
            showIndexOptions();
            break;
        case 'r':
            clearScreen();
            displayWelcomeMenu();
            break;
        case 'q':
            clearScreen();
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::createIndex()
{
    std::cout << std::endl;
    std::string option, columnName;
    printAvailableTables();
    std::cout << "Enter table name (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option))
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name (or 'return' / 'quit')" << std::endl;
        std::cout << "> ";
    }
    const std::string tableName = tableCollection->getActualTableName(option);
    Table* table = tableCollection->getTable(tableName);

    printAvailableColumnsOfTable(tableName);
    std::cout << "Enter column to index:" << std::endl;
    std::cout << "> ";

    while (true)
    {
        std::cin >> columnName;
        if (table->columnExists(columnName) == true && indexCollection->hasIndex(tableName, columnName) == false)
        {
            break;
        }
        if (table->columnExists(columnName) == false)
        {
            statusManager->print(StatusManager::Error,
                    "Table '" + option + "' does not have column '" + columnName + "'!");
            std::cout << std::endl;
            std::cout << "Enter column to index:" << std::endl;
            std::cout << "> ";
        }
        else if (indexCollection->hasIndex(tableName, columnName) == true)
        {
            statusManager->print(StatusManager::Error,
                    "Table '" + option + "' already has an index on column '" + columnName + "'!");
            std::cout << std::endl;
            std::cout << "Enter column to index:" << std::endl;
            std::cout << "> ";
        }
    }

    std::string indexName;
    std::cout << "Enter index name:" << std::endl;
    std::cout << "> ";
    while (true)
    {
        std::cin >> indexName;
        if (indexCollection->indexExists(indexName) == false)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Index '" + indexName + "' already exists!");
        std::cout << std::endl;
        std::cout << "Enter index name:" << std::endl;
        std::cout << "> ";
    }
    auto index = new Index(indexName, tableName, columnName);
    indexCollection->addIndex(*index);
    table->addIndex(indexName);
    app->writeTableToFile(*table);
    indexCollection->writeIndexCollectionToFile();
    statusManager->print(StatusManager::Success, "Index '" + indexName + "' created successfully!");

    delete index;
    delete table;
    showMenuLoop(index_options);
}

void MainMenu::dropIndex()
{
    std::cout << std::endl;
    std::cout << "Enter index name to drop (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        int position = indexCollection->getPositionOfIndex(option);
        if (position != -1)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Index '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter index name to drop (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    if (indexCollection->dropIndex(option) == 0)
    {
        const std::string tableNameOfIndex = indexCollection->getIndex(option)->getTableName();
        const auto table = tableCollection->getTable(tableNameOfIndex);
        table->removeIndex(option);
        indexCollection->writeIndexCollectionToFile();

        statusManager->print(StatusManager::Success, "Index '" + option + "' dropped successfully!");

        delete table;
    }
    showMenuLoop(index_options);
}

void MainMenu::displayIndexes()
{
    std::cout << std::endl;
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        std::cout << "You have to create a table first!" << std::endl << std::endl;
        return;
    }

    printAvailableTables();
    std::cout << "Enter table name to display indexes (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to display indexes (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    const std::string tableName = tableCollection->getActualTableName(option);
    if (indexCollection->getNoOfIndexesOfTableByName(tableName) == 0)
    {
        std::cout << "Table '" << option << "' does not have any index!" << std::endl;
        showMenuLoop(index_options);
    }

    indexCollection->printIndexesOfTableByName(tableName);
    showMenuLoop(index_options);
}

void MainMenu::showMenuLoop(MenuOptions option)
{
    std::cout << std::endl;
    std::cout << "What would you like to do now?" << std::endl;
    std::cout << "(r) Return to the previous menu." << std::endl;
    std::cout << "(q) Quit the program." << std::endl;
    std::cout << BOLD << "Choice: [rq] " << RESET;
    while (true)
    {
        switch (_getch())
        {
        case 'r':
            clearScreen();
            switch (option)
            {
            case index_options:
                showIndexOptions();
                break;
            case table_options:
                showTableOptions();
                break;
            default:
                break;
            }
            break;
        case 'q':
            clearScreen();
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::showIndexOptions()
{
    std::cout << std::endl;
    std::cout << "What would you like to do?" << std::endl << std::endl << std::endl;
    std::cout << "(1) Create an index." << std::endl << std::endl;
    std::cout << "(2) Display all indexes of a table." << std::endl << std::endl;
    std::cout << "(3) Drop an index." << std::endl << std::endl;
    std::cout << "(r) Return to the previous menu." << std::endl;
    std::cout << "(q) Quit program." << std::endl << std::endl;
    std::cout << BOLD << "Choice: [123rq] " << RESET;

    while (true)
    {
        switch (_getch())
        {
        case '1':
            clearScreen();
            createIndex();
            showIndexOptions();
            break;
        case '2':
            clearScreen();
            displayIndexes();
            showIndexOptions();
            break;
        case '3':
            clearScreen();
            dropIndex();
            showIndexOptions();
            break;
        case 'r':
            clearScreen();
            runInteractiveMenu();
            break;
        case 'q':
            clearScreen();
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::selectFrom()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }
    std::cout << "Enter table name to select from (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to select from:" << std::endl;
        std::cout << "> ";
    }
    const std::string& synonymName = option;
    Table* originalTable = tableCollection->getTable(synonymName);

    printAvailableColumnsOfTable(synonymName);
    std::cout << "Enter columns to select, separated by spaces, or 'ALL' to select all "
        "columns: "
        << std::endl;
    std::cout << "[E.g.: col1 col2 col3 ...]" << std::endl;
    std::cout << "> ";

    auto selectedColumns = new std::string[50];
    std::string columnName;
    int noOfSelectedColumns;
    bool checkForAll = false;
    while (true)
    {
        delete[] selectedColumns;
        selectedColumns = new std::string[50]; // just hope 50 is enough
        noOfSelectedColumns = 0;
        while (true)
        {
            std::cin >> columnName;
            selectedColumns[noOfSelectedColumns++] = columnName;
            char nextChar = std::cin.get();
            if (nextChar == '\n')
            {
                break;
            }
        }
        if ((selectedColumns[0] == "all" || selectedColumns[0] == "ALL") && noOfSelectedColumns == 1)
        {
            checkForAll = true;
            break;
        }

        bool hasError = false;
        for (int i = 0; i < noOfSelectedColumns; i++)
        {
            if (originalTable->columnExists(selectedColumns[i]) == false)
            {
                std::cout << "Column '" << selectedColumns[i] << "' does not exist in table '" + option + "'!"
                    << std::endl;
                hasError = true;
            }
        }
        if (hasError == false)
        {
            break;
        }
        std::cout << "Enter columns to select, separated by spaces, or 'ALL' to select all "
            "columns: "
            << std::endl;
        std::cout << "> ";
    }

    std::cout << std::endl;
    std::cout << "Search based on the value of a column?" << std::endl;
    std::cout << "(y) Yes" << std::endl;
    std::cout << "(n) No" << std::endl;
    std::cout << BOLD << "Choice: [yn] " << RESET;

    bool searchWith_column_name;
    std::string columnNameSearchedFor, value;
    while (true)
    {
        switch (_getch())
        {
        case 'y':
            std::cout << std::endl << std::endl;
            std::cout << "Enter column name and value to search for, separated by spaces: " << std::endl;
            std::cout << "[E.g.: firstName Andrei]" << std::endl;
            std::cout << "> ";
            while (true)
            {
                std::cin >> columnNameSearchedFor >> value;
                if (originalTable->columnExists(columnNameSearchedFor) == true)
                {
                    break;
                }
                statusManager->print(StatusManager::Error,
                        "Column '" + columnNameSearchedFor + "' does not exist!");
                std::cout << std::endl;
                std::cout << "Enter column value to search for, separated by spaces:" << std::endl;
                std::cout << "> ";
            }
            searchWith_column_name = true;
            break;
        case 'n':
            searchWith_column_name = false;
            break;
        default:
            statusManager->print(StatusManager::Error, "You need to enter a valid option!");
            std::cout << std::endl;
            std::cout << BOLD << "Choice: [yn] " << RESET;
            continue;
        }
        break;
    }
    if (checkForAll == false)
    {
        auto* tableWithSelectedColumnsOnly = new Table(noOfSelectedColumns, "");

        tableWithSelectedColumnsOnly->setNoOfRows(originalTable->getNoOfRows());
        const auto* columnsOfOriginalTable = originalTable->getColumns();
        const int noOfRows = originalTable->getNoOfRows();
        std::string** rowsOfOriginalTable = originalTable->getRows();
        std::string** rowsOfNewTable = tableWithSelectedColumnsOnly->getRows();
        const int noOfColumnsOfOriginalTable = originalTable->getNoOfColumns();
        // set the column names
        for (int i = 0; i < noOfSelectedColumns; i++)
        {
            tableWithSelectedColumnsOnly->setColumn(i, selectedColumns[i]);
        }

        // set the row values, only on the columns that we need
        int k = 0;
        while (k < noOfSelectedColumns)
        {
            for (int i = 0; i < noOfColumnsOfOriginalTable && k < noOfSelectedColumns; i++)
            {
                if (columnsOfOriginalTable[i] == selectedColumns[k])
                {
                    for (int j = 0; j < noOfRows; j++)
                    {
                        rowsOfNewTable[j][k] = rowsOfOriginalTable[j][i];
                    }
                    k++;
                }
            }
        }
        tableWithSelectedColumnsOnly->setRows(rowsOfNewTable, noOfRows, noOfSelectedColumns);

        if (searchWith_column_name == true)
        {
            const auto tableWithSelectedRows = new Table(noOfSelectedColumns, "");
            for (int i = 0; i < noOfSelectedColumns; i++)
            {
                tableWithSelectedRows->setColumn(i, selectedColumns[i]);
            }
            const int index = tableWithSelectedRows->returnIndexOfColumnByName(columnNameSearchedFor);
            bool found = false;
            for (int i = 0; i < noOfRows; i++)
            {
                for (int j = 0; j < noOfSelectedColumns; j++)
                {
                    if (rowsOfNewTable[i][j] == value && j == index)
                    {
                        found = true;
                        tableWithSelectedRows->addRow(rowsOfNewTable[i], noOfSelectedColumns);
                    }
                }
            }
            if (!found)
            {
                statusManager->print(StatusManager::Error,
                        "No matching values for '" + value + "' in column '" + columnNameSearchedFor +
                        "'!");
                showMenuLoop(table_options);
            }
            app->writeSelectToFile(*tableWithSelectedRows, synonymName);
            tableWithSelectedRows->printTable(std::cout, synonymName);

            delete tableWithSelectedColumnsOnly;
            delete tableWithSelectedRows;
            for (int i = 0; i < noOfRows; i++)
            {
                delete[] rowsOfOriginalTable[i];
                delete[] rowsOfNewTable[i];
            }
            delete[] columnsOfOriginalTable;
            delete[] rowsOfNewTable;
            delete[] rowsOfOriginalTable;
        }
        else
        {
            app->writeSelectToFile(*tableWithSelectedColumnsOnly, synonymName);
            tableWithSelectedColumnsOnly->printTable(std::cout, synonymName);

            delete tableWithSelectedColumnsOnly;
            for (int i = 0; i < noOfRows; i++)
            {
                delete rowsOfOriginalTable[i];
                delete rowsOfNewTable[i];
            }
            delete[] columnsOfOriginalTable;
            delete[] rowsOfNewTable;
            delete[] rowsOfOriginalTable;
        }
    }
    else
    {
        if (searchWith_column_name == true)
        {
            bool found = false;
            const int index = originalTable->returnIndexOfColumnByName(columnNameSearchedFor);
            const int noOfRows = originalTable->getNoOfRows();
            const int noOfColumns = originalTable->getNoOfColumns();
            std::string** rowsOfOriginalTable = originalTable->getRows();
            auto* tableWithSelectedRows = new Table(noOfColumns, "");
            std::string* columnTypes = originalTable->getColumnTypes();
            unsigned int* maxColumnLengths = originalTable->getMaxColumnLengths();
            tableWithSelectedRows->setMaxColumnLengths(maxColumnLengths, noOfColumns);
            tableWithSelectedRows->setColumnTypes(columnTypes, noOfColumns);

            std::string* selectedColumns = originalTable->getColumns();
            for (int i = 0; i < noOfColumns; i++)
            {
                tableWithSelectedRows->setColumn(i, selectedColumns[i]);
            }

            for (int i = 0; i < noOfRows; i++)
            {
                for (int j = 0; j < noOfColumns; j++)
                {
                    if (rowsOfOriginalTable[i][j] == value && j == index)
                    {
                        // it matches what we are searching for
                        found = true;
                        tableWithSelectedRows->addRow(rowsOfOriginalTable[i], noOfColumns);
                    }
                }
            }

            if (!found)
            {
                statusManager->print(StatusManager::Error,
                        "No matching values for: '" + value + "' in column: '" +
                        columnNameSearchedFor + "'!");
            }
            else
            {
                tableWithSelectedRows->printTable(std::cout, synonymName);
                app->writeSelectToFile(*tableWithSelectedRows, synonymName);
                delete[] maxColumnLengths;
                delete[] columnTypes;
                delete tableWithSelectedRows;
                delete[] rowsOfOriginalTable;
            }
        }
        else
        {
            tableCollection->getTable(option)->printTable(std::cout, synonymName);
            app->writeSelectToFile(*tableCollection->getTable(synonymName), synonymName);
        }
    }
    showMenuLoop(table_options);
}

void MainMenu::updateTable()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name to update (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to update (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    const std::string tableName = tableCollection->getActualTableName(option);
    printAvailableColumnsOfTable(tableName);
    std::cout << "Enter column to change and value to search for, separated by spaces: " << std::endl;
    std::cout << "[E.g.: id 101]" << std::endl;
    std::cout << "> ";
    std::string whereColumn, whereValue;
    const auto table = tableCollection->getTable(option);
    while (true)
    {
        std::cin >> whereColumn >> whereValue;
        if (table->columnExists(whereColumn) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error,
                "Column '" + whereColumn + "' does not exist in table '" + option + "'!");
        std::cout << std::endl;
        std::cout << "Enter column to change and value to search for, separated by spaces: " << std::endl;
        std::cout << "> ";
    }
    std::cout << "Enter column name to update and the new value, separated by spaces:" << std::endl;
    std::cout << "> ";

    std::string setColumn, setValue;
    while (true)
    {
        std::cin >> setColumn >> setValue;
        if (table->columnExists(setColumn) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error,
                "Column '" + setColumn + "' does not exist in table '" + option + "'!");
        std::cout << std::endl;
        std::cout << "Enter column name to update and the new value, separated by spaces:" << std::endl;
        std::cout << "> ";
    }
    const int setIndex = table->returnIndexOfColumnByName(setColumn);
    const int whereIndex = table->returnIndexOfColumnByName(setValue);
    std::string** tableRows = table->getRows();
    const int noOfRows = table->getNoOfRows();
    for (int i = 0; i < table->getNoOfRows(); i++)
    {
        if (tableRows[i][whereIndex] == whereValue)
        {
            tableRows[i][setIndex] = setValue;
        }
    }
    table->setRows(tableRows, table->getNoOfRows(), table->getNoOfColumns());
    app->writeTableToFile(*table);
    statusManager->print(StatusManager::Success, "Updated table successfully!");

    for (int i = 0; i < noOfRows; i++)
    {
        delete[] tableRows[i];
    }
    delete[] tableRows;
    delete table;
    showMenuLoop(table_options);
}

void MainMenu::deleteFrom()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name to delete from (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to delete from (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    printAvailableColumnsOfTable(option);
    std::cout << "Enter column of the value and the value to delete, separated by space:" << std::endl;
    std::cout << "[E.g.: id 101]" << std::endl;
    std::cout << "> ";
    std::string whereColumn, whereValue;
    const auto table = tableCollection->getTable(option);
    while (true)
    {
        std::cin >> whereColumn >> whereValue;
        if (table->columnExists(whereColumn) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error,
                "Column '" + whereColumn + "' does not exist in table '" + option + "'!");

        std::cout << std::endl;
        std::cout << "Enter column of the value and the value to delete, separated by space:" << std::endl;
        std::cout << "> ";
    }
    if (table->deleteFrom(whereColumn, whereValue) == 0)
    {
        statusManager->print(StatusManager::Success, "Deleted successfully!");
        app->writeTableToFile(*table);
        delete table;
        showMenuLoop(table_options);
    }
    else
    {
        delete table;
        showMenuLoop(table_options);
    }
}

void MainMenu::dropTable()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name to drop (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";
    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to drop (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    const std::string actualTableName = tableCollection->getActualTableName(option);
    if (remove(("./tables/" + actualTableName + ".bin").data()) != 0)
    {
        statusManager->print(StatusManager::Error, "Could not remove file '" + actualTableName + ".bin'!");
        showMenuLoop(table_options);
    }
    if (tableCollection->dropTable(option) == 0)
    {
        statusManager->print(StatusManager::Success, "Table '" + option + "' dropped successfully!");
        showMenuLoop(table_options);
    }
    else
    {
        showMenuLoop(table_options);
    }
}

void MainMenu::createSynonym()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name for the synonym (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name for the synonym (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }

    std::cout << std::endl;
    std::cout << "Enter synonym name to create:" << std::endl;
    std::cout << "> ";
    std::string synonymName;
    while (true)
    {
        std::cin >> synonymName;
        if (tableCollection->tableExists(synonymName) == false)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + synonymName + "' already exists!");
        std::cout << std::endl;
        std::cout << "Enter synonym name to create:" << std::endl;
        std::cout << "> ";
    }

    Table* table = tableCollection->getTable(option);
    table->addSynonym(synonymName);
    app->writeTableToFile(*table);

    statusManager->print(StatusManager::Success,
            "Synonym '" + synonymName + "' created successfully for table '" + option + "'!");
    showMenuLoop(table_options);
}

void MainMenu::listTables()
{
    std::cout << std::endl;
    app->listTables();
    showMenuLoop(table_options);
}

void MainMenu::insertInto()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name to insert into (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";

    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option) == true)
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to insert into (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }

    const std::string& tableName = option;
    Table* table = tableCollection->getTable(tableName);
    const std::string* columns = table->getColumns();
    const unsigned int* maxColumnLengths = table->getMaxColumnLengths();
    const int noOfColumns = table->getNoOfColumns();
    int maxColumnLength = 0;
    for (int i = 0; i < noOfColumns; i++)
    {
        if (columns[i].size() > maxColumnLength)
        {
            maxColumnLength = columns[i].size();
        }
    }
    auto* newValues = new std::string[noOfColumns];
    std::string currValue;
    const std::string* columnTypes = table->getColumnTypes();
    int currIndex = 0;
    while (true)
    {
        if (currIndex == noOfColumns)
        {
            break;
        }
        for (int i = 0; i < maxColumnLength - columns[currIndex].size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << columns[currIndex] << " : ";
        std::cin >> currValue;
        if (table->isColumnType(currValue, columnTypes[currIndex]) &&
                currValue.length() <= maxColumnLengths[currIndex])
        {
            newValues[currIndex] = currValue;
            currIndex++;
            continue;
        }

        if (!table->isColumnType(currValue, columnTypes[currIndex]))
        {
            for (int i = 0; i < maxColumnLength + 3; i++)
            {
                std::cout << ' ';
            }
            statusManager->print(StatusManager::Error,
                    "Value does not match column type '" + columnTypes[currIndex] + "'!");
        }
        else
        {
            for (int i = 0; i < maxColumnLength + 3; i++)
            {
                std::cout << ' ';
            }
            statusManager->print(StatusManager::Error,
                    "Value exceeds max length! (Length: " + std::to_string(currValue.length()) +
                    ", max: " + std::to_string(maxColumnLengths[currIndex]) + "!");
        }
    }

    if (table->addRow(newValues, noOfColumns) == 0)
    {
        app->writeTableToFile(*table);
        int noOfRows = table->getNoOfRows();
        statusManager->print(StatusManager::Success,
                "Inserted successfully! (Total number of rows: " + std::to_string(noOfRows) + ')');

        delete[] columns;
        delete[] columnTypes;
        delete[] newValues;
        delete table;
        showMenuLoop(table_options);
    }
}

void MainMenu::showTableOptions()
{
    std::cout << std::endl;
    std::cout << "What would you like to do?" << std::endl << std::endl << std::endl;
    std::cout << "(1) Create a table." << std::endl << std::endl;
    std::cout << "(2) Create a synonym." << std::endl << std::endl;
    std::cout << "(3) List all tables." << std::endl << std::endl;
    std::cout << "(4) Display a table." << std::endl << std::endl;
    std::cout << "(5) Select from a table." << std::endl << std::endl;
    std::cout << "(6) Insert into a table." << std::endl << std::endl;
    std::cout << "(7) Update a table." << std::endl << std::endl;
    std::cout << "(8) Delete from a table." << std::endl << std::endl;
    std::cout << "(9) Drop a table." << std::endl << std::endl;
    std::cout << "(r) Return to the previous menu." << std::endl;
    std::cout << "(q) Quit program." << std::endl << std::endl;
    std::cout << BOLD << "Choice: [1-9rq] " << RESET;

    while (true)
    {
        switch (_getch())
        {
        case '1':
            clearScreen();
            createTable();
            showTableOptions();
            break;
        case '2':
            clearScreen();
            createSynonym();
            showTableOptions();
            break;
        case '3':
            clearScreen();
            listTables();
            showTableOptions();
            break;
        case '4':
            clearScreen();
            displayTable();
            showTableOptions();
            break;
        case '5':
            clearScreen();
            selectFrom();
            showTableOptions();
            break;
        case '6':
            clearScreen();
            insertInto();
            showTableOptions();
            break;
        case '7':
            clearScreen();
            updateTable();
            showTableOptions();
            break;
        case '8':
            clearScreen();
            deleteFrom();
            showTableOptions();
            break;
        case '9':
            clearScreen();
            dropTable();
            showTableOptions();
            break;
        case 'r':
            clearScreen();
            runInteractiveMenu();
            showTableOptions();
            break;
        case 'q':
            clearScreen();
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::createTable()
{
    std::cout << std::endl;
    std::string option;
    std::cout << "Enter table name to create (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (!tableCollection->tableExists(option))
        {
            break;
        }

        statusManager->print(StatusManager::Error, "Table '" + option + "' already exists!");
        std::cout << std::endl;
        std::cout << "Enter table name to create (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }

    std::cout << "Enter number of columns for the table:" << std::endl;
    std::cout << "> ";

    int noOfColumns;
    while (true)
    {
        if (!(std::cin >> noOfColumns))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            statusManager->print(StatusManager::Error, "You have to enter an integer!");
            std::cout << std::endl;
            std::cout << "Enter number of columns for the table:" << std::endl;
            std::cout << "> ";
        }
        else
        {
            if (noOfColumns < 1)
            {
                statusManager->print(StatusManager::Error, "Number of columns has to be at least 1!");
                std::cout << std::endl;
                std::cout << "Enter number of columns for the table:" << std::endl;
                std::cout << "> ";
            }
            else
            {
                break;
            }
        }
    }

    std::cout << "Enter attribute details separated by spaces (Name, Type, Length, Default): " << std::endl;
    std::cout << "[E.g.: id INTEGER 30 0]" << std::endl;
    const auto columnNames = new std::string[noOfColumns];
    const auto columnTypes = new std::string[noOfColumns];
    const auto maxColumnLengths = new unsigned int[noOfColumns];

    std::string columnName, columnType, defaultValue;
    int maxColumnLength;
    for (int i = 0; i < noOfColumns; i++)
    {
        std::cout << "> ";
        std::cin >> columnName >> columnType >> maxColumnLength >> defaultValue;
        if (columnType != "text" && columnType != "varchar" && columnType != "float" && columnType != "int" &&
                columnType != "integer" && columnType != "numeric")
        {
            statusManager->print(StatusManager::Error,
                    "You need to enter a valid column type! (Options are: text, varchar, int, "
                    "integer, numeric, float)");
            i--;
            continue;
        }

        if (columnName == "text" && columnName == "varchar" && columnName == "float" && columnName == "int" &&
                columnName == "integer" && columnName == "numeric")
        {
            statusManager->print(StatusManager::Error,
                    "Keyword '" + columnName + "' can't be used for the column name!");
            std::cout << std::endl;
            i--;
            continue;
        }

        if (maxColumnLength < 1)
        {
            statusManager->print(StatusManager::Error, "Length of attribute has to be at least 1!");
            std::cout << std::endl;
            i--;
            continue;
        }

        bool hasError = false;
        for (int j = 0; j < i; j++)
        {
            if (columnNames[j] == columnName)
            {
                statusManager->print(StatusManager::Error,
                        "You can't enter two identical column names! Try again.");
                hasError = true;
                break;
            }
        }

        // check here so we can continue with the outer for not the one above
        if (hasError == true)
        {
            i--;
            continue;
        }

        columnNames[i] = columnName;
        columnTypes[i] = columnType;
        maxColumnLengths[i] = maxColumnLength;
    }

    auto table = new Table(noOfColumns, option);
    table->setColumnNames(columnNames, noOfColumns);
    table->setColumnTypes(columnTypes, noOfColumns);
    table->setMaxColumnLengths(maxColumnLengths, noOfColumns);
    tableCollection->addTable(*table);
    app->writeTableToFile(*table);
    statusManager->print(StatusManager::Success, "Table '" + option + "' created successfully!");

    delete table;
    delete[] maxColumnLengths;
    delete[] columnTypes;
    delete[] columnNames;
    showMenuLoop(table_options);
}

void MainMenu::displayTable()
{
    std::cout << std::endl;
    printAvailableTables();
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        return;
    }

    std::cout << "Enter table name to display (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";
    std::string option;
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
            clearScreen();
            return;
        }
        if (option == "quit")
        {
            exit(0);
        }

        if (tableCollection->tableExists(option))
        {
            break;
        }
        statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
        std::cout << std::endl;
        std::cout << "Enter table name to display (or 'return' / 'quit'):" << std::endl;
        std::cout << "> ";
    }
    const std::string& synonymName = option;
    tableCollection->getTable(option)->printTable(std::cout, synonymName);
    app->writeSelectToFile(*tableCollection->getTable(synonymName), synonymName);
    showMenuLoop(table_options);
}

void MainMenu::printAvailableTables()
{
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        std::cout << "There are no available tables! Create a table first." << std::endl;
        showMenuLoop(table_options);
        return;
    }
    const std::string msg = "Available tables: ";
    std::cout << msg;
    const Table* tables = tableCollection->getTables();
    int totalMessageLength = msg.length();
    for (int i = 0; i < noOfTables; i++)
    {
        const std::string tableName = tables[i].getTableName();
        std::cout << tableName;
        totalMessageLength += tableName.length();
        const int noOfSynonyms = tables[i].getNoOfSynonyms();
        if (noOfSynonyms > 0)
        {
            totalMessageLength += 2; // both parentheses
            std::cout << "[";

            const std::string* synonyms = tables[i].getSynonyms();
            for (int j = 0; j < noOfSynonyms; j++)
            {
                const std::string& synonym = synonyms[j];
                std::cout << synonym;
                totalMessageLength += synonym.length();
                if (j < noOfSynonyms - 1)
                {
                    std::cout << ", ";
                    totalMessageLength += 2;
                }
            }
            std::cout << "]";
        }
        if (i < noOfTables - 1)
        {
            std::cout << ", ";
            totalMessageLength += 2;
        }
    }

    std::cout << std::endl;
    for (int i = 0; i < totalMessageLength; i++)
    {
        std::cout << '-';
    }
    std::cout << std::endl << std::endl;
    delete[] tables;
}

void MainMenu::printAvailableColumnsOfTable(const std::string& tableName)
{
    if (tableCollection->tableExists(tableName) == false)
    {
        throw std::runtime_error("Wrong table!");
    }

    const auto table = tableCollection->getTable(tableName);
    const auto columns = table->getColumns();
    const int noOfColumns = table->getNoOfColumns();

    std::cout << std::endl;
    for (int i = 0; i < STATUS_OFFSET; i++)
    {
        std::cout << ' ';
    }
    std::cout << "[Selected table: '" << tableName << "']";
    std::cout << std::endl;
    for (int i = 0; i < STATUS_OFFSET; i++)
    {
        std::cout << ' ';
    }
    std::cout << "[Available columns: ";
    for (int i = 0; i < noOfColumns; i++)
    {
        std::cout << " " << columns[i] << " ";
        if (i < noOfColumns - 1)
        {
            std::cout << "| ";
        }
    }
    std::cout << ']' << std::endl << std::endl;

    delete[] columns;
}

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
#define STATUS_OFFSET 5

// output bold text on the console
#define BOLD "\033[1m"

// reset the bold text 
#define RESET "\033[0m"


MainMenu::MainMenu() { app = new App(); }

void MainMenu::displayWelcomeMenu()
{
    while (true)
    {
        clearScreen();
        std::cout << std::endl;
        std::cout << "sql-engine";
        std::cout << "──────────" << std::endl;

        if (firstEnter == true)
        {
            AppInitializer* initializer = new AppInitializer;
            initializer->run();
            delete initializer;

            firstEnter = false;
        }

        std::cout << std::endl;
        std::cout << "Choose an option:" << std::endl<<std::endl;
        std::cout << "  1  Interactive menu"<< std::endl;
        std::cout << "  2  Run SQL queries" << std::endl<<std::endl;
        std::cout << "  h  Help" << std::endl;
        std::cout << "  q  Quit" << std::endl << std::endl;
        std::cout << BOLD << "Choice: [12hq] " << RESET;

        switch (_getch())
        {
        case '1':
            runInteractiveMenu();
            break;
        case '2':
            app->parseCommands();
            break;
        case 'h':
            helpMenu->displayHelp();
            break;
        case 'q':
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::runInteractiveMenu()
{
    bool keepRunning = true;
    while (keepRunning)
    {
        clearScreen();
        std::cout << std::endl;
        std::cout << "Choose object type:" << std::endl
            << std::endl;
        std::cout << "  1  Table" << std::endl; 
        std::cout << "  2  Index" << std::endl << std::endl;
        std::cout << "  r  Return to previous menu" << std::endl;
        std::cout << "  q  Quit" << std::endl << std::endl;
        std::cout << BOLD << "Choice: [12rq] " << RESET;

        switch (_getch())
        {
        case '1':
            showTableOptions();
            break;
        case '2':
            showIndexOptions();
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
    statusManager->print(StatusManager::Success, "Index '" + indexName + "' created.");

    delete index;
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

        statusManager->print(StatusManager::Success, "Index '" + option + "' dropped.");

    }
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
    std::cout << "Choose option:" << std::endl<<std::endl;
    std::cout << "  (r) Return to previous menu" << std::endl;
    std::cout << "  (q) Quit program" << std::endl<<std::endl;
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
            exit(0);
        default:
            break;
        }
    }
}

void MainMenu::showIndexOptions()
{
    clearScreen();
    std::cout << std::endl;
    std::cout << "Choose action:" << std::endl << std::endl;
    std::cout << "  (1) Create index" << std::endl << std::endl;
    std::cout << "  (2) Display indexes of a table" << std::endl << std::endl;
    std::cout << "  (3) Drop index" << std::endl << std::endl;
    std::cout << "  (r) Return to previous menu" << std::endl;
    std::cout << "  (q) Quit program" << std::endl << std::endl;
    std::cout << BOLD << "Choice: [123rq] " << RESET;

    bool keepRunning = true;
    while (true)
    {
        switch (_getch())
        {
        case '1':
            createIndex();
            break;
        case '2':
            displayIndexes();
            break;
        case '3':
            dropIndex();
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
    std::cout << "y  Yes" << std::endl;
    std::cout << "n  No" << std::endl;
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
    int updatedRowsCount = 0;
    for (int i = 0; i < table->getNoOfRows(); i++)
    {
        if (tableRows[i][whereIndex] == whereValue)
        {
            tableRows[i][setIndex] = setValue;
            updatedRowsCount++;
        }
    }
    table->setRows(tableRows, table->getNoOfRows(), table->getNoOfColumns());
    app->writeTableToFile(*table);
    statusManager->print(StatusManager::Success, "Updated " + std::to_string(updatedRowsCount) + " rows.");

    for (int i = 0; i < noOfRows; i++)
    {
        delete[] tableRows[i];
    }
    delete[] tableRows;
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
        statusManager->print(StatusManager::Success, "Row deleted.");
        app->writeTableToFile(*table);
        showMenuLoop(table_options);
    }
    else
    {
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
        statusManager->print(StatusManager::Success, "Table '" + option + "' dropped.");
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
            "Synonym '" + synonymName + "' created for table '" + option + "'.");
    showMenuLoop(table_options);
}

void MainMenu::listTables()
{
    clearScreen();
    std::cout << std::endl;
    if(tableCollection->getNoOfTables() == 0){
        statusManager->print(StatusManager::Info, "No tables available. Create a table first.");
    }
    else {
        app->listTables();
    }
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
                "Row inserted. (Total number of rows: " + std::to_string(noOfRows) + ')');

        delete[] columns;
        delete[] columnTypes;
        delete[] newValues;
        showMenuLoop(table_options);
    }
}

void MainMenu::showTableOptions()
{
    bool keepRunning = true;
    while (keepRunning)
    {
        clearScreen();
        std::cout << std::endl;
        printTableMenu();

        switch (_getch())
        {
        case '1':
            createTable();
            break;
        case '2':
            createSynonym();
            break;
        case '3':
            listTables();
            break;
        case '4':
            displayTable();
            break;
        case '5':
            selectFrom();
            break;
        case '6':
            insertInto();
            break;
        case '7':
            updateTable();
            break;
        case '8':
            deleteFrom();
            break;
        case '9':
            dropTable();
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

void MainMenu::createTable()
{
    clearScreen();
    std::cout << std::endl;
    std::string option;
    std::cout << "Enter table name to create (or 'return' / 'quit'):" << std::endl;
    std::cout << "> ";
    while (true)
    {
        std::cin >> option;
        if (option == "return")
        {
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
    statusManager->print(StatusManager::Success, "Table '" + option + "' created.");

    delete[] maxColumnLengths;
    delete[] columnTypes;
    delete[] columnNames;
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
}

void MainMenu::printAvailableTables()
{
    const int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        statusManager->print(StatusManager::Info, "No tables available. Create a table first.");
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
    for (int i = 0; i < noOfColumns; i++){
        std::cout << " " << columns[i] << " ";
        if (i < noOfColumns - 1){
            std::cout << "| ";
        }
    }
    std::cout << ']' << std::endl << std::endl;

    delete[] columns;
}

void MainMenu::printTableMenu(){
    std::cout << "Choose action:" << std::endl << std::endl;
    if(tableCollection->getNoOfTables() == 0) {
        std::cout << BOLD << "  (1) Create table " <<RESET << "(recommended)"<< std::endl;
    }
    else {
        std::cout << "  1  Create table" << std::endl;
    }
    std::cout << "  2  Create synonym" << std::endl;
    std::cout << "  3  List tables" << std::endl;
    std::cout << "  4  Display table" << std::endl << std::endl;
    std::cout << "  5  Select from table" << std::endl;
    std::cout << "  6  Insert into table" << std::endl;
    std::cout << "  7  Update table" << std::endl;
    std::cout << "  8  Delete from table" << std::endl << std::endl;
    std::cout << "  9  Drop table" << std::endl << std::endl;
    std::cout << "  r  Return to previous menu" << std::endl;
    std::cout << "  q  Quit" << std::endl << std::endl;
    std::cout << BOLD << "Choice: [1-9rq] " << RESET;
}

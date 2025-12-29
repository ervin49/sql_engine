#pragma once
#include "globals.h"
#include <iostream>

#include "StatusManager.h"

class Menu
{
private:
    Application* application;

public:
    Menu()
    {
        application = new Application();
    }


    void display_welcome_menu()
    {
        clear_screen();
        std::cout << std::endl;
        std::cout << "Welcome to sql engine! Please choose what you would like to do." << std::endl;
        std::cout << "(1) Run commands using the interactive menu." << std::endl;
        std::cout << "(2) Run commands using plain old sql queries." << std::endl;
        std::cout << "(h) Display help menu." << std::endl;
        std::cout << "(q) Quit program." << std::endl;
        std::cout << "Please choose an option: [12hq] ";

        char c;

        while (true)
        {
            std::cin >> c;
            c = tolower(c);
            switch (c)
            {
            case('1'):
                clear_screen();
                run_interactive_menu();
                break;
            case('2'):
                clear_screen();
                application->parse_commands();
                break;
            case('h'):
                clear_screen();
                display_help();
                break;
            case('q'):
                return;
            default:
                statusManager->print(StatusManager::Error, "You need to enter a valid option!");
                std::cout << "Please choose an option: [12hq] ";
            }
        }
    }

    void run_interactive_menu()
    {
        std::cout << std::endl;
        std::cout << "Welcome to the sql engine interactive menu! What would you like to work with?" << std::endl;
        std::cout << "(t) A table." << std::endl;
        std::cout << "(i) An index." << std::endl;
        std::cout << "(r) Return to previous menu." << std::endl;
        std::cout << "(q) Quit program." << std::endl;
        std::cout << "Please choose an option: [tirq] ";

        char c;

        while (true)
        {
            std::cin >> c;
            c = tolower(c);
            switch (c)
            {
            case 't':
                clear_screen();
                table_options();
                break;
            case 'i':
                clear_screen();
                index();
                break;
            case 'r':
                clear_screen();
                display_welcome_menu();
                break;
            case 'q':
                return;
            default:
                statusManager->print(StatusManager::Error, "You need to enter a valid option!");
                std::cout << "Please choose an option: [tirq] ";
            }
        }
    }

    void index()
    {
    }

    void select_from()
    {
    }

    void update_table()
    {
    }

    void delete_from()
    {
    }

    void drop_table()
    {
    }

    void table_options()
    {
        std::cout << std::endl;
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "(1) Create a table." << std::endl;
        std::cout << "(2) Display a table." << std::endl;
        std::cout << "(3) Select from a table." << std::endl;
        std::cout << "(4) Update a table." << std::endl;
        std::cout << "(5) Delete from a table." << std::endl;
        std::cout << "(6) Drop a table." << std::endl;
        std::cout << "(r) Return to previous menu." << std::endl;
        std::cout << "(q) Quit program." << std::endl;
        std::cout << "Please choose an option: [1-6rq] ";

        char c;
        std::cin >> c;
        c = tolower(c);
        switch (c)
        {
        case '1':
            create_table();
            break;
        case '2':
            display_table();
            break;
        case '3':
            select_from();
            break;
        case '4':
            update_table();
            break;
        case '5':
            delete_from();
            break;
        case '6':
            drop_table();
            break;
        case 'r':
            clear_screen();
            run_interactive_menu();
            break;
        case 'q':
            return;
        default:
            statusManager->print(StatusManager::Error, "You need to enter a valid option!");
        }
    }

    void create_table()
    {
        std::string tableName;
        int noOfColumns;
        std::cout << "Table name? ";
        std::cin >> tableName;
        std::cout << "Number of columns? ";
        std::cin >> noOfColumns;
        std::cout <<
            "Please enter attribute details separated by spaces(Name, Type, Length, Default): "
            << std::endl;
        std::cout << "[E.g.: id INTEGER 30 0]" << std::endl;
        const auto columnNames = new std::string[noOfColumns];
        const auto columnTypes = new std::string[noOfColumns];
        auto maxColumnLengths = new unsigned int[noOfColumns];

        std::string columnName, columnType, defaultValue;
        int maxColumnLength;
        for (int i = 0; i < noOfColumns; i++)
        {
            std::cin >> columnName >> columnType >> maxColumnLength >> defaultValue;
            if (columnType != "text" && columnType != "varchar" && columnType != "float" && columnType != "int" &&
                columnType != "integer" && columnType != "numeric")
            {
                statusManager->print(StatusManager::Error,
                                     "You need to enter a valid column type! (Options are: text, varchar, int, integer, numeric, float)");
                i--;
                continue;
            }

            bool hasIdenticalNames = false;
            for (int j = 0; j < i; j++)
            {
                if (columnNames[i] == columnNames[j])
                {
                    statusManager->print(StatusManager::Error,
                                         "You can't enter two identical column names! Try again.");
                    i--;
                    hasIdenticalNames = true;
                }
            }
            if (hasIdenticalNames == true)
            {
                continue;
            }

            columnNames[i] = columnName;
            columnTypes[i] = columnType;
            maxColumnLengths[i] = maxColumnLength;
        }

        auto table = new Table(noOfColumns, tableName);
        table->setColumnNames(columnNames, noOfColumns);
        table->setColumnTypes(columnTypes, noOfColumns);
        table->setMaxColumnLengths(maxColumnLengths, noOfColumns);
        tableCatalog->add_table(*table);
        application->write_table_to_file(*table);
        delete table;

        statusManager->print(StatusManager::Success,
                             "Table \"" + tableName +
                             "\" created succesfully! What would you like to do now?");
        std::cout << "(r) Return to the menu." << std::endl;
        std::cout << "(q) Quit the program." << std::endl;
        std::cout << "Please choose an option: [rq] ";

        char c;
        std::cin >> c;
        c = tolower(c);
        switch (c)
        {
        case 'r':
            clear_screen();
            table_options();
            break;
        case 'q':
            return;
        default:
            statusManager->print(StatusManager::Error, "You need to enter a valid option!");
            break;
        }
    }

    void display_help()
    {
        std::cout << std::endl;
        std::cout << "What do you need help with?" << std::endl;
        //de scris
    }

    void clear_screen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void display_table()
    {
        std::string tableName;
        std::cout << "What is the name of the table you want to display? ";
        std::cin >> tableName;
        tableCatalog->getTable(tableName)->print_table(std::cout);
        application->write_select_to_file(*tableCatalog->getTable(tableName));
    }
};

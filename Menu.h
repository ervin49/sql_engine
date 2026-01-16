#pragma once
#include <iostream>
#include <limits>

#include "globals.h"

#include "StatusManager.h"

//indentation for status messages
#define STATUS_OFFSET 5

//this is to output bold text on the console
#define BOLD "\033[1m"

//this is to reset the bold text, otherwise
//all following text would be bold, not just the next word
#define RESET "\033[0m"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

inline int _getch()
{
	struct termios oldattr, newattr;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	int ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}

#endif
class Menu
{
private:
	App* app;
	bool first_enter = true;

	enum menu_options
	{
		table_options,
		index_options
	};

	static void press_enter_to_continue()
	{
		std::cout << std::endl << "Press Enter to return...";
		std::string dummy;
		std::getline(std::cin, dummy);
	}

	static void print_syntax_help()
	{
		while (true)
		{
			clear_screen();
			std::cout << "========= SYNTAX CHEATSHEET =========" << std::endl << std::endl;
			std::cout << "Select a command to view details:" << std::endl << std::endl << std::endl;
			std::cout << "(1) CREATE TABLE" << std::endl << std::endl;
			std::cout << "(2) CREATE SYNONYM" << std::endl << std::endl;
			std::cout << "(3) INSERT INTO" << std::endl << std::endl;
			std::cout << "(4) SELECT" << std::endl << std::endl;
			std::cout << "(5) UPDATE & DELETE" << std::endl << std::endl;
			std::cout << "(6) INDEX Operations" << std::endl << std::endl;
			std::cout << "(7) LIST" << std::endl << std::endl;
			std::cout << "(r) Return to Help Menu" << std::endl;
			std::cout << "(q) Quit program" << std::endl << std::endl;
			std::cout << BOLD << "Choice: [1-6rq] " << RESET;
			char c = _getch();
			c = tolower(c);

			clear_screen();
			switch (c)
			{
			case '1':
				std::cout << "--- CREATE TABLE ---" << std::endl;
				std::cout << "(!) Note: What is denoted between [] is not mandatory." << std::endl << std::endl;
				std::cout << "Format: CREATE TABLE table_name [IF NOT EXISTS] ((col_name, type, size, default), ...)"
					<< std::endl;
				std::cout << "Types:  integer, text, float" << std::endl;
				std::cout << "E.g.:   CREATE TABLE students ((name, text, 20, -), (age, integer, 3, 0))" << std::endl;
				press_enter_to_continue();
				break;
			case '2':
				std::cout << "--- CREATE SYNONYM ---" << std::endl;
				std::cout << "Format: CREATE SYNONYM synonym_name FOR table_name" << std::endl;
				std::cout << "E.g.:   CREATE SYNONYM learners FOR students" << std::endl;
				press_enter_to_continue();
				break;
			case '3':
				std::cout << "--- INSERT INTO ---" << std::endl;
				std::cout << "Format: INSERT INTO table_name VALUES (val1, val2, ...)" << std::endl;
				std::cout << "E.g.:     INSERT INTO students VALUES ('Alex', 21)" << std::endl;
				press_enter_to_continue();
				break;
			case '4':
				std::cout << "--- SELECT ---" << std::endl;
				std::cout << "(!) Note: What is denoted between [] is not mandatory." << std::endl << std::endl;
				std::cout << "Format: SELECT (col1, col2, ...) | ALL FROM table_name [WHERE col = val]" << std::endl;
				std::cout << "E.g.:   SELECT name FROM students WHERE age = 21" << std::endl;
				press_enter_to_continue();
				break;
			case '5':
				std::cout << "--- UPDATE & DELETE ---" << std::endl;
				std::cout << "Update: UPDATE table SET col = new_val WHERE col = target_val" << std::endl;
				std::cout << "Delete: DELETE FROM table WHERE col = target_val" << std::endl;
				press_enter_to_continue();
				break;
			case '6':
				std::cout << "--- INDEX COMMANDS ---" << std::endl;
				std::cout << "Create: CREATE INDEX index_name ON table_name (column_name)" << std::endl;
				std::cout << "Drop:   DROP INDEX index_name" << std::endl;
				press_enter_to_continue();
				break;
			case '7':
				std::cout << "--- LIST COMMAND ---" << std::endl;
				std::cout << "FORMAT: LIST" << std::endl;
				std::cout << "Displays all current available tables with their synonyms and indexes" << std::endl;
				press_enter_to_continue();
				break;
			case 'r':
				return;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	static void print_datatypes_help()
	{
		clear_screen();
		std::cout << "================ SUPPORTED DATA TYPES ================" << std::endl << std::endl;
		std::cout << "1. INTEGER (integer / int)" << std::endl;
		std::cout << "   - Whole numbers (e.g., 10, -5). Used for IDs, Counts." << std::endl << std::endl;

		std::cout << "2. FLOAT (float / numeric)" << std::endl;
		std::cout << "   - Decimal numbers (e.g., 10.5). Used for Prices, Averages." << std::endl << std::endl;

		std::cout << "3. TEXT (text / varchar)" << std::endl;
		std::cout << "   - Strings (e.g., 'Hello'). Used for Names, Descriptions." << std::endl << std::endl;

		std::cout << "----------------------------" << std::endl;
		press_enter_to_continue();
	}

	static void print_general_help()
	{
		clear_screen();
		std::cout << "================ SYSTEM INFORMATION ================" << std::endl << std::endl;

		// Aici e partea "complexa" - afiseaza date reale din aplicatie
		std::cout << "[ LIVE STATISTICS ]" << std::endl;
		std::cout << "Tables loaded:  " << tableCatalog->getNoOfTables() << std::endl;
		std::cout << "Indexes loaded: " << indexCatalog->getNoOfIndexes() << std::endl;
		std::cout << "----------------------------------------------------" << std::endl << std::endl;

		std::cout << "[ STORAGE LOCATIONS ]" << std::endl;
		std::cout << "  - Tables  -> ./tables" << std::endl;
		std::cout << "  - Indexes -> ./index_catalog" << std::endl << std::endl;

		std::cout << "[ TIPS ]" << std::endl;
		std::cout << "  - Use 'q' to Quit, 'r' to Return." << std::endl;
		std::cout << "  - You can import .csv files using the IMPORT command." << std::endl;

		std::cout << "--------------------" << std::endl;
		press_enter_to_continue();
	}

public:
	Menu() { app = new App(); }

	void display_welcome_menu()
	{
		clear_screen();
		if (first_enter == true)
		{
			const first_configuration config;
			config.load_tables();
			first_enter = false;
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
				clear_screen();
				run_interactive_menu();
				break;
			case '2':
				clear_screen();
				app->parse_commands();
				display_welcome_menu();
				break;
			case 'h':
				clear_screen();
				display_help();
				break;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	void run_interactive_menu()
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
				clear_screen();
				show_table_options();
				break;
			case '2':
				clear_screen();
				show_index_options();
				break;
			case 'r':
				clear_screen();
				display_welcome_menu();
				break;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	void create_index()
	{
		std::cout << std::endl;
		std::string option, columnName;
		print_available_tables();
		std::cout << "Enter table name (or 'return' / 'quit'):" << std::endl;
		std::cout << "> ";

		while (true)
		{
			std::cin >> option;
			if (option == "return")
			{
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option))
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name (or 'return' / 'quit')" << std::endl;
			std::cout << "> ";
		}
		const std::string tableName = tableCatalog->getActualTableName(option);
		Table* table = tableCatalog->getTable(tableName);

		print_available_columns_of_table(tableName);
		std::cout << "Enter column to index:" << std::endl;
		std::cout << "> ";

		while (true)
		{
			std::cin >> columnName;
			if (table->column_exists(columnName) == true && indexCatalog->has_index(tableName, columnName) == false)
			{
				break;
			}
			if (table->column_exists(columnName) == false)
			{
				statusManager->print(StatusManager::Error,
				                     "Table '" + option + "' does not have column '" + columnName + "'!");
				std::cout << std::endl;
				std::cout << "Enter column to index:" << std::endl;
				std::cout << "> ";
			}
			else if (indexCatalog->has_index(tableName, columnName) == true)
			{
				statusManager->print(StatusManager::Error,
				                     "Table '" + option + "' already has an index on column '" + columnName +
				                     "'!");
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
			if (indexCatalog->index_exists(indexName) == false)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Index '" + indexName + "' already exists!");
			std::cout << std::endl;
			std::cout << "Enter index name:" << std::endl;
			std::cout << "> ";
		}
		auto index = new Index(indexName, tableName, columnName);
		indexCatalog->add_index(*index);
		table->add_index(indexName);
		app->write_table_to_file(*table);
		indexCatalog->write_index_catalog_to_file();
		statusManager->print(StatusManager::Success, "Index '" + indexName + "' created successfully!");

		delete index;
		delete table;
		show_menu_loop(index_options);
	}

	void drop_index()
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			int position = indexCatalog->return_position_of_index(option);
			if (position != -1)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Index '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter index name to drop (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		if (indexCatalog->drop_index(option) == 0)
		{
			const std::string tableNameOfIndex = indexCatalog->getIndex(option)->getTableName();
			const auto table = tableCatalog->getTable(tableNameOfIndex);
			table->remove_index(option);
			indexCatalog->write_index_catalog_to_file();

			statusManager->print(StatusManager::Success, "Index '" + option + "' dropped successfully!");

			delete table;
			show_menu_loop(index_options);
		}
		else
		{
			show_menu_loop(index_options);
		}
	}

	void display_indexes()
	{
		std::cout << std::endl;
		const int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables == 0)
		{
			std::cout << "You have to create a table first!" << std::endl << std::endl;
			return;
		}

		print_available_tables();
		std::cout << "Enter table name to display indexes (or 'return' / 'quit'):" << std::endl;
		std::cout << "> ";

		std::string option;
		while (true)
		{
			std::cin >> option;
			if (option == "return")
			{
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to display indexes (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		const std::string tableName = tableCatalog->getActualTableName(option);
		if (indexCatalog->getNoOfIndexesOfTableByName(tableName) == 0)
		{
			std::cout << "Table '" << option << "' does not have any index!" << std::endl;
			show_menu_loop(index_options);
		}

		indexCatalog->print_indexes_of_a_table_by_name(tableName);
		show_menu_loop(index_options);
	}

	void show_menu_loop(menu_options option)
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
				clear_screen();
				switch (option)
				{
				case index_options:
					show_index_options();
					break;
				case table_options:
					show_table_options();
					break;
				default:
					break;
				}
				break;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	void show_index_options()
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
				clear_screen();
				create_index();
				show_index_options();
				break;
			case '2':
				clear_screen();
				display_indexes();
				show_index_options();
				break;
			case '3':
				clear_screen();
				drop_index();
				show_index_options();
				break;
			case 'r':
				clear_screen();
				run_interactive_menu();
				break;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	void select_from()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to select from:" << std::endl;
			std::cout << "> ";
		}
		const std::string& synonymName = option;
		const auto originalTable = tableCatalog->getTable(synonymName);

		print_available_columns_of_table(synonymName);
		std::cout << "Enter columns to select, separated by spaces, or 'ALL' to select all "
			"columns: " << std::endl;
		std::cout << "[E.g.: col1 col2 col3 ...]" << std::endl;
		std::cout << "> ";

		auto selectedColumns = new std::string[50];
		std::string columnName;
		int noOfSelectedColumns;
		bool checkForAll = false;
		while (true)
		{
			delete[] selectedColumns;
			selectedColumns = new std::string[50]; //just hope 50 is enough
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

			bool has_error = false;
			for (int i = 0; i < noOfSelectedColumns; i++)
			{
				if (originalTable->column_exists(selectedColumns[i]) == false)
				{
					std::cout << "Column '" << selectedColumns[i]
						<< "' does not exist in table '" + option + "'!" << std::endl;
					has_error = true;
				}
			}
			if (has_error == false)
			{
				break;
			}
			std::cout << "Enter columns to select, separated by spaces, or 'ALL' to select all "
				"columns: " << std::endl;
			std::cout << "> ";
		}

		std::cout << std::endl;
		std::cout << "Search based on the value of a column?" << std::endl;
		std::cout << "(y) Yes" << std::endl;
		std::cout << "(n) No" << std::endl;
		std::cout << BOLD << "Choice: [yn] " << RESET;

		bool search_with_column_name;
		std::string columnNameSearchedFor, value;
		while (true)
		{
			switch (_getch())
			{
			case 'y':
				std::cout << std::endl << std::endl;
				std::cout << "Enter column name and value to search for, separated by spaces: "
					<< std::endl;
				std::cout << "[E.g.: first_name Andrei]" << std::endl;
				std::cout << "> ";
				while (true)
				{
					std::cin >> columnNameSearchedFor >> value;
					if (originalTable->column_exists(columnNameSearchedFor) == true)
					{
						break;
					}
					statusManager->print(StatusManager::Error,
					                     "Column '" + columnNameSearchedFor + "' does not exist!");
					std::cout << std::endl;
					std::cout
						<< "Enter column value to search for, separated by spaces:" << std::endl;
					std::cout << "> ";
				}
				search_with_column_name = true;
				break;
			case 'n':
				search_with_column_name = false;
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

			if (search_with_column_name == true)
			{
				const auto tableWithSelectedRows = new Table(noOfSelectedColumns, "");
				const int index = tableWithSelectedRows->return_index_of_column_by_name(columnName);
				bool found = false;
				for (int i = 0; i < noOfRows; i++)
				{
					for (int j = 0; j < noOfSelectedColumns; j++)
					{
						if (rowsOfNewTable[i][j] == value && j == index)
						{
							found = true;
							tableWithSelectedRows->add_row(rowsOfNewTable[i]);
						}
					}
				}
				if (!found)
				{
					statusManager->print(StatusManager::Error,
					                     "No matching values for '" + value + "' in column '" + columnName +
					                     "'!");
					show_menu_loop(table_options);
				}
				app->write_select_to_file(*tableWithSelectedColumnsOnly, synonymName);
				std::cout << std::endl;
				tableWithSelectedColumnsOnly->print_table(std::cout, synonymName);
				std::cout << std::endl;

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
			else
			{
				app->write_select_to_file(*tableWithSelectedColumnsOnly, synonymName);
				std::cout << std::endl;
				tableWithSelectedColumnsOnly->print_table(std::cout, synonymName);
				std::cout << std::endl;

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
			if (search_with_column_name == true)
			{
				bool found = false;
				int index = originalTable->return_index_of_column_by_name(columnNameSearchedFor);
				const int noOfRows = originalTable->getNoOfRows();
				const int noOfColumns = originalTable->getNoOfColumns();
				std::string** rowsOfOriginalTable = originalTable->getRows();
				auto* tableWithSelectedRows = new Table(noOfColumns, "");

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
							tableWithSelectedRows->add_row(rowsOfOriginalTable[i]);
						}
					}
				}

				if (!found)
				{
					statusManager->print(StatusManager::Error,
					                     "No matching values for: '" + value + "' in column: '" +
					                     columnNameSearchedFor + "'!");
					std::cout << std::endl;
				}
				else
				{
					tableWithSelectedRows->print_table(std::cout, synonymName);
					app->write_select_to_file(*tableWithSelectedRows, synonymName);
					delete selectedColumns;
					for (int i = 0; i < noOfRows; i++)
					{
						delete rowsOfOriginalTable[i];
					}
					delete tableWithSelectedRows;
					delete[] rowsOfOriginalTable;
				}
			}
			else
			{
				tableCatalog->getTable(option)->print_table(std::cout, synonymName);
				app->write_select_to_file(*tableCatalog->getTable(synonymName), synonymName);
			}
		}
		show_menu_loop(table_options);
	}

	void update_table()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to update (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		const std::string tableName = tableCatalog->getActualTableName(option);
		print_available_columns_of_table(tableName);
		std::cout << "Enter column to change and value to search for, separated by spaces: " << std::endl;
		std::cout << "[E.g.: id 101]" << std::endl;
		std::cout << "> ";
		std::string whereColumn, whereValue;
		const auto table = tableCatalog->getTable(option);
		while (true)
		{
			std::cin >> whereColumn >> whereValue;
			if (table->column_exists(whereColumn) == true)
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
			if (table->column_exists(setColumn) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error,
			                     "Column '" + setColumn + "' does not exist in table '" + option + "'!");
			std::cout << std::endl;
			std::cout << "Enter column name to update and the new value, separated by spaces:" << std::endl;
			std::cout << "> ";
		}
		const int setIndex = table->return_index_of_column_by_name(setColumn);
		const int whereIndex = table->return_index_of_column_by_name(setValue);
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
		app->write_table_to_file(*table);
		statusManager->print(StatusManager::Success, "Updated table successfully!");

		for (int i = 0; i < noOfRows; i++)
		{
			delete[] tableRows[i];
		}
		delete[] tableRows;
		delete table;
		show_menu_loop(table_options);
	}

	void delete_from()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to delete from (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		print_available_columns_of_table(option);
		std::cout << "Enter column of the value and the value to delete, separated by space:" << std::endl;
		std::cout << "[E.g.: id 101]" << std::endl;
		std::cout << "> ";
		std::string whereColumn, whereValue;
		const auto table = tableCatalog->getTable(option);
		while (true)
		{
			std::cin >> whereColumn >> whereValue;
			if (table->column_exists(whereColumn) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error,
			                     "Column '" + whereColumn + "' does not exist in table '" + option + "'!");

			std::cout << std::endl;
			std::cout << "Enter column of the value and the value to delete, separated by space:" << std::endl;
			std::cout << "> ";
		}
		if (table->delete_from(whereColumn, whereValue) == 0)
		{
			statusManager->print(StatusManager::Success, "Deleted successfully!");
			app->write_table_to_file(*table);
			delete table;
			show_menu_loop(table_options);
		}
		else
		{
			delete table;
			show_menu_loop(table_options);
		}
	}

	void drop_table()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to drop (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		const std::string actualTableName = tableCatalog->getActualTableName(option);
		if (remove(("./tables/" + actualTableName + ".bin").data()) != 0)
		{
			statusManager->print(StatusManager::Error, "Could not remove file '" + actualTableName + ".bin'!");
			show_menu_loop(table_options);
		}
		if (tableCatalog->drop_table(option) == 0)
		{
			statusManager->print(StatusManager::Success, "Table '" + option + "' dropped successfully!");
			show_menu_loop(table_options);
		}
		else
		{
			show_menu_loop(table_options);
		}
	}

	void create_synonym()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables == 0)
		{
			return;
		}

		std::cout <<
			"Enter table name for the synonym (or 'return' / 'quit'):" << std::endl;
		std::cout << "> ";

		std::string option;
		while (true)
		{
			std::cin >> option;
			if (option == "return")
			{
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout <<
				"Enter table name for the synonym (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}

		std::cout << std::endl;
		std::cout << "Enter synonym name to create:" << std::endl;
		std::cout << "> ";
		std::string synonymName;
		while (true)
		{
			std::cin >> synonymName;
			if (tableCatalog->table_exists(synonymName) == false)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + synonymName + "' already exists!");
			std::cout << std::endl;
			std::cout << "Enter synonym name to create:" << std::endl;
			std::cout << "> ";
		}

		Table* table = tableCatalog->getTable(option);
		table->add_synonym(synonymName);
		app->write_table_to_file(*table);

		statusManager->print(StatusManager::Success,
		                     "Synonym '" + synonymName + "' created successfully for table '" + option + "'!");
		show_menu_loop(table_options);
	}

	void list_tables()
	{
		std::cout << std::endl;
		app->list_tables();
		show_menu_loop(table_options);
	}

	void insert_into()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables == 0)
		{
			return;
		}

		std::cout <<
			"Enter table name to insert into (or 'return' / 'quit'):" << std::endl;
		std::cout << "> ";

		std::string option;
		while (true)
		{
			std::cin >> option;
			if (option == "return")
			{
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout <<
				"Enter table name to insert into (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}

		const std::string& tableName = option;
		Table* table = tableCatalog->getTable(tableName);
		const std::string* columns = table->getColumns();
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
			if (table->is_column_type(currValue, columnTypes[currIndex]))
			{
				newValues[currIndex] = currValue;
				currIndex++;
				continue;
			}

			for (int i = 0; i < maxColumnLength + 3; i++)
			{
				std::cout << ' ';
			}
			statusManager->print(StatusManager::Error,
			                     "Value does not match column type '" + columnTypes[currIndex] + "'!");
		}

		if (table->add_row(newValues) == 0)
		{
			app->write_table_to_file(*table);
			int noOfRows = table->getNoOfRows();
			statusManager->print(StatusManager::Success,
			                     "Inserted successfully! (Total number of rows: " + std::to_string(noOfRows) + ')');

			delete[] columns;
			delete[] columnTypes;
			delete[] newValues;
			delete table;
			show_menu_loop(table_options);
		}
	}

	void show_table_options()
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
				clear_screen();
				create_table();
				show_table_options();
				break;
			case '2':
				clear_screen();
				create_synonym();
				show_table_options();
				break;
			case '3':
				clear_screen();
				list_tables();
				show_table_options();
				break;
			case '4':
				clear_screen();
				display_table();
				show_table_options();
				break;
			case '5':
				clear_screen();
				select_from();
				show_table_options();
				break;
			case '6':
				clear_screen();
				insert_into();
				show_table_options();
				break;
			case '7':
				clear_screen();
				update_table();
				show_table_options();
				break;
			case '8':
				clear_screen();
				delete_from();
				show_table_options();
				break;
			case '9':
				clear_screen();
				drop_table();
				show_table_options();
				break;
			case 'r':
				clear_screen();
				run_interactive_menu();
				show_table_options();
				break;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}

	void create_table()
	{
		std::cout << std::endl;
		std::string option;
		std::cout <<
			"Enter table name to create (or 'return' / 'quit'):" << std::endl;
		std::cout << "> ";
		while (true)
		{
			std::cin >> option;
			if (option == "return")
			{
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option) == false)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' already exists!");
			std::cout << std::endl;
			std::cout <<
				"Enter table name to create (or 'return' / 'quit'):" << std::endl;
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
		tableCatalog->add_table(*table);
		app->write_table_to_file(*table);
		statusManager->print(StatusManager::Success, "Table '" + option + "' created successfully!");

		delete table;
		delete[] maxColumnLengths;
		delete[] columnTypes;
		delete[] columnNames;
		show_menu_loop(table_options);
	}

	void display_help()
	{
		clear_screen();
		std::cout << std::endl;
		std::cout << "Help Center" << std::endl << std::endl << std::endl;
		std::cout << "(1) Syntax guide (commands & examples)" << std::endl << std::endl;
		std::cout << "(2) Data types reference" << std::endl << std::endl;
		std::cout << "(3) General information" << std::endl << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
		std::cout << "(q) Quit program." << std::endl << std::endl;
		std::cout << BOLD << "Choice: [123rq] " << RESET;
		while (true)
		{
			char c = _getch();
			c = tolower(c);
			switch (c)
			{
			case '1':
				clear_screen();
				print_syntax_help();
				display_help();
				break;
			case '2':
				clear_screen();
				print_datatypes_help();
				display_help();
				break;
			case '3':
				clear_screen();
				print_general_help();
				display_help();
				break;
			case 'r':
				display_welcome_menu();
				return;
			case 'q':
				clear_screen();
				exit(0);
			default:
				break;
			}
		}
	}


	static void clear_screen()
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
	}

	void display_table()
	{
		std::cout << std::endl;
		print_available_tables();
		const int noOfTables = tableCatalog->getNoOfTables();
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
				clear_screen();
				return;
			}
			if (option == "quit")
			{
				exit(0);
			}

			if (tableCatalog->table_exists(option))
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table '" + option + "' does not exist!");
			std::cout << std::endl;
			std::cout << "Enter table name to display (or 'return' / 'quit'):" << std::endl;
			std::cout << "> ";
		}
		const std::string& synonymName = option;
		tableCatalog->getTable(option)->print_table(std::cout, synonymName);
		app->write_select_to_file(*tableCatalog->getTable(synonymName), synonymName);
		show_menu_loop(table_options);
	}

	void print_available_tables()
	{
		const int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables == 0)
		{
			std::cout << "There are no available tables! Create a table first." << std::endl;
			show_menu_loop(table_options);
			return;
		}
		const std::string msg = "Available tables: ";
		std::cout << msg;
		const Table* tables = tableCatalog->getTables();
		int totalMessageLength = msg.length();
		for (int i = 0; i < noOfTables; i++)
		{
			const std::string tableName = tables[i].getTableName();
			std::cout << tableName;
			totalMessageLength += tableName.length();
			const int noOfSynonyms = tables[i].getNoOfSynonyms();
			if (noOfSynonyms > 0)
			{
				totalMessageLength += 2; //both parentheses
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

	static void print_available_columns_of_table(const std::string& tableName)
	{
		if (tableCatalog->table_exists(tableName) == false)
		{
			throw std::runtime_error("Wrong table!");
		}

		const auto table = tableCatalog->getTable(tableName);
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

		delete table;
		delete[] columns;
	}
};

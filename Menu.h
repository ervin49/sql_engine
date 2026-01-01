#pragma once
#include <iostream>
#include "globals.h"

#include "StatusManager.h"

class Menu
{
private:
	Application* application;
	bool first_enter = true;
	enum menu_options
	{
		table_options,
		index_options
	};

	void print_syntax_help()
	{
		std::cout << std::endl;
		std::cout << "================ SQL SYNTAX CHEATSHEET ================" << std::endl;
		std::cout << "1. CREATE TABLE:" << std::endl;
		std::cout << "   Format: CREATE TABLE table_name [IF NOT EXISTS] ((col1, type, size, default_value), ...)"
				  << std::endl;
		std::cout << "   E.g.:     CREATE TABLE students ((name, text, 20, -), (age, integer, 3, 0))" << std::endl
				  << std::endl;

		std::cout << "2. INSERT:" << std::endl;
		std::cout << "   Format: INSERT INTO table_name VALUES (val1, val2, ...)" << std::endl;
		std::cout << "   E.g.:     INSERT INTO students VALUES (\"Alex\", 21)" << std::endl;

		std::cout << "3. SELECT:" << std::endl;
		std::cout << "   Format: SELECT (col1, col2, ...) | ALL FROM table_name [WHERE column_name = value]"
				  << std::endl;
		std::cout << "   E.g.:     SELECT name FROM students WHERE age = 21" << std::endl;

		std::cout << "4. INDEX:" << std::endl;
		std::cout << " CREATE INDEX index_name ON table_name (column_name)" << std::endl;
		std::cout << "   E.g.:     CREATE INDEX name_index ON students (name)" << std::endl;
		std::cout << "-----------------------------" << std::endl;
		std::cout << "Press any key (and Enter) to go back..." << std::endl;
		char dummy;
		std::cin >> dummy; // Asteapta userul
	}

	void print_datatypes_help()
	{
		std::cout << std::endl;
		std::cout << "================ SUPPORTED DATA TYPES ================" << std::endl;
		std::cout << "* integer / int   : Whole numbers (e.g., 10, -5)" << std::endl;
		std::cout << "* numeric / float : Decimal numbers (e.g., 10.5)" << std::endl;
		std::cout << "* text / varchar  : Strings (e.g., \'Hello World\')" << std::endl;
		std::cout << std::endl;
		std::cout << "(!) Note: Text columns require a maximum size definition." << std::endl;
		std::cout << "----------------------------" << std::endl;
		std::cout << "Press any key (and Enter) to go back..." << std::endl;
		char dummy;
		std::cin >> dummy;
	}

	void print_general_help()
	{
		std::cout << std::endl;
		std::cout << "================ GENERAL INFO ================" << std::endl;
		std::cout << "Storage:" << std::endl;
		std::cout << "  - Tables -> /tables folder (binary files)" << std::endl;
		std::cout << "  - Indexes -> /index_catalog folder (binary files)" << std::endl;
		std::cout << "Navigation:" << std::endl;
		std::cout << "  - Use 'q' to Quit, 'r' to Return in menus." << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << "Press any key (and Enter) to go back..." << std::endl;
		char dummy;
		std::cin >> dummy;
	}

public:
	Menu() { application = new Application(); }


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
			case '1':
				clear_screen();
				run_interactive_menu();
				break;
			case '2':
				clear_screen();
				application->parse_commands();
				break;
			case 'h':
				clear_screen();
				display_help();
				break;
			case 'q':
				exit(0);
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
		std::cout << "(1) A table." << std::endl;
		std::cout << "(2) An index." << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
		std::cout << "(q) Quit program." << std::endl;
		std::cout << "Please choose an option: [12rq] ";

		char c;

		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
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
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [tirq] ";
			}
		}
	}

	void create_index()
	{
		std::cout << std::endl;
		std::string tableName, indexName, columnName;
		std::cout << "Please enter the name of the table on which you want to create the index: (Available options: ";

		int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables > 0)
		{
			Table* tables = tableCatalog->getTables();
			for (int i = 0; i < noOfTables - 1; i++)
			{
				std::cout << '\"' + tables[i].getTableName() + "\", ";
			}
			std::cout << '\"' << tables[noOfTables - 1].getTableName() << "\") ";
		}
		else
		{
			std::cout << "none.";
		}

		while (true)
		{
			std::cin >> tableName;
			if (tableCatalog->table_exists(tableName))
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			std::cout << "Please enter the name of the table on which you want to create the index: ";
		}
		Table* table = tableCatalog->getTable(tableName);
		std::cout << "Please enter the name of the index you want to create: ";
		while (true)
		{
			std::cin >> indexName;
			if (indexCatalog->index_exists(indexName) == false)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Index with name \"" + indexName + "\" already exists!");
			std::cout << "Please enter the name of the index you want to create: ";
		}
		std::cout << "Please enter the name of the column you want to index: (Available options: ";
		std::string* columns = table->getColumns();
		int noOfColumns = table->getNoOfColumns();
		for (int i = 0; i < noOfColumns - 1; i++)
		{
			std::cout << '\"' << columns[i] << "\", ";
		}
		std::cout << '\"' << columns[noOfColumns - 1] << "\") ";
		while (true)
		{
			std::cin >> columnName;
			if (table->column_exists(columnName) == true && indexCatalog->has_index(indexName, columnName) == false)
			{
				break;
			}
			if (table->column_exists(columnName) == false)
			{
				statusManager->print(StatusManager::Error,
									 "Table \"" + tableName + "\" does not have column \"" + columnName + "\"!");
				std::cout << "Please enter the name of the column you want to index: ";
			}
			else if (indexCatalog->has_index(tableName, columnName) == true)
			{
				statusManager->print(StatusManager::Error,
									 "Table \"" + tableName + "\" already has an index on column \"" + columnName +
										 "\"!");
				std::cout << "Please enter the name of the column you want to index: ";
			}
		}

		auto index = new Index(indexName, tableName, columnName);
		indexCatalog->add_index(*index);
		table->add_index(indexName);
		application->write_table_to_file(*table);
		indexCatalog->write_index_catalog_to_file();
		statusManager->print(StatusManager::Success, "Index \"" + indexName + "\" created successfully!");
		std::cout << std::endl;
		show_menu_loop(index_options);

		char c;
		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
			{
			case 'r':
				clear_screen();
				show_index_options();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [rq] ";
			}
		}
	}

	void drop_index() {}

	void display_indexes()
	{
		std::cout << std::endl;
		int noOfTables = tableCatalog->getNoOfTables();
		if (noOfTables > 0)
		{
			std::cout << "Please enter the name of the table of which you wish to display indexes: " << std::endl
					  << "(Available options: ";
			Table* tables = tableCatalog->getTables();
			for (int i = 0; i < noOfTables - 1; i++)
			{
				std::cout << '\"' + tables[i].getTableName() + "\", ";
			}
			std::cout << '\"' << tables[noOfTables - 1].getTableName() << "\") ";
		}
		else
		{
			std::cout << "You have to create a table first!" << std::endl << std::endl;
			show_menu_loop(index_options);
		}
		std::string tableName;
		while (true)
		{
			std::cin >> tableName;
			if (tableCatalog->table_exists(tableName) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			std::cout << "Please enter the name of the table of which you wish to display indexes: ";
		}
		if (indexCatalog->getNoOfIndexesOfTableByName(tableName) == 0)
		{
			std::cout << "Table \"" << tableName << "\" does not have any index!" << std::endl << std::endl;
			show_menu_loop(index_options);
		}
		else
		{
			indexCatalog->print_indexes_of_a_table_by_name(tableName);
			show_menu_loop(index_options);
		}
	}

	void show_menu_loop(menu_options option)
	{

		std::cout << "What would you like to do now?" << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
		std::cout << "(q) Quit the program." << std::endl;
		std::cout << "Please choose an option: [rq] ";
		char c;
		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
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
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [rq] ";
			}
		}
	}

	void show_index_options()
	{
		std::cout << std::endl;
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "(1) Create an index." << std::endl;
		std::cout << "(2) Display all indexes of a table." << std::endl;
		std::cout << "(3) Drop an index." << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
		std::cout << "(q) Quit program." << std::endl;
		std::cout << "Please choose an option: [123rq] ";

		char c;
		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
			{
			case '1':
				clear_screen();
				create_index();
				break;
			case '2':
				clear_screen();
				display_indexes();
				break;
			case '3':
				clear_screen();
				drop_index();
				break;
			case 'r':
				clear_screen();
				run_interactive_menu();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [123rq] ";
			}
		}
	}

	void select_from()
	{
		std::cout << std::endl;
		std::string tableName;
		std::cout << "Please enter the name of the table you would like to select from: (Available options: ";
		int noOfTables = tableCatalog->getNoOfTables();
		Table* tables = tableCatalog->getTables();
		for (int i = 0; i < noOfTables - 1; i++)
		{
			std::cout << '\"' + tables[i].getTableName() + "\", ";
		}
		std::cout << '\"' << tables[noOfTables - 1].getTableName() << "\") ";
		while (true)
		{
			std::cin >> tableName;
			if (tableCatalog->table_exists(tableName) == true)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			std::cout << "Please enter the name of the table you would like to select from: ";
		}
		auto originalTable = tableCatalog->getTable(tableName);
		std::cout << "Please enter the columns you would like to select, separated by spaces, or \"ALL\" to select all "
					 "columns: (Available options: ";
		int totalNoOfColumns = originalTable->getNoOfColumns();
		auto availableColumns = originalTable->getColumns();
		for (int i = 0; i < totalNoOfColumns - 1; i++)
		{
			std::cout << '\"' << availableColumns[i] << "\", ";
		}
		std::cout << '\"' << availableColumns[totalNoOfColumns - 1] << "\")" << std::endl;
		std::cout << "[E.g.: col1 col2 col3 ...] ";
		auto selectedColumns = new std::string[50];
		std::string columnName;
		int noOfSelectedColumns;
		bool checkForAll = false;
		while (true)
		{
			delete[] selectedColumns;
			selectedColumns = new std::string[50];
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
					std::cout << "Column \"" << selectedColumns[i]
							  << "\" does not exist in table \"" + tableName + "\"!" << std::endl;
					has_error = true;
				}
			}
			if (has_error == false)
			{
				break;
			}
			std::cout << "Please enter the columns you would like to select, separated by spaces: ";
		}

		std::cout << std::endl;
		std::cout << "Would you like to search based on the value of a column?" << std::endl;
		std::cout << "(y) Yes" << std::endl;
		std::cout << "(n) No" << std::endl;
		std::cout << "Please choose an option: [yn] ";

		char c;
		bool search_with_column_name;
		std::string columnNameSearchedFor, value;
		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
			{
			case 'y':
				std::cout << std::endl;
				std::cout << "Please enter the column name and the value you are searching for, separated by spaces: "
						  << std::endl;
				std::cout << "[E.g.: first_name Andrei] ";
				while (true)
				{
					std::cin >> columnNameSearchedFor >> value;
					if (originalTable->column_exists(columnNameSearchedFor) == true)
					{
						break;
					}
					statusManager->print(StatusManager::Error,
										 "Column \"" + columnNameSearchedFor + "\" does not exist!");
					std::cout
						<< "Please enter the column name and the value you are searching for, separated by spaces: ";
				}
				search_with_column_name = true;
				break;
			case 'n':
				search_with_column_name = false;
				break;
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [yn] ";
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
					clear_screen();
					statusManager->print(StatusManager::Error,
										 "No matching values for: \"" + value + "\" in column: \"" + columnName +
											 "\"!");
					std::cout << std::endl;
					show_menu_loop(table_options);
				}
				application->write_select_to_file(*tableWithSelectedColumnsOnly);
				tableWithSelectedColumnsOnly->print_table(std::cout);
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
				clear_screen();
				application->write_select_to_file(*tableWithSelectedColumnsOnly);
				tableWithSelectedColumnsOnly->print_table(std::cout);
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
										 "No matching values for: \"" + value + "\" in column: \"" +
											 columnNameSearchedFor + "\"!");
					std::cout << std::endl;
				}
				else
				{
					clear_screen();
					tableWithSelectedRows->print_table(std::cout);
					std::cout << std::endl;
					application->write_select_to_file(*tableWithSelectedRows);
				}
			}
			else
			{
				clear_screen();
				tableCatalog->getTable(tableName)->print_table(std::cout);
				std::cout << std::endl;
				application->write_select_to_file(*tableCatalog->getTable(tableName));
			}
		}
		show_menu_loop(table_options);
	}

	void update_table() {}

	void delete_from() {}

	void drop_table() {}

	void show_table_options()
	{
		std::cout << std::endl;
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "(1) Create a table." << std::endl;
		std::cout << "(2) Display a table." << std::endl;
		std::cout << "(3) Select from a table." << std::endl;
		std::cout << "(4) Update a table." << std::endl;
		std::cout << "(5) Delete from a table." << std::endl;
		std::cout << "(6) Drop a table." << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
		std::cout << "(q) Quit program." << std::endl;
		std::cout << "Please choose an option: [1-6rq] ";

		char c;
		while (true)
		{
			std::cin >> c;
			c = tolower(c);
			switch (c)
			{
			case '1':
				clear_screen();
				create_table();
				break;
			case '2':
				clear_screen();
				display_table();
				break;
			case '3':
				clear_screen();
				select_from();
				break;
			case '4':
				clear_screen();
				update_table();
				break;
			case '5':
				clear_screen();
				delete_from();
				break;
			case '6':
				clear_screen();
				drop_table();
				break;
			case 'r':
				clear_screen();
				run_interactive_menu();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [1-6rq] ";
			}
		}
	}

	void create_table()
	{
		std::cout << std::endl;
		std::string tableName;
		int noOfColumns;
		std::cout << "Please enter the name of the table you would like to create: ";
		while (true)
		{
			std::cin >> tableName;
			if (tableCatalog->table_exists(tableName) == false)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" already exists!");
			std::cout << "Please enter the name of the table you would like to create: ";
		}
		std::cout << "Please enter the number of columns you wish the table to have: ";
		while (true)
		{
			std::cin >> noOfColumns;
			if (noOfColumns > 0)
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Number of columns has to be at least 1!");
			std::cout << "Please enter the number of columns you wish the table to have: ";
		}
		std::cout << "Please enter attribute details separated by spaces(Name, Type, Length, Default): " << std::endl;
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
									 "You need to enter a valid column type! (Options are: text, varchar, int, "
									 "integer, numeric, float)");
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

		statusManager->print(StatusManager::Success, "Table \"" + tableName + "\" created succesfully!");
		show_menu_loop(table_options);
	}

	void display_help()
	{
		while (true)
		{
			clear_screen();
			std::cout << std::endl;
			std::cout << "================ HELP CENTER ================" << std::endl;
			std::cout << "(1) Syntax Guide (Commands & Examples)" << std::endl;
			std::cout << "(2) Data Types Reference" << std::endl;
			std::cout << "(3) General Information" << std::endl;
			std::cout << "(r) Return to Main Menu" << std::endl;
			std::cout << "(q) Quit program" << std::endl;
			std::cout << "=============================================" << std::endl;
			std::cout << "Please choose an option: [123rq] ";

			char c;
			std::cin >> c;
			c = tolower(c);

			clear_screen();

			switch (c)
			{
			case '1':
				print_syntax_help();
				break;
			case '2':
				print_datatypes_help();
				break;
			case '3':
				print_general_help();
				break;
			case 'r':
				display_welcome_menu();
				return;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "Invalid option!");
				break;
			}
		}
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
		std::cout << std::endl;
		std::string tableName;
		std::cout << "What is the name of the table you want to display? (Available options: ";
		int noOfTables = tableCatalog->getNoOfTables();
		Table* tables = tableCatalog->getTables();
		for (int i = 0; i < noOfTables - 1; i++)
		{
			std::cout << '\"' + tables[i].getTableName() + "\", ";
		}
		std::cout << '\"' << tables[noOfTables - 1].getTableName() << "\") ";
		while (true)
		{
			std::cin >> tableName;
			if (tableCatalog->table_exists(tableName))
			{
				break;
			}
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			std::cout << "Please enter the name of the table: ";
		}
		clear_screen();
		tableCatalog->getTable(tableName)->print_table(std::cout);
		std::cout << std::endl;
		application->write_select_to_file(*tableCatalog->getTable(tableName));
		show_menu_loop(table_options);
	}
};

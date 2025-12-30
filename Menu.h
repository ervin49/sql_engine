#pragma once
#include <iostream>
#include "globals.h"

#include "StatusManager.h"

class Menu
{
private:
	Application* application;

public:
	Menu() { application = new Application(); }


	void display_welcome_menu()
	{
		clear_screen();
		first_configuration config;
		config.load_tables();
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
		std::cout << "(t) A table." << std::endl;
		std::cout << "(i) An index." << std::endl;
		std::cout << "(r) Return to the previous menu." << std::endl;
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
				index_options();
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
			else
			{
				statusManager->print(StatusManager::Error,
									 "Table \"" + tableName + "\" already has an index on column \"" + columnName +
										 "\"!");
				std::cout << "Please enter the name of the column you want to index: ";
			}
		}

		Index* index = new Index(indexName, tableName, columnName);
		indexCatalog->add_index(*index);
		table->add_index(indexName);
		application->write_table_to_file(*table);
		indexCatalog->write_index_catalog_to_file();
		statusManager->print(StatusManager::Success, "Index \"" + indexName + "\" created successfully!");
		std::cout << std::endl;
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
				index_options();
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
		std::cout << "Please enter the name of the table of which you wish to display indexes: (Available options: ";
		int noOfTables = tableCatalog->getNoOfTables();
		Table* tables = tableCatalog->getTables();
		for (int i = 0; i < noOfTables - 1; i++)
		{
			std::cout << '\"' + tables[i].getTableName() + "\", ";
		}
		std::cout << '\"' << tables[noOfTables - 1].getTableName() << "\") ";
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
		}
		else
		{
			indexCatalog->print_indexes_of_a_table_by_name(tableName);
		}
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
				index_options();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [rq] ";
			}
		}
	}

	void index_options()
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

	void select_from() {}

	void update_table() {}

	void delete_from() {}

	void drop_table() {}

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
				table_options();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [rq] ";
			}
		}
	}

	void display_help()
	{
		std::cout << std::endl;
		std::cout << "What do you need help with?" << std::endl;
		// de scris
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
		application->write_select_to_file(*tableCatalog->getTable(tableName));
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
				table_options();
				break;
			case 'q':
				exit(0);
			default:
				statusManager->print(StatusManager::Error, "You need to enter a valid option!");
				std::cout << "Please choose an option: [rq] ";
			}
		}
	}
};

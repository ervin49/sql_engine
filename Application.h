#pragma once
#include <cstring>
#include <fstream>

#include "Index.h"
#include "Index_Catalog.h"
#include "Parser.h"
#include "StatusManager.h"
#include "Table_Catalog.h"
#include "first_configuration.h"
#include "globals.h"


class Application
{
private:
	Parser* parser = new Parser();
	std::string* words;
	int noOfWords = 0;
	std::string s;

public:
	Application()
	{
		this->words = nullptr;
		this->noOfWords = 0;
	}

	Application(const std::string* words, const int noOfWords)
	{
		if (noOfWords != 0 && words != nullptr)
		{
			this->noOfWords = noOfWords;
			this->words = new std::string[noOfWords];
			for (int i = 0; i < noOfWords; i++)
			{
				this->words[i] = words[i];
			}
		}
	}

	~Application()
	{
		delete[] this->words;
		delete parser;
	}


	Application(const Application& a)
	{
		this->s = a.s;
		if (a.words != nullptr && a.noOfWords != 0)
		{
			this->noOfWords = a.noOfWords;
			this->words = new std::string[this->noOfWords];
			for (int i = 0; i < this->noOfWords; i++)
			{
				this->words[i] = a.words[i];
			}
		}
		else
		{
			this->noOfWords = 0;
			this->words = nullptr;
		}
		if (a.parser != nullptr)
		{
			// a.parser can not be nullptr (class invariant), but for safety reasons
			this->parser = new Parser(*a.parser);
		}
		else
		{
			this->parser = new Parser;
		}
	}


	Application& operator=(const Application& a)
	{
		if (this != &a)
		{
			if (this->words != nullptr)
			{
				delete[] this->words;
				this->words = nullptr;
			}

			this->s = a.s;
			if (a.words != nullptr && a.noOfWords != 0)
			{
				this->noOfWords = a.noOfWords;
				this->words = new std::string[this->noOfWords];
				for (int i = 0; i < this->noOfWords; i++)
				{
					this->words[i] = a.words[i];
				}
			}
			else
			{
				this->noOfWords = 0;
			}
			if (a.parser != nullptr)
			{
				// a.parser can not be nullptr (class invariant), but for safety reasons x2
				this->parser = new Parser(*a.parser);
			}
			else
			{
				this->parser = new Parser;
			}
		}
		return *this;
	}

	operator int() const { return this->noOfWords; }

	bool operator!() const { return noOfWords == 0; }

	std::string operator[](int index) const
	{
		if (index >= 0 && index < noOfWords)
		{
			return words[index];
		}
		return "";
	}

	void print_application() const { std::cout << "Current running application: " << "de schimbat aici" << std::endl; }


	friend std::ostream& operator<<(std::ostream& out, const Application& application)
	{
		application.print_application();
		return out;
	}

	bool operator==(const Application& application) const
	{
		if (this->parser != application.parser || this->noOfWords != application.noOfWords)
			return false;

		for (int i = 0; i < noOfWords; i++)
		{
			if (this->words[i] != application.words[i])
			{
				return false;
			}
		}

		if (this->s != application.s)
			return false;

		return true;
	}


	void setQuery(std::string* words, int noOfWords, std::string s)
	{
		this->s = s;
		this->words = words;
		this->noOfWords = noOfWords;
	}

	void parse_command() const
	{
		std::string firstWord = words[0];
		if (firstWord == "create")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			std::string secondWord = words[1];
			if (secondWord == "table")
			{
				if (has_invalid_word_count(2) || has_invalid_word_count(3))
				{
					return;
				}

				if (words[3] == "if")
				{
					create_table_with_if_not_exists();
				}
				else
				{
					create_table_without_if_not_exists();
				}
			}
			else if (secondWord == "index")
			{
				if (has_invalid_word_count(2))
				{
					return;
				}
				if (words[3] == "if")
				{
					create_index_with_if_not_exists();
				}
				else
				{
					create_index_without_if_not_exists();
				}
			}
			else
			{
				statusManager->print(StatusManager::Error, "Wrong statement! You can create a table or an index.");
			}
		}
		else if (firstWord == "drop")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			std::string secondWord = words[1];
			if (secondWord == "table")
			{
				if (has_invalid_word_count(2))
				{
					return;
				}
				drop_table();
			}
			else if (secondWord == "index")
			{
				if (has_invalid_word_count(2))
				{
					return;
				}
				drop_index();
			}
			else
			{
				statusManager->print(StatusManager::Error, "Wrong statement! You can drop a table or an index.");
			}
		}
		else if (firstWord == "display")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			display_table();
		}
		else if (firstWord == "insert")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			insert_into_table();
		}
		else if (firstWord == "select")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			select_from();
		}
		else if (firstWord == "delete")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			delete_from();
		}
		else if (firstWord == "update")
		{
			if (has_invalid_word_count(1))
			{
				return;
			}
			update_table();
		}
		else if (firstWord == "debug")
		{
			if (noOfWords == 1)
			{
				std::cout << "tables or indexes?" << std::endl;
			}
			else
			{
				if (words[1] == "tables")
				{
					if (tableCatalog->getNoOfTables() == 0)
					{
						statusManager->print(StatusManager::Error, "There are no tables!");
					}
					else
					{
						std::cout << "The tables are:" << std::endl;
						tableCatalog->print_tables();
					}
				}
				else if (words[1] == "indexes")
				{
					indexCatalog->print_indexes();
				}
			}
		}
		else if ( firstWord == "import" ) {
			if (has_invalid_word_count(1) || has_invalid_word_count(2))
			{
				return;
			}
			import_table();
		}
		else
		{
			statusManager->print(StatusManager::Error, "Command is wrong! Please enter a new command. ");
		}
	}

	bool has_invalid_word_count(int wordCount) const
	{
		if (this->noOfWords == wordCount)
		{
			statusManager->print(StatusManager::Error, "Incomplete input!");
			return true;
		}
		return false;
	}

	void insert_into_table() const
	{
		if (words[1] != "into" || words[3] != "values")
		{
			statusManager->print(StatusManager::Error, "Syntax error!");
			return;
		}

		const std::string tableName = words[2];
		if (!tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			return;
		}

		int noOfFields;
		std::string* inputFields = parser->parse_column(words[4], noOfFields);

		if (noOfFields != tableCatalog->getNumberOfColumns(tableName))
		{
			statusManager->print(StatusManager::Error, "Invalid input!");
			return;
		}

		auto table = tableCatalog->getTable(tableName);
		if (table->add_row(inputFields) == 0)
		{
			write_table_to_file(*table);
			int noOfRows = table->getNoOfRows();
			statusManager->print(StatusManager::Success,
								 "Inserted successfully! (Total number of rows: " + std::to_string(noOfRows) + ')');
		}
	}


	void create_table(const std::string& tableName) const
	{
		int indexOfLastWord = noOfWords - 1;
		if (words[indexOfLastWord][0] != '(' || words[indexOfLastWord][1] != '(' ||
			words[indexOfLastWord][words[indexOfLastWord].length() - 2] != ')')
		{
			statusManager->print(StatusManager::Error, "Invalid parentheses!");
			return;
		}

		for (int i = 0; i < words[indexOfLastWord].length(); i++)
		{
			if (!isascii(words[indexOfLastWord][i]))
			{
				statusManager->print(StatusManager::Error, "Only ascii characters allowed!");
				return;
			}
		}

		int noOfColumns = 0;

		bool isColumnInfo = false;
		bool isText = false;

		for (int i = 1; i < words[indexOfLastWord].length() - 1; i++)
		{
			if (words[indexOfLastWord][i] == '"')
			{
				isText = !isText;
			}
			if (isText == false)
			{
				if (words[indexOfLastWord][i] == ')')
				{
					noOfColumns++;
				}
			}
		}

		auto* columns = new std::string[noOfColumns + 1];
		int k = 0;
		for (int i = 1; i < words[indexOfLastWord].length() - 1; i++)
		{
			if (words[indexOfLastWord][i] == '"')
			{
				isText = !isText;
			}
			if (isText == false)
			{
				if (words[indexOfLastWord][i] == '(')
				{
					isColumnInfo = true;
				}
				else if (words[indexOfLastWord][i] == ')')
				{
					isColumnInfo = false;
					k++;
				}
			}
			if (isColumnInfo == true || isText == true)
			{
				columns[k] += words[indexOfLastWord][i];
			}
		}


		int noOfFields;
		auto* table = new Table(noOfColumns, tableName);
		auto* columnNames = new std::string[noOfColumns];
		auto* columnTypes = new std::string[noOfColumns];
		auto* maxColumnLengths = new unsigned int[noOfColumns];
		for (int j = 0; j < noOfColumns; j++)
		{
			const auto fields = parser->parse_column(columns[j], noOfFields);

			if (noOfFields != 4)
			{
				statusManager->print(StatusManager::Error, "Every column should contain exactly 4 fields!");
				delete table;
				delete[] columnTypes;
				delete[] maxColumnLengths;
				delete[] columnNames;
				delete[] columns;
				return;
			}
			columnNames[j] = fields[0];
			columnTypes[j] = fields[1];
			if (table->is_integer(fields[2]))
			{
				maxColumnLengths[j] = stoi(fields[2]);
			}
			else
			{
				statusManager->print(
					StatusManager::Error,
					"The value \"" + fields[2] +
						"\" is invalid! You need to specify a positive integer for the attribute size.");
				delete table;
				delete[] maxColumnLengths;
				delete[] columnTypes;
				delete[] columnNames;
				delete[] columns;
				return;
			}
		}

		for (int i = 0; i < noOfColumns - 1; i++)
		{
			for (int j = i + 1; j < noOfColumns; j++)
			{
				if (columnNames[i] == columnNames[j])
				{
					statusManager->print(StatusManager::Error, "You can't enter two identical column names!");
					delete table;
					delete[] columns;
					delete[] columnNames;
					delete[] columnTypes;
					return;
				}
			}
		}

		table->setColumnNames(columnNames, noOfColumns);
		table->setColumnTypes(columnTypes, noOfColumns);
		table->setMaxColumnLengths(maxColumnLengths, noOfColumns);

		if (tableCatalog->add_table(*table) == 0)
		{
			write_table_to_file(*table);
			statusManager->print(StatusManager::Success, "Table \"" + tableName + "\" created successfully!");
		}

		delete table;
		delete[] columnTypes;
		delete[] columnNames;
		delete[] columns;
	}

	void write_table_to_file(Table table) const
	{
		// open the file(or create it if it doesn't exist already)
		std::string tableName = table.getTableName();
		std::ofstream file("./tables/" + tableName + ".bin", std::ios::binary);

		// get all the variables from the table
		int noOfColumns = table.getNoOfColumns(), noOfRows = table.getNoOfRows(), noOfIndexes = table.getNoOfIndexes();
		std::string* columns = table.getColumns();
		std::string** rows = table.getRows();
		std::string* indexNames = table.getIndexNames();
		std::string* columnTypes = table.getColumnTypes();
		unsigned int* maxColumnLengths = table.getMaxColumnLengths();

		// write into the file
		file.write(reinterpret_cast<char*>(&noOfColumns), sizeof(int));
		file.write(reinterpret_cast<char*>(&noOfRows), sizeof(int));
		file.write(reinterpret_cast<char*>(&noOfIndexes), sizeof(int));

		// have to write the strings lengths
		// before the strings themselves
		int len = tableName.size();
		file.write(reinterpret_cast<char*>(&len), sizeof(int));
		file.write(tableName.data(), len);

		// write the columns
		for (int i = 0; i < noOfColumns; i++)
		{
			len = columns[i].length();
			file.write(reinterpret_cast<char*>(&len), sizeof(int));
			file.write(columns[i].data(), len);

			len = columnTypes[i].length();
			file.write(reinterpret_cast<char*>(&len), sizeof(int));
			file.write(columnTypes[i].data(), len);

			file.write(reinterpret_cast<char*>(&maxColumnLengths[i]), sizeof(int));
		}

		// write the rows
		// if this function was called from create table
		// then noOfRows will be 0 so it won't enter the for loop
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				len = rows[i][j].size();
				file.write(reinterpret_cast<char*>(&len), sizeof(int));
				file.write(rows[i][j].data(), len);
			}
		}

		// write the indexes
		// same thing as the rows
		for (int i = 0; i < noOfIndexes; i++)
		{
			len = indexNames[i].size();
			file.write(reinterpret_cast<char*>(&len), sizeof(int));
			file.write(indexNames[i].data(), len);

			len = indexCatalog->getIndex(indexNames[i])->getColumnName().length();
			file.write(reinterpret_cast<char*>(&len), sizeof(int));
			file.write(indexCatalog->getIndex(indexNames[i])->getColumnName().data(), len);
		}
	}


	void create_table_without_if_not_exists() const
	{
		std::string tableName;

		if (noOfWords != 4)
		{
			statusManager->print(StatusManager::Error, "Invalid syntax!");
			return;
		}

		for (int i = 0; i < words[2].length(); i++)
		{
			tableName += tolower(words[2][i]);
		}

		if (tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" already exists!");
			return;
		}

		create_table(tableName);
	}

	void create_table_with_if_not_exists() const
	{
		std::string tableName;
		for (int i = 0; i < words[2].length(); i++)
		{
			tableName += tolower(words[2][i]);
		}

		if (noOfWords < 6)
		{
			return;
		}
		if (words[4] != "not")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[4] + "\"!");
			return;
		}
		if (words[5] != "exists")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[5] + "\"!");
			return;
		}
		if (tableCatalog->table_exists(tableName))
		{
			return;
		}
		create_table(tableName);
	}

	void create_index_without_if_not_exists() const
	{
		std::string indexName;
		for (int i = 0; i < words[2].length(); i++)
		{
			indexName += tolower(words[2][i]);
		}

		if (indexCatalog->index_exists(indexName))
		{
			statusManager->print(StatusManager::Error, "Index already exists!");
			return;
		}

		create_index();
	}

	void create_index_with_if_not_exists() const
	{
		if (noOfWords < 6)
		{
			statusManager->print(StatusManager::Error, "Incomplete input!");
		}
		if (words[3] != "not")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[3] + "\"!");
			return;
		}
		if (words[4] != "exists")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[4] + "\"!");
			return;
		}
		if (words[6] != "on")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[6] + "\"!");
			return;
		}

		std::string indexName;
		for (int i = 0; i < words[5].length(); i++)
		{
			indexName += tolower(words[5][i]);
		}

		if (indexCatalog->index_exists(indexName))
		{
			return;
		}
		create_index();
	}

	void create_index() const
	{
		int indexOfLastWord = noOfWords - 1;
		if (noOfWords < 6)
		{
			statusManager->print(StatusManager::Error, "Incomplete input!");
			return;
		}

		std::string indexName = words[noOfWords - 4];

		std::string tableName = words[indexOfLastWord - 1];
		if (!tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table " + tableName + " does not exist! Cannot create index.");
			return;
		}

		if (words[indexOfLastWord][0] != '(' || words[indexOfLastWord][words[indexOfLastWord].length() - 1] != ')')
		{
			statusManager->print(StatusManager::Error, "Invalid parentheses!");
			return;
		}

		std::string columnName;
		for (int i = 1; i < words[indexOfLastWord].length() - 1; i++)
		{
			columnName += tolower(words[indexOfLastWord][i]);
		}

		auto table = tableCatalog->getTable(tableName);
		if (!table->column_exists(columnName))
		{
			statusManager->print(StatusManager::Error, "Column " + columnName + " does not exist!");
			return;
		}

		if (indexCatalog->setIndex(indexName, columnName) == 0)
		{
			Index* index = new Index(indexName, tableName, columnName);
			indexCatalog->add_index(*index);
			table->add_index(indexName);
			write_table_to_file(*table);
			indexCatalog->write_index_catalog_to_file();
			statusManager->print(StatusManager::Success, "Index \"" + indexName + "\" created successfully!");
			delete index;
		}
	}

	void drop_table() const
	{
		std::string tableName, aux = words[2];

		if (noOfWords != 3)
		{
			statusManager->print(StatusManager::Error,
								 "Argument count mismatch: expected 3, got " + std::to_string(noOfWords) + "!");
			return;
		}

		for (int i = 0; i < words[2].length(); i++)
		{
			tableName += tolower(words[2][i]);
		}

		if (tableCatalog->drop_table(tableName) == 0)
		{
			remove(("./tables/" + tableName + ".bin").c_str());
			statusManager->print(StatusManager::Success, "Table \"" + aux + "\" dropped successfully!");
		}
	}

	void drop_index() const
	{
		std::string indexName, aux = words[2];
		for (int i = 0; i < words[2].length(); i++)
		{
			indexName += tolower(words[2][i]);
		}

		auto index = indexCatalog->getIndex(indexName);
		if (index == nullptr)
		{
			statusManager->print(StatusManager::Error, "Index \"" + indexName + "\" does not exist!");
			return;
		}
		const std::string tableNameOfIndex = indexCatalog->getIndex(indexName)->getTableName();
		if (indexCatalog->drop_index(indexName) == 0)
		{
			auto table = tableCatalog->getTable(tableNameOfIndex);
			table->remove_index(indexName);
			statusManager->print(StatusManager::Success, "Index \"" + aux + "\" dropped successfully!");
			indexCatalog->write_index_catalog_to_file();
		}
	}

	void display_table() const
	{
		if (noOfWords != 3)
		{
			statusManager->print(StatusManager::Error,
								 "Argument count mismatch: expected 3, got " + std::to_string(noOfWords) + "!");
			return;
		}

		std::string tableName = words[2];
		if (tableCatalog->table_exists(tableName))
		{
			tableCatalog->getTable(tableName)->print_table(std::cout);
			write_select_to_file(*tableCatalog->getTable(tableName));
		}
		else
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
		}
	}

	void update_table() const
	{
		std::string tableName = words[1];

		if (noOfWords != 10)
		{
			statusManager->print(StatusManager::Error,
								 "Invalid number of tokens, expected 9, got " + std::to_string(noOfWords) + "!");
			return;
		}

		if (!tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			return;
		}
		if (words[2] != "set")
		{
			statusManager->print(StatusManager::Error,
								 "Syntax Error: Expected 'SET' at position 2, but found \"" + words[2] + "\".");
			return;
		}

		auto table = tableCatalog->getTable(tableName);
		std::string setColumnName = words[3];
		if (!table->column_exists(setColumnName))
		{
			statusManager->print(StatusManager::Error,
								 "Table \"" + tableName + "\" does not have column \"" + setColumnName + "\"!");
			return;
		}
		if (words[4] != "=")
		{
			statusManager->print(StatusManager::Error,
								 "Syntax Error: Expected '=' at position 4, but found \"" + words[4] + "\".");
			return;
		}
		if (words[8] != "=")
		{
			statusManager->print(StatusManager::Error,
								 "Syntax Error: Expected '=' at position 8, but found \"" + words[8] + "\".");
			return;
		}

		std::string whereColumnName = words[7];
		if (!table->column_exists(whereColumnName))
		{
			statusManager->print(StatusManager::Error,
								 "Table \"" + tableName + "\" does not have column \"" + whereColumnName + "\"!");
			return;
		}
		int setIndex = table->return_index_of_column_by_name(setColumnName);
		std::string setValue = words[5];
		int whereIndex = table->return_index_of_column_by_name(whereColumnName);
		std::string whereValue = words[9];
		std::string** tableRows = table->getRows();
		for (int i = 0; i < table->getNoOfRows(); i++)
		{
			if (tableRows[i][whereIndex] == whereValue)
			{
				tableRows[i][setIndex] = setValue;
			}
		}
		table->setRows(tableRows, table->getNoOfRows(), table->getNoOfColumns());
		write_table_to_file(*table);
		std::cout << setValue << std::endl;
		statusManager->print(StatusManager::Success, "Updated table successfully!");
	}

	void delete_from() const
	{
		if (noOfWords > 7)
		{
			statusManager->print(StatusManager::Error, "Invalid number of tokens.");
			return;
		}
		if (noOfWords < 6)
		{
			statusManager->print(StatusManager::Error, "Incomplete input!");
			return;
		}
		if (words[1] != "from")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[1] + "\"!");
			return;
		} // de adaugat near si aici
		if (words[3] != "where")
		{
			statusManager->print(StatusManager::Error, "Parse error near \"" + words[3] + "\"!");
			return;
		}

		std::string tableName = words[2];
		std::string columnName = words[4];
		std::string value = words[6];
		if (!tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			return;
		}

		Table* table = tableCatalog->getTable(tableName);
		if (table->delete_from(columnName, value) == 0)
		{
			statusManager->print(StatusManager::Success, "Row deleted successfully!");
			write_table_to_file(*table);
		}
	}

	void write_select_to_file(Table table) const
	{
		std::string targetPath = "./select_outputs/";
		int index = -1; // subtract "." and ".."
		DIR* dir = opendir(targetPath.c_str());
		while (readdir(dir) != nullptr)
		{
			index++;
		}
		std::ofstream file;
		file.open(targetPath + "select_" + std::to_string(index));
		table.print_table(file);
	}

	void select_from() const
	{
		std::string tableName = words[3];

		if (!tableCatalog->table_exists(tableName))
		{
			statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
			return;
		}

		std::string secondWord;
		for (int i = 0; i < 3; i++)
		{
			secondWord += tolower(words[1][i]);
		}
		bool checkForAll = secondWord == "all";

		if (checkForAll == true)
		{
			if (noOfWords == 4)
			{
				tableCatalog->getTable(tableName)->print_table(std::cout);
				write_select_to_file(*tableCatalog->getTable(tableName));
			}
			else
			{
				if (words[4] != "where")
				{
					statusManager->print(StatusManager::Error,
										 "Syntax Error: Expected keyword 'WHERE', but found \"" + words[4] +
											 "\" instead.");
				}
				else if (words[6] != "=")
				{
					statusManager->print(StatusManager::Error,
										 "Syntax Error: Expected symbol '=', but found \"" + words[6] + "\" instead.");
				}
				auto originalTable = tableCatalog->getTable(tableName);
				std::string columnName = words[noOfWords - 3];
				// value that we search for
				std::string value = words[noOfWords - 1];
				bool found = false;
				int index = originalTable->return_index_of_column_by_name(columnName);
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
										 "No matching values for: \"" + value + "\" in column: \"" + columnName +
											 "\"!");
				}
				else
				{
					tableWithSelectedRows->print_table(std::cout);
					write_select_to_file(*tableWithSelectedRows);
				}

				// delete all dynamically allocated variables
				delete tableWithSelectedRows;
			}
			return;
		}

		if (noOfWords > 8)
		{
			statusManager->print(StatusManager::Error, "Syntax error!");
			return;
		}

		int poz = 0;
		while (poz < s.length() && s[poz] != '(')
		{
			poz++; // dupa asta i se va afla pe prima paranteza
		}

		int noOfSelectedColumns = 1;
		for (int i = poz + 1; i < s.length() && s[i] != ')'; i++)
		{
			if (i > 0 && s[i] == ',')
			{
				noOfSelectedColumns++;
			}
		}

		auto* selectedColumns = new std::string[noOfSelectedColumns];
		int columnIndex = 0;
		for (int i = poz + 1; i < s.length() && s[i] != ')'; i++)
		{
			if (s[i] == ' ')
			{
				continue;
			}
			if (s[i] == ',')
			{
				columnIndex++;
			}
			else
			{
				selectedColumns[columnIndex] += s[i];
			}
		}

		const auto originalTable = tableCatalog->getTable(tableName);
		for (int i = 0; i < noOfSelectedColumns; i++)
		{
			if (originalTable->column_exists(selectedColumns[i]) == false)
			{
				statusManager->print(StatusManager::Error,
									 "Column \"" + selectedColumns[i] + "\" does not exist in table \"" + tableName +
										 "\"!");
				return;
			}
		}
		// set the number of rows of new table to be the same as the original table
		// create a new table only with the columns we need
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


		if (noOfWords < 5)
		{
			tableWithSelectedColumnsOnly->print_table(std::cout);
			write_select_to_file(*tableWithSelectedColumnsOnly);
			delete tableWithSelectedColumnsOnly;
			delete[] selectedColumns;
			return;
		} // for select with no where clause

		if (words[4] != "where")
		{
			statusManager->print(StatusManager::Error,
								 "Syntax Error: Expected keyword 'WHERE', but found \"" + words[4] + "\" instead.");
		}
		else if (words[6] != "=")
		{
			statusManager->print(StatusManager::Error,
								 "Syntax Error: Expected symbol '=', but found \"" + words[6] + "\" instead.");
		}

		const std::string columnName = words[noOfWords - 3]; //
		// value that we search for
		const std::string value = words[noOfWords - 1]; //
		auto* tableWithSelectedRows = new Table(noOfSelectedColumns, "");

		for (int i = 0; i < noOfSelectedColumns; i++)
		{
			tableWithSelectedRows->setColumn(i, selectedColumns[i]);
		}

		bool found = false;
		int index = tableWithSelectedRows->return_index_of_column_by_name(columnName);

		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfSelectedColumns; j++)
			{
				if (rowsOfNewTable[i][j] == value && j == index)
				{
					// it matches what we are searching for
					found = true;
					tableWithSelectedRows->add_row(rowsOfNewTable[i]);
				}
			}
		}

		if (!found)
		{
			statusManager->print(StatusManager::Error,
								 "No matching values for: \"" + value + "\" in column: \"" + columnName + "\"!");
		}
		else
		{
			tableWithSelectedRows->print_table(std::cout);
			write_select_to_file(*tableWithSelectedRows);
		}

		// delete all dynamically allocated variables
		delete tableWithSelectedColumnsOnly;
		delete tableWithSelectedRows;
		delete[] selectedColumns;
	}
	void import_table() const{
        std::string tableName = words[1];
        std::string fileName = words[2];

        if (!tableCatalog->table_exists(tableName)) {
            statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exists!");
            return;
        }

        Table* table = tableCatalog->getTable(tableName);

        std::ifstream file(fileName);
        std::string line;

        if (!file.is_open()) {
            statusManager->print(StatusManager::Error, "Could not open file: \"" + fileName + "\"");
            return;
        }

        int noOfColumns = table->getNoOfColumns();
        int noOfRows = 0;
        int importedRows = 0;


        while (std::getline(file, line)) {
            noOfRows++;
            int noOfFields;

            //checking the delimiter (it should be ',')
            if (noOfColumns > 1) {
                int noOfCommas = 0;
                bool isText = false;
                for (int i = 0; i<line.length(); i++) {
                    //ignore ',' if it is in between ' '
                    if (line[i] == '\'') {
                        isText = !isText;
                    }
                    if (isText == false && line[i] == ',') {
                        noOfCommas++;
                    }
                }
                if (noOfCommas != noOfColumns - 1) {
                    statusManager->print(StatusManager::Error, "Invalid delimiter!");
                    continue;
                }
            }
            std:: string* fields = this->parser->parse_column(line, noOfFields);

            bool addRow = true;


            if (noOfColumns != noOfFields) {
                statusManager->print(StatusManager::Error, "Invalid column number on row " + std::to_string(noOfRows-1));
                addRow = false;
            }

            if (addRow) {
                table->add_row(fields);
                importedRows++;

            }

            delete[] fields;

        }
        file.close();

        if (importedRows > 0) {
            write_table_to_file(*table);
            statusManager->print(StatusManager::Success, "Import completed successfully!");
        } else {
            statusManager->print(StatusManager::Error, "No rows were imported.");
        }
    }


	void parse_commands_from_files(int argc, char** argv)
	{
		if (argc > 6)
		{
			throw std::runtime_error("You can't enter more than 5 input files!");
		}

		for (int i = 1; i < argc; i++)
		{
			std::string str = argv[i];
			if (str.substr(str.length() - 4, 4) != ".txt")
			{
				throw std::runtime_error("Input files need to have .txt extension!");
			}
		}
		first_configuration config;
		config.load_tables();
		config.load_index_catalog();

		int i = 1;
		for (std::string fileName; i < argc; i++)
		{
			fileName = argv[i];
			std::ifstream file(fileName);
			for (std::string line; std::getline(file, line);)
			{
				int noOfWords;

				parser->setCommandFromFile(line);
				std::string word = parser->getString();

				if (word.empty())
				{
					continue;
				}

				auto* numberOfParentheses = parser->checkBrackets();

				this->words = nullptr;

				if (numberOfParentheses[0] == numberOfParentheses[1] && numberOfParentheses[0] != 0)
				{
					if (numberOfParentheses[0] == 1 && tolower(word[0]) == 's')
					{
						words = parser->parse_with_brackets_select(noOfWords);
					}
					else
					{
						if (tolower(word[0]) == 'i')
						{
							words = parser->parse_with_brackets(noOfWords, true);
						}
						else
						{
							words = parser->parse_with_brackets(noOfWords);
						}
					}
				}
				else if (numberOfParentheses[0] == 0 && numberOfParentheses[1] == 0)
				{
					words = parser->parse_without_brackets(noOfWords);
				}
				else
				{
					statusManager->print(StatusManager::Error, "Invalid number of parentheses!");
					delete[] numberOfParentheses;
					continue;
				}

				setQuery(words, noOfWords, parser->getString());
				if (noOfWords == 1 && (words[0] == "exit" || words[0] == "quit"))
				{
					exit(0);
				}
				parse_command();
				delete[] numberOfParentheses;
			}
		}
	}

	void parse_commands()
	{
		std::cout << "Note: You can exit this program anytime by typing \"exit\" or \"quit\"." << std::endl
				  << std::endl;
		while (true)
		{
			int noOfWords;

			parser->setCommand();
			std::string word = parser->getString();

			if (word.empty())
			{
				continue;
			}

			auto* numberOfParentheses = parser->checkBrackets();

			this->words = nullptr;

			if (numberOfParentheses[0] == numberOfParentheses[1] && numberOfParentheses[0] != 0)
			{
				if (numberOfParentheses[0] == 1 && tolower(word[0]) == 's')
				{
					words = parser->parse_with_brackets_select(noOfWords);
				}
				else
				{
					if (tolower(word[0]) == 'i')
					{
						words = parser->parse_with_brackets(noOfWords, true);
					}
					else
					{
						words = parser->parse_with_brackets(noOfWords);
					}
				}
			}
			else if (numberOfParentheses[0] == 0 && numberOfParentheses[1] == 0)
			{
				words = parser->parse_without_brackets(noOfWords);
			}
			else
			{
				statusManager->print(StatusManager::Error, "Invalid number of parentheses!");
				delete[] numberOfParentheses;
				continue;
			}

			setQuery(words, noOfWords, parser->getString());
			if (noOfWords == 1 && (words[0] == "exit" || words[0] == "quit"))
			{
				exit(0);
			}
			parse_command();
			delete[] numberOfParentheses;
		}
	}
};

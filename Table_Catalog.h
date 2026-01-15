#pragma once
#include "Table.h"

class Table_Catalog
{
private:
	Table* tables;
	int noOfTables;

public:
	Table_Catalog()
	{
		tables = nullptr;
		noOfTables = 0;
	}

	Table_Catalog(const Table* tables, const int noOfTables)
	{
		if (noOfTables != 0 && tables != nullptr)
		{
			this->noOfTables = noOfTables;
			this->tables = new Table[noOfTables];
			for (int i = 0; i < noOfTables; i++)
			{
				this->tables[i] = tables[i];
			}
		}
	}

	Table_Catalog(const Table_Catalog& that)
	{
		if (that.noOfTables != 0 && that.tables != nullptr)
		{
			this->noOfTables = that.noOfTables;
			this->tables = new Table[noOfTables];
			for (int i = 0; i < noOfTables; i++)
			{
				this->tables[i] = that.tables[i];
			}
		}
		else
		{
			this->noOfTables = 0;
			this->tables = nullptr;
		}
	}

	~Table_Catalog() { delete[] tables; }

	Table_Catalog& operator=(const Table_Catalog& that)
	{
		if (this != &that)
		{
			if (this->tables != nullptr)
			{
				delete[] this->tables;
				this->tables = nullptr;
			}
			if (that.noOfTables != 0 && that.tables != nullptr)
			{
				this->noOfTables = that.noOfTables;
				this->tables = new Table[noOfTables];
				for (int i = 0; i < noOfTables; i++)
				{
					this->tables[i] = that.tables[i];
				}
			}
			else
			{
				this->noOfTables = 0;
			}
		}
		return *this;
	}

	operator int() const { return this->noOfTables; }

	bool operator==(const Table_Catalog& table_catalog) const
	{
		if (this->noOfTables != table_catalog.noOfTables)
		{
			return false;
		}

		for (int i = 0; i < noOfTables; i++)
		{
			if (this->tables[i] != table_catalog.tables[i])
			{
				return false;
			}
		}
		return true;
	}

	Table& operator[](const int index) const { return tables[index]; }

	void operator+=(const Table& t) { this->add_table(t); }

	bool operator!() const { return noOfTables == 0; }

	friend std::ostream& operator<<(std::ostream& out, const Table_Catalog& table_catalog)
	{
		table_catalog.print_tables();
		return out;
	}


	int return_index_of_table(const std::string& tableName) const
	{
		for (int i = 0; i < noOfTables; i++)
		{
			if (tables[i].getTableName() == tableName)
			{
				return i;
			}

			if (tables[i].has_synonym(tableName))
			{
				return i;
			}
		}
		return -1;
	}

	int drop_table(const std::string& tableName)
	{
		const int index = return_index_of_table(tableName);
		if (index == -1)
		{
			std::cout << "Table '" << tableName << "' does not exist!" << std::endl;
			return -1;
		}

		auto* auxCatalog = new Table_Catalog;
		for (int i = 0; i < noOfTables; i++)
		{
			if (i == index)
				continue;
			auxCatalog->add_table(tables[i]);
		}

		delete[] this->tables;
		this->tables = auxCatalog->getTables();
		this->setNoOfTables(noOfTables - 1);
		delete auxCatalog;
		return 0;
	}

	int add_table(const Table& newTable)
	{
		if (table_exists(newTable.getTableName()))
		{
			std::cout << "Table with this name already exists!";
			return -1;
		}
		for (int i = 0; i < noOfTables; i++)
		{
			if (newTable.has_synonym(tables[i].getTableName()))
			{
				std::cout << "Table with this name already exists!";
				return -2;
			}

			if (tables[i].has_synonym(newTable.getTableName()))
			{
				std::cout << "Table with this name already exists!";
				return -3;
			}
		}
		// create a new array of pointers to objects with updated size
		Table* newTables = new Table[noOfTables + 1];

		for (int i = 0; i < noOfTables; i++)
		{
			newTables[i] = tables[i];
		}

		// add the new table at the end of the new array
		newTables[noOfTables] = newTable;

		delete[] tables;
		tables = newTables;
		noOfTables++;
		return 0;
	}

	int getNoOfTables() const { return noOfTables; }

	bool table_exists(const std::string& tableName) const
	{
		if (noOfTables == 0 || tables == nullptr)
		{
			return false;
		}
		for (int i = 0; i < noOfTables; i++)
		{
			if (tables[i].getTableName() == tableName)
			{
				return true;
			}

			if (tables[i].has_synonym(tableName) == true)
			{
				return true;
			}
		}

		return false;
	}

	int getNumberOfColumns(const std::string& tableName) const
	{
		for (int i = 0; i < noOfTables; i++)
		{
			if (tables[i].getTableName() == tableName)
				return tables[i].getNoOfColumns();
		}
		return 0;
	}

	Table* getTables() const
	{
		Table* aux = new Table[noOfTables];
		for (int i = 0; i < noOfTables; i++)
		{
			aux[i] = tables[i];
		}
		return aux;
	}

	Table* getTable(const std::string& tableName) const
	{
		for (int i = 0; i < noOfTables; i++)
		{
			if (tables[i].getTableName() == tableName)
			{
				return &tables[i];
			}

			if (tables[i].has_synonym(tableName))
			{
				return &tables[i];
			}
		}

		return nullptr;
	}

	void setTables(const Table* newTables, int newNoOfTables)
	{
		for (int i = 0; i < newNoOfTables; i++)
		{
			if (newTables[i].getTableName().empty())
			{
				statusManager->print(StatusManager::Error, "Table names can't be empty!");
				return;
			}
		}

		delete[] this->tables;
		this->tables = new Table[newNoOfTables];
		this->noOfTables = newNoOfTables;
		for (int i = 0; i < newNoOfTables; i++)
		{
			this->tables[i] = newTables[i];
		}
	}

	void setNoOfTables(const int newNoOfTables)
	{
		if (newNoOfTables < 0)
		{
			statusManager->print(StatusManager::Error, "Number of tables has to be at least 0!");
			return;
		}
		this->noOfTables = newNoOfTables;
	}

	// print all the tables
	void print_tables() const
	{
		std::cout << std::endl;
		for (const Table* p = &tables[0]; p < &tables[noOfTables]; p++)
		{
			p->print_table(std::cout, p->getTableName());
			std::cout << std::endl << std::endl;
		}
	}

	bool synonym_exists(const std::string& synonym) const
	{
		if (noOfTables == 0 || tables == nullptr)
		{
			return false;
		}

		for (int i = 0; i < noOfTables; i++)
		{
			if (tables[i].has_synonym(synonym) == true)
			{
				return true;
			}
		}

		return false;
	}

	std::string getActualTableName(const std::string& option) const
	{
		for (int i = 0; i < noOfTables; i++)
		{
			const Table& currTable = tables[i];
			if (currTable.has_synonym(option))
			{
				return currTable.getTableName();
			}
		}

		return "";
	}
};

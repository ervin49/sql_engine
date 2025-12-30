#include <iostream>
#include <string>

#include "Index_Catalog.h"
#include "globals.h"
#define OFFSET 10

class Table
{
private:
	int noOfColumns;
	int noOfRows;
	int noOfIndexes;
	std::string tableName;
	std::string* columnNames;
	std::string** rows;
	std::string* columnTypes;
	std::string* indexNames;
	unsigned int* maxColumnLengths;

public:
	Table(int noOfColumns, const std::string& tableName, const std::string* columnTypes)
	{
		for (int i = 0; i < noOfColumns; i++)
		{
			const std::string type = columnTypes[i];
			if (type != "int" || type != "numeric" || type != "integer" || type != "float" || type != "text")
			{
				throw std::runtime_error("Incorrect column type!");
			}
		}
		this->noOfColumns = noOfColumns;
		this->tableName = tableName;
		this->noOfColumns = noOfColumns;
		noOfRows = 0;
		noOfIndexes = 0;
		columnNames = new std::string[this->noOfColumns];
		this->columnTypes = new std::string[this->noOfColumns];
		this->maxColumnLengths = new unsigned int[this->noOfColumns];
		for (int i = 0; i < noOfColumns; i++)
		{
			this->columnTypes[i] = columnTypes[i];
		}

		rows = nullptr;
		indexNames = nullptr;
	}


	Table(const Table& other)
	{
		noOfColumns = other.noOfColumns;
		columnNames = new std::string[noOfColumns];
		maxColumnLengths = new unsigned int[noOfColumns];
		this->columnTypes = new std::string[noOfColumns];
		tableName = other.tableName;
		noOfRows = other.noOfRows;
		noOfIndexes = other.noOfIndexes;
		for (int i = 0; i < noOfColumns; i++)
		{
			columnNames[i] = other.columnNames[i];
		}

		rows = new std::string*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			rows[i] = new std::string[noOfColumns];
		}

		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				rows[i][j] = other.rows[i][j];
			}
		}

		this->indexNames = new std::string[noOfIndexes];
		for (int i = 0; i < noOfIndexes; i++)
		{
			this->indexNames[i] = other.indexNames[i];
		}

		for (int i = 0; i < noOfColumns; i++)
		{
			this->columnTypes[i] = other.columnTypes[i];
		}

		for (int i = 0; i < noOfColumns; i++)
		{
			this->maxColumnLengths[i] = other.maxColumnLengths[i];
		}
	}


	Table()
	{
		this->rows = nullptr;
		this->indexNames = nullptr;
		this->columnNames = nullptr;
		this->columnTypes = nullptr;
		this->maxColumnLengths = nullptr;
		this->noOfRows = 0;
		this->noOfColumns = 0;
		this->noOfIndexes = 0;
		this->tableName = "";
	}

	Table(int noOfColumns, const std::string& tableName)
	{
		this->noOfColumns = noOfColumns;
		this->tableName = tableName;
		noOfRows = 0;
		noOfIndexes = 0;
		columnNames = new std::string[this->noOfColumns];
		columnTypes = new std::string[this->noOfColumns];
		maxColumnLengths = new unsigned int[this->noOfColumns];
		rows = nullptr;
		indexNames = nullptr;
	}

	bool operator==(Table const& table) const
	{
		if (this->noOfColumns != table.noOfColumns || this->noOfRows != table.noOfRows ||
			this->tableName != table.tableName)
			return false;

		for (int i = 0; i < noOfColumns; i++)
		{
			if (this->columnNames[i] != table.columnNames[i] || this->columnTypes[i] != table.columnTypes[i] ||
				this->maxColumnLengths[i] != table.maxColumnLengths[i])
			{
				return false;
			}
		}


		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				if (this->rows[i][j] != table.rows[i][j])
				{
					return false;
				}
			}
		}

		return true;
	}

	~Table()
	{
		delete[] columnNames;
		delete[] indexNames;
		for (int i = 0; i < noOfRows; i++)
		{
			delete[] rows[i];
		}
		delete[] rows;
	}

	Table& operator=(const Table& other)
	{
		if (this != &other)
		{
			if (columnNames != nullptr)
				delete[] columnNames;
			if (rows != nullptr)
			{
				for (int i = 0; i < noOfRows; i++)
					delete[] rows[i];
				delete[] rows;
			}

			this->tableName = other.tableName;
			this->noOfColumns = other.noOfColumns;
			this->noOfRows = other.noOfRows;
			this->noOfIndexes = other.noOfIndexes;

			this->columnNames = new std::string[noOfColumns];
			for (int i = 0; i < noOfColumns; i++)
			{
				this->columnNames[i] = other.columnNames[i];
			}

			this->rows = new std::string*[noOfRows];
			for (int i = 0; i < noOfRows; i++)
			{
				this->rows[i] = new std::string[noOfColumns];
				for (int j = 0; j < noOfColumns; j++)
				{
					this->rows[i][j] = other.rows[i][j];
				}
			}

			this->indexNames = new std::string[noOfIndexes];
			for (int i = 0; i < noOfIndexes; i++)
			{
				indexNames[i] = other.indexNames[i];
			}

			this->columnTypes = new std::string[noOfColumns];
			for (int i = 0; i < noOfColumns; i++)
			{
				columnTypes[i] = other.columnTypes[i];
			}

			this->maxColumnLengths = new unsigned int[noOfColumns];
			for (int i = 0; i < noOfColumns; i++)
			{
				maxColumnLengths[i] = other.maxColumnLengths[i];
			}
		}
		return *this;
	}

	operator int() const { return this->noOfRows; }

	Table& operator+=(std::string* row)
	{
		this->add_row(row);
		return *this;
	}

	// table -= column
	Table& operator-=(const std::string& columnName)
	{
		this->remove_column(columnName);
		return *this;
	}

	std::string* operator[](int index) const
	{
		if (index >= 0 && index < noOfColumns)
		{
			return &columnNames[index];
		}
		return nullptr;
	}

	bool operator!() const { return noOfRows == 0; }

	friend std::ostream& operator<<(std::ostream& out, const Table& table)
	{
		table.print_table(out);
		return out;
	}

	bool operator!=(const Table& table) const
	{
		if (*this == table)
		{
			return false;
		}
		return true;
	}

	bool is_column_type(const std::string& value, const std::string& columnType)
	{
		if (value[0] == '\'' && value[value.length() - 1] == '\'' && (columnType == "text" || columnType == "varchar"))
		{
			return true;
		}

		if ((columnType == "int" || columnType == "integer" || columnType == "numeric") && is_integer(value))
		{
			return true;
		}

		if (columnType == "float" && is_float(value))
		{
			return true;
		}

		return false;
	}

	bool is_integer(const std::string& str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			if (std::isdigit(str[i]) == false)
			{
				return false;
			}
		}
		return true;
	}

	bool is_float(const std::string& str)
	{
		bool hasDot = false;
		for (int i = 0; i < str.length(); i++)
		{
			if (std::isdigit(str[i]) == false && str[i] != '.')
			{
				return false;
			}
			if (str[i] == '.')
			{
				// can't have dot on first or last character
				if (i == 0 || i == str.length() - 1)
				{
					return false;
				}
				if (hasDot == false)
				{
					hasDot = true;
				}
				else
				{
					// if hasDot was already true, the string has 2 dots, so it's not valid
					return false;
				}
			}
		}
		return true;
	}

	std::string* getColumns()
	{
		auto* newColumns = new std::string[noOfColumns];
		for (int i = 0; i < noOfColumns; i++)
		{
			newColumns[i] = columnNames[i];
		}
		return newColumns;
	}

	void setRows(std::string** newRows, const int noOfRows, const int noOfColumns)
	{
		if (noOfRows < 0 || noOfColumns <= 0)
		{
			throw std::runtime_error("Invalid no. of rows or columns!");
		}

		for (int i = 0; i < this->noOfRows; i++)
		{
			delete[] rows[i];
		}
		delete[] rows;

		rows = new std::string*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			rows[i] = new std::string[noOfColumns];
			for (int j = 0; j < noOfColumns; j++)
			{
				rows[i][j] = newRows[i][j];
			}
		}

		this->noOfRows = noOfRows;
		this->noOfColumns = noOfColumns;
	}

	std::string** getRows()
	{
		std::string** newRows = new std::string*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			newRows[i] = new std::string[noOfColumns];
		}
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				newRows[i][j] = rows[i][j];
			}
		}
		return newRows;
	}

	void setNoOfRows(int newNoOfRows)
	{
		if (newNoOfRows < 0)
		{
			statusManager->print(StatusManager::Error, "Number of rows has to be at least 0!");
			return;
		}

		int oldNoOfRows = noOfRows;

		auto** newRows = new std::string*[newNoOfRows];
		for (int i = 0; i < newNoOfRows; i++)
		{
			newRows[i] = new std::string[noOfColumns];

			if (i < oldNoOfRows)
			{
				for (int j = 0; j < noOfColumns; j++)
				{
					newRows[i][j] = rows[i][j];
				}
			}
		}

		for (int i = 0; i < oldNoOfRows; i++)
		{
			delete[] rows[i];
		}
		delete[] rows;
		rows = newRows;
		noOfRows = newNoOfRows;
	}

	int getNoOfColumns() const { return noOfColumns; }

	int getNoOfRows() const { return noOfRows; }

	void setColumn(const int index, const std::string& columnName)
	{
		if (index < 0 || index >= noOfColumns)
		{
			throw std::runtime_error("Index out of range!");
		}

		columnNames[index] = columnName;
	}

	void setColumnType(const int index, const std::string& columnType)
	{
		if (index < 0 || index >= noOfColumns)
		{
			throw std::runtime_error("Index out of range!");
		}

		columnTypes[index] = columnType;
	}

	bool column_exists(const std::string& columnName) const
	{
		for (int i = 0; i < noOfColumns; i++)
		{
			if (columnNames[i] == columnName)
			{
				return true;
			}
		}

		return false;
	}

	int add_row(std::string newRow[])
	{
		for (int i = 0; i < noOfColumns; i++)
		{
			if (is_column_type(newRow[i], columnTypes[i]) == false)
			{
				statusManager->print(StatusManager::Error,
									 "Value \"" + newRow[i] + "\" does not match column type \"" + columnTypes[i] +
										 "\"!");
				return 1;
			}
			if (maxColumnLengths[i] < newRow[i].length())
			{
				statusManager->print(StatusManager::Error,
									 "Value \"" + newRow[i] + "\" in column \"" + columnNames[i] +
										 "\" exceeds max length! (Length: " + std::to_string(newRow[i].length()) +
										 ", Max: " + std::to_string(maxColumnLengths[i]) + ")");
				return 1;
			}
			if (columnTypes[i] == "text" || columnTypes[i] == "varchar")
			{
				newRow[i] = newRow[i].substr(1, newRow[i].length() - 2);
			}
		}

		std::string** newRows = new std::string*[noOfRows + 1];
		for (int i = 0; i <= noOfRows; i++)
		{
			newRows[i] = new std::string[noOfColumns];
		}

		// transfer the values from old rows array to the newRows array
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				newRows[i][j] = rows[i][j];
			}
		}

		for (int i = 0; i < noOfColumns; i++)
		{
			newRows[noOfRows][i] = newRow[i];
		}
		delete[] rows;
		rows = newRows;
		this->noOfRows++;
		return 0;
	}

	std::string getTableName() { return this->tableName; }

	void print_table(std::ostream& out) const
	{
		out << std::endl;

		if (!tableName.empty())
		{
			out << '[' << tableName << ']' << std::endl;
		}
		int* maxLengthOnColumn = new int[noOfColumns];

		// initialize all the max lengths with 0
		for (int i = 0; i < noOfColumns; i++)
		{
			maxLengthOnColumn[i] = 0;
		}

		// find the max length of each column
		for (int j = 0; j < noOfColumns; j++)
		{
			for (int i = 0; i < noOfRows; i++)
			{
				if (rows[i][j].length() > maxLengthOnColumn[j])
				{
					maxLengthOnColumn[j] = rows[i][j].length();
				}
			}
		}

		for (int i = 0; i < noOfColumns; i++)
		{
			if (columnNames[i].length() > maxLengthOnColumn[i])
			{
				maxLengthOnColumn[i] = columnNames[i].length();
			}
		}

		// find the sum of all lengths
		int sum = 0;
		for (int i = 0; i < noOfColumns; i++)
		{
			sum += maxLengthOnColumn[i];
			sum += OFFSET;
		}
		sum -= OFFSET;
		sum += 2 * (OFFSET / 2 - 1);

		// display the first line
		out << '+';
		for (int i = 0; i < noOfColumns; i++)
		{
			for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++)
			{
				out << '-';
			}
			if (i < noOfColumns - 1)
			{
				out << '+';
			}
		}
		out << '+' << std::endl << '|';

		for (int i = 0; i < OFFSET / 2 - 1; i++)
		{
			out << ' ';
		}

		// display the column names
		for (int i = 0; i < noOfColumns; i++)
		{
			out << columnNames[i];

			if (columnNames[i].length() == maxLengthOnColumn[i])
			{
				for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++)
				{
					out << ' ';
				}
				if (i < noOfColumns - 1)
				{
					out << '|';
				}
				for (int k = OFFSET / 2 + 1; k < OFFSET; k++)
				{
					out << ' ';
				}
				if (i == noOfColumns - 1)
				{
					out << ' ';
				}
			}
			else
			{
				for (int k = 0; k < maxLengthOnColumn[i] - columnNames[i].length(); k++)
				{
					out << ' ';
				}
				for (int k = 0; k < OFFSET / 2 && i < noOfColumns - 1; k++)
				{
					out << ' ';
				}
				if (i < noOfColumns - 1)
				{
					out << '|';
				}
				if (i == noOfColumns - 1)
				{
					for (int k = OFFSET / 2; k < OFFSET; k++)
					{
						out << ' ';
					}
				}
				else
				{
					for (int k = OFFSET / 2 + 1; k < OFFSET; k++)
					{
						out << ' ';
					}
				}
			}
		}

		out << '|' << std::endl << '+';

		// display the line below the column names
		for (int i = 0; i < noOfColumns; i++)
		{
			for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++)
			{
				out << '-';
			}
			if (i < noOfColumns - 1)
			{
				out << '+';
			}
		}

		out << '+' << std::endl;

		// display the rows
		for (int i = 0; i < noOfRows; i++)
		{
			out << '|';
			for (int k = 0; k < OFFSET / 2 - 1; k++)
			{
				out << ' ';
			}
			for (int j = 0; j < noOfColumns; j++)
			{
				out << rows[i][j];

				if (rows[i][j].length() == maxLengthOnColumn[j])
				{
					for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++)
					{
						out << ' ';
					}
					if (j < noOfColumns - 1)
					{
						out << '|';
					}
					if (j == noOfColumns - 1)
					{
						for (int k = 0; k < OFFSET / 2; k++)
						{
							out << ' ';
						}
					}
					else
					{
						for (int k = 0; k < OFFSET / 2 - 1; k++)
						{
							out << ' ';
						}
					}
				}
				else
				{
					for (int k = 0; k < maxLengthOnColumn[j] - rows[i][j].length(); k++)
					{
						out << ' ';
					}
					for (int k = 0; k < OFFSET / 2 && j < noOfColumns - 1; k++)
					{
						out << ' ';
					}
					if (j < noOfColumns - 1)
					{
						out << '|';
					}
					if (j == noOfColumns - 1)
					{
						for (int k = 0; k < OFFSET / 2; k++)
						{
							out << ' ';
						}
					}
					else
					{
						for (int k = 0; k < OFFSET / 2 - 1; k++)
						{
							out << ' ';
						}
					}
				}
			}
			out << '|' << std::endl;
		}

		// display the last line, only if we have at least a row
		if (noOfRows >= 1)
		{
			out << '+';
			for (int i = 0; i < noOfColumns; i++)
			{
				for (int j = 0; j < maxLengthOnColumn[i] + OFFSET - 1; j++)
				{
					out << '-';
				}
				if (i < noOfColumns - 1)
				{
					out << '+';
				}
			}
			out << '+' << std::endl << std::endl;
		}

		delete[] maxLengthOnColumn;
	}

	void setName(const std::string& name)
	{
		if (name.empty() || name.length() < 3)
		{
			statusManager->print(StatusManager::Error, "Name needs to be at least 3 characters long!");
			return;
		}

		this->tableName = name;
	}

	int return_index_of_column_by_name(const std::string& columnName) const
	{
		for (int i = 0; i < noOfColumns; i++)
		{
			if (columnNames[i] == columnName)
			{
				return i;
			}
		}

		return -1;
	}

	// we could delete this function
	void remove_column(const std::string& columnName)
	{
		if (noOfColumns <= 1)
		{
			statusManager->print(StatusManager::Error, "You need at least 1 column!");
		}
		auto* newColumns = new std::string[noOfColumns - 1];

		int k = 0;
		for (int i = 0; i < noOfColumns; i++)
		{
			if (columnNames[i] == columnName)
			{
				continue;
			}
			newColumns[k++] = columnNames[i];
		}
		for (int i = 0; i < noOfColumns - 1; i++)
		{
			columnNames[i] = newColumns[i];
		}
		this->setNoOfColumns(noOfColumns - 1);
		delete[] newColumns;
	}

	void setNoOfColumns(int newNoOfColumns)
	{
		if (newNoOfColumns <= 1)
		{
			statusManager->print(StatusManager::Error, "You need at least 1 column!");
		}
		this->noOfColumns = newNoOfColumns;
	}

	void remove_row(int index)
	{
		if (index == 0 && noOfRows == 1)
		{
			setNoOfRows(0);
			return;
		}

		for (int i = index; i < noOfRows - 1; i++)
		{
			for (int j = 0; j < noOfColumns; j++)
			{
				rows[i][j] = rows[i + 1][j];
			}
		}

		setNoOfRows(noOfRows - 1);
	}

	void index_table(const std::string& indexName) const
	{
		Index* index = indexCatalog->getIndex(indexName);
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = i + 1; j < noOfColumns; j++)
			{
			}
		}
		// to be written
		delete index;
	}

	int delete_from(const std::string& columnName, const std::string& nameOfValue)
	{
		int indexOfColumn;
		if ((indexOfColumn = return_index_of_column_by_name(columnName)) == -1)
		{
			statusManager->print(StatusManager::Error,
								 "Table \"" + tableName + "\" does not have the column \"" + columnName + "\"!");
			return -1;
		}

		for (int i = 0; i < noOfRows; i++)
		{
			if (rows[i][indexOfColumn] == nameOfValue)
			{
				remove_row(i);
				i--;
			}
		}
		return 0;
	}

	std::string* getIndexNames()
	{
		const auto newIndexNames = new std::string[noOfIndexes];
		for (int i = 0; i < noOfIndexes; i++)
		{
			newIndexNames[i] = indexNames[i];
		}

		return newIndexNames;
	}

	int getNoOfIndexes() const { return noOfIndexes; }

	void setIndexNames(std::string* indexNames, int noOfIndexes)
	{
		const auto newIndexNames = new std::string[noOfIndexes];
		for (int i = 0; i < noOfIndexes; i++)
		{
			newIndexNames[i] = indexNames[i];
		}

		delete[] this->indexNames;
		this->indexNames = newIndexNames;
		this->noOfIndexes = noOfIndexes;
	}

	void add_index(const std::string& indexName)
	{
		auto newIndexNames = new std::string[noOfIndexes + 1];
		for (int i = 0; i < noOfIndexes; i++)
		{
			newIndexNames[i] = indexNames[i];
		}

		newIndexNames[noOfIndexes] = indexName;

		delete[] indexNames;
		indexNames = newIndexNames;
		this->noOfIndexes++;
	}

	void setColumnNames(const std::string* columnNames, int noOfColumns)
	{
		if (columnNames == nullptr)
		{
			return;
		}

		this->noOfColumns = noOfColumns;
		delete[] this->columnNames;
		this->columnNames = new std::string[noOfColumns];

		for (int i = 0; i < noOfColumns; i++)
		{
			this->columnNames[i] = columnNames[i];
		}
	}

	void setColumnTypes(const std::string* columnTypes, int noOfColumns)
	{
		if (columnTypes == nullptr)
		{
			return;
		}

		if (this->columnTypes == columnTypes)
		{
			return;
		}

		delete[] this->columnTypes;
		this->noOfColumns = noOfColumns;
		this->columnTypes = new std::string[noOfColumns];

		for (int i = 0; i < noOfColumns; i++)
		{
			this->columnTypes[i] = columnTypes[i];
		}
	}

	std::string* getColumnTypes() { return columnTypes; }

	void setMaxColumnLengths(unsigned int* maxColumnLengths, int noOfColumns)
	{
		if (maxColumnLengths == nullptr)
		{
			return;
		}

		if (this->maxColumnLengths == maxColumnLengths)
		{
			return;
		}

		delete[] this->maxColumnLengths;
		this->noOfColumns = noOfColumns;
		this->maxColumnLengths = new unsigned int[noOfColumns];

		for (int i = 0; i < noOfColumns; i++)
		{
			this->maxColumnLengths[i] = maxColumnLengths[i];
		}
	}

	unsigned int* getMaxColumnLengths() const
	{
		auto aux = new unsigned int[noOfColumns];
		for (int i = 0; i < noOfColumns; i++)
		{
			aux[i] = maxColumnLengths[i];
		}
		return aux;
	}

	void print_indexes() const
	{
		Index* index;
		for (int i = 0; i < noOfIndexes; i++)
		{
			index = indexCatalog->getIndex(indexNames[i]);
			index->print_index();
		}
	}
	void remove_index(const std::string& indexName)
	{
		auto newIndexNames = new std::string[noOfIndexes - 1];
		int k = 0;
		for (int i = 0; i < noOfIndexes; i++)
		{
			if (indexNames[i] == indexName)
			{
				continue;
			}
			newIndexNames[k++] = indexNames[i];
		}

		delete[] indexNames;
		indexNames = newIndexNames;
		this->noOfIndexes--;
	}
};

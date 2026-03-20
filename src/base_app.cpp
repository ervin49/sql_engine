#include "base_app.hpp"
#include "index.hpp"
#include "index_collection.hpp"
#include "status_manager.hpp"
#include "table_collection.hpp"
#include "app_initializer.hpp"
#include "globals.hpp"
#include <fstream>

BaseApp::BaseApp(){
    noOfWords = 0;
    words = nullptr;
}

BaseApp::~BaseApp()
{
    delete[] this->words;
    delete parser;
}

void BaseApp::listTables(){
    int noOfTables = tableCollection->getNoOfTables();
    if (noOfTables == 0)
    {
        std::cout << "There aren't any tables!" << std::endl;
        return;
    }

    const std::string msg = "Current tables:";
    std::cout << msg << std::endl;
    for (int i = 0; i < msg.length(); i++)
    {
        std::cout << '-';
    }
    std::cout << std::endl;

    auto tables = tableCollection->getTables();
    for (int i = 0; i < noOfTables; i++)
    {
        const Table& currTable = tables[i];
        const int noOfSynonyms = currTable.getNoOfSynonyms();
        const std::string* synonyms = currTable.getSynonyms();

        std::cout << i + 1 << ". Table: " << currTable.getTableName();
        if (noOfSynonyms > 0)
        {
            std::cout << '[';
            for (int j = 0; j < noOfSynonyms; j++)
            {
                const std::string& synonym = synonyms[j];
                std::cout << synonym;
                if (j < noOfSynonyms - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ']';
        }

        const int noOfIndexes = currTable.getNoOfIndexes();
        const std::string* indexes = currTable.getIndexNames();
        const std::string* columnsOfIndexes =
            indexCollection->getColumnsOfIndexesByTableName(currTable.getTableName());
        if (noOfIndexes > 0)
        {
            std::cout << std::endl << "   Indexes: " << std::endl;
            for (int j = 0; j < noOfIndexes; j++)
            {
                std::cout << "     - '" << indexes[j] << "' on column '" << columnsOfIndexes[j] << "'";
                if (j < noOfIndexes - 1)
                {
                    std::cout << std::endl;
                }
            }
        }
        delete[] indexes;
        delete[] columnsOfIndexes;
        if (i < noOfTables - 1)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    delete[] tables;
}

BaseApp::BaseApp(const std::string* words, const int noOfWords)
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

void BaseApp::parseCommand() {
    std::string firstWord = words[0];
    if (firstWord == "create")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        std::string secondWord = words[1];
        if (secondWord == "table")
        {
            if (hasInvalidWordCount(2) || hasInvalidWordCount(3))
            {
                return;
            }

            if (words[3] == "if")
            {
                createTableWithIfNotExists();
            }
            else
            {
                createTableWithoutIfNotExists();
            }
        }
        else if (secondWord == "index")
        {
            if (hasInvalidWordCount(2))
            {
                return;
            }
            if (words[3] == "if")
            {
                createIndexWithIfNotExists();
            }
            else
            {
                createIndexWithoutIfNotExists();
            }
        }
        else if (secondWord == "synonym")
        {
            if (hasInvalidWordCount(2))
            {
                return;
            }
            createSynonym();
        }
        else
        {
            statusManager->print(StatusManager::Error, "Wrong statement! You can create a table or an index.");
        }
    }
    else if (firstWord == "drop")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        std::string secondWord = words[1];
        if (secondWord == "table")
        {
            if (hasInvalidWordCount(2))
            {
                return;
            }
            dropTable();
        }
        else if (secondWord == "index")
        {
            if (hasInvalidWordCount(2))
            {
                return;
            }
            dropIndex();
        }
        else
        {
            statusManager->print(StatusManager::Error, "Wrong statement! You can drop a table or an index.");
        }
    }
    else if (firstWord == "display")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        displayTable();
    }
    else if (firstWord == "insert")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        insertIntoTable();
    }
    else if (firstWord == "select")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        selectFrom();
    }
    else if (firstWord == "delete")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        deleteFrom();
    }
    else if (firstWord == "update")
    {
        if (hasInvalidWordCount(1))
        {
            return;
        }
        updateTable();
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
                if (tableCollection->getNoOfTables() == 0)
                {
                    statusManager->print(StatusManager::Error, "There are no tables!");
                }
                else
                {
                    std::cout << "The tables are:" << std::endl;
                    tableCollection->printTables();
                }
            }
            else if (words[1] == "indexes")
            {
                indexCollection->printIndexes();
            }
        }
    }
    else if (firstWord == "import")
    {
        if (hasInvalidWordCount(1) || hasInvalidWordCount(2))
        {
            return;
        }
        importTable();
    }
    else if (firstWord == "list")
    {
        listTables();
    }
    else
    {
        statusManager->print(StatusManager::Error, "Command is wrong! Please enter a new command. ");
    }

}

BaseApp::BaseApp(const BaseApp& a)
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
        this->parser = new Parser(*a.parser);
    }
    else
    {
        this->parser = new Parser;
    }
}

BaseApp& BaseApp::operator=(const BaseApp& a)
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

BaseApp::operator int() const { return this->noOfWords; }

bool BaseApp::operator!() const { return noOfWords == 0; }

std::string BaseApp::operator[](int index) const
{
    if (index >= 0 && index < noOfWords)
    {
        return words[index];
    }
    return "";
}

static void print_application() { std::cout << "Current running application: " << "de schimbat aici" << std::endl; }


std::ostream& operator<<(std::ostream& out, const BaseApp& application)
{
    application.printApplication();
    return out;
}

bool BaseApp::operator==(const BaseApp& application) const
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


void BaseApp::setQuery(std::string* words, int noOfWords, const std::string& s)
{
    this->s = s;
    this->words = words;
    this->noOfWords = noOfWords;
}


bool BaseApp::hasInvalidWordCount(const int wordCount) const
{
    if (this->noOfWords == wordCount)
    {
        statusManager->print(StatusManager::Error, "Incomplete input!");
        return true;
    }
    return false;
}

void BaseApp::insertIntoTable() const
{
    if (words[1] != "into" || words[3] != "values")
    {
        statusManager->print(StatusManager::Error, "Syntax error!");
        return;
    }

    const std::string tableName = words[2];
    if (!tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
        return;
    }

    int noOfFields;
    std::string* inputFields = parser->parseColumn(words[4], noOfFields);

    if (noOfFields != tableCollection->getNumberOfColumns(tableName))
    {
        statusManager->print(StatusManager::Error, "Invalid input!");
        return;
    }

    auto table = tableCollection->getTable(tableName);
    if (table->addRow(inputFields, noOfFields) == 0)
    {
        writeTableToFile(*table);
        int noOfRows = table->getNoOfRows();
        statusManager->print(StatusManager::Success,
                "Inserted successfully! (Total number of rows: " + std::to_string(noOfRows) + ')');
    }
}


void BaseApp::createTable(const std::string& tableName) const
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
        const auto fields = parser->parseColumn(columns[j], noOfFields);

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
        if (table->isInteger(fields[2]))
        {
            maxColumnLengths[j] = stoi(fields[2]);
        }
        else
        {
            statusManager->print(
                    StatusManager::Error,
                    "The value '" + fields[2] +
                    "' is invalid! You need to specify a positive integer for the attribute size.");
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

    if (tableCollection->addTable(*table) == 0)
    {
        writeTableToFile(*table);
        statusManager->print(StatusManager::Success, "Table '" + tableName + "' created successfully!");
    }

    delete table;
    delete[] columnTypes;
    delete[] columnNames;
    delete[] columns;
}

void BaseApp::writeTableToFile(const Table& table) const
{
    // open the file(or create it if it doesn't exist already)
    std::string tableName = table.getTableName();
    DIR* dir = opendir("./tables/");
    if (!dir)
    {
        system("mkdir ./tables"); // only works on linux
    }
    else
    {
        closedir(dir);
    }
    std::ofstream file("./tables/" + tableName + ".bin", std::ios::binary);

    // get all the variables from the table
    int noOfColumns = table.getNoOfColumns(), noOfRows = table.getNoOfRows(), noOfIndexes = table.getNoOfIndexes(),
        noOfSynonyms = table.getNoOfSynonyms();
    std::string* columns = table.getColumns();
    std::string* synonyms = table.getSynonyms();
    std::string** rows = table.getRows();
    std::string* indexNames = table.getIndexNames();
    std::string* columnTypes = table.getColumnTypes();
    unsigned int* maxColumnLengths = table.getMaxColumnLengths();

    // write into the file
    file.write(reinterpret_cast<char*>(&noOfColumns), sizeof(int));
    file.write(reinterpret_cast<char*>(&noOfRows), sizeof(int));
    file.write(reinterpret_cast<char*>(&noOfIndexes), sizeof(int));
    file.write(reinterpret_cast<char*>(&noOfSynonyms), sizeof(int));

    // have to write the strings lengths
    // before the strings themselves
    unsigned int len = tableName.size();
    file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
    file.write(tableName.data(), len);

    // write the columns
    for (int i = 0; i < noOfColumns; i++)
    {
        len = columns[i].length();
        file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
        file.write(columns[i].data(), len);

        len = columnTypes[i].length();
        file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
        file.write(columnTypes[i].data(), len);

        file.write(reinterpret_cast<char*>(&maxColumnLengths[i]), sizeof(unsigned int));
    }

    // write the rows
    // if this function was called from create table
    // then noOfRows will be 0 so it won't enter the for loop
    for (int i = 0; i < noOfRows; i++)
    {
        for (int j = 0; j < noOfColumns; j++)
        {
            len = rows[i][j].size();
            file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
            file.write(rows[i][j].data(), len);
        }
    }

    // write the indexes
    // same thing as the rows
    for (int i = 0; i < noOfIndexes; i++)
    {
        len = indexNames[i].size();
        file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
        file.write(indexNames[i].data(), len);

        std::string columnNameOfIndex = indexCollection->getIndex(indexNames[i])->getColumnName();
        len = columnNameOfIndex.length();
        file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
        file.write(columnNameOfIndex.data(), len);
    }

    // write the synonyms
    for (int i = 0; i < noOfSynonyms; i++)
    {
        len = synonyms[i].size();
        file.write(reinterpret_cast<char*>(&len), sizeof(unsigned int));
        file.write(synonyms[i].data(), len);
    }

    for (int i = 0; i < noOfRows; i++)
    {
        delete[] rows[i];
    }
    delete[] rows;
    delete[] columns;
    delete[] columnTypes;
    delete[] maxColumnLengths;
    delete[] indexNames;
    delete[] synonyms;
}

void BaseApp::createTableWithoutIfNotExists() const
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

    if (tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' already exists!");
        return;
    }

    createTable(tableName);
}

void BaseApp::createTableWithIfNotExists() const
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
        statusManager->print(StatusManager::Error, "Parse error near '" + words[4] + "'!");
        return;
    }
    if (words[5] != "exists")
    {
        statusManager->print(StatusManager::Error, "Parse error near '" + words[5] + "'!");
        return;
    }
    if (tableCollection->tableExists(tableName))
    {
        return;
    }
    createTable(tableName);
}

void BaseApp::createIndexWithoutIfNotExists() const
{
    std::string indexName;
    for (int i = 0; i < words[2].length(); i++)
    {
        indexName += tolower(words[2][i]);
    }

    if (indexCollection->indexExists(indexName))
    {
        statusManager->print(StatusManager::Error, "Index already exists!");
        return;
    }

    createIndex();
}

void BaseApp::createIndexWithIfNotExists() const
{
    if (noOfWords < 6)
    {
        statusManager->print(StatusManager::Error, "Incomplete input!");
    }
    if (words[3] != "not")
    {
        statusManager->print(StatusManager::Error, "Parse error near '" + words[3] + "'!");
        return;
    }
    if (words[4] != "exists")
    {
        statusManager->print(StatusManager::Error, "Parse error near '" + words[4] + "'!");
        return;
    }
    if (words[6] != "on")
    {
        statusManager->print(StatusManager::Error, "Parse error near '" + words[6] + "'!");
        return;
    }

    std::string indexName;
    for (int i = 0; i < words[5].length(); i++)
    {
        indexName += tolower(words[5][i]);
    }

    if (indexCollection->indexExists(indexName))
    {
        return;
    }
    createIndex();
}

void BaseApp::createIndex() const
{
    int indexOfLastWord = noOfWords - 1;
    if (noOfWords < 6)
    {
        statusManager->print(StatusManager::Error, "Incomplete input!");
        return;
    }

    std::string indexName = words[noOfWords - 4];

    std::string tableName = words[indexOfLastWord - 1];
    if (!tableCollection->tableExists(tableName))
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

    auto table = tableCollection->getTable(tableName);
    if (!table->columnExists(columnName))
    {
        statusManager->print(StatusManager::Error, "Column " + columnName + " does not exist!");
        return;
    }

    if (indexCollection->setIndex(indexName, columnName) == 0)
    {
        Index* index = new Index(indexName, tableName, columnName);
        indexCollection->addIndex(*index);
        table->addIndex(indexName);
        writeTableToFile(*table);
        indexCollection->writeIndexCollectionToFile();
        statusManager->print(StatusManager::Success, "Index '" + indexName + "' created successfully!");
        delete index;
    }
}

void BaseApp::dropTable() const
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

    const std::string actualTableName = tableCollection->getActualTableName(tableName);
    if (remove(("./tables/" + actualTableName + ".bin").data()) != 0)
    {
        statusManager->print(StatusManager::Error, "Could not remove file '" + actualTableName + ".bin'!");
    }
    if (tableCollection->dropTable(tableName) == 0)
    {
        statusManager->print(StatusManager::Success, "Table '" + aux + "' dropped successfully!");
    }
}

void BaseApp::dropIndex() const
{
    std::string indexName, aux = words[2];
    for (int i = 0; i < words[2].length(); i++)
    {
        indexName += tolower(words[2][i]);
    }

    auto index = indexCollection->getIndex(indexName);
    if (index == nullptr)
    {
        statusManager->print(StatusManager::Error, "Index '" + indexName + "' does not exist!");
        return;
    }
    if (indexCollection->dropIndex(indexName) == 0)
    {
        const std::string tableNameOfIndex = indexCollection->getIndex(indexName)->getTableName();
        const auto table = tableCollection->getTable(tableNameOfIndex);
        table->removeIndex(indexName);
        statusManager->print(StatusManager::Success, "Index '" + aux + "' dropped successfully!");
        indexCollection->writeIndexCollectionToFile();
    }
}

void BaseApp::displayTable() const
{
    if (noOfWords != 3)
    {
        statusManager->print(StatusManager::Error,
                "Argument count mismatch: expected 3, got " + std::to_string(noOfWords) + "!");
        return;
    }

    std::string tableName = words[2];
    if (tableCollection->tableExists(tableName))
    {
        tableCollection->getTable(tableName)->printTable(std::cout, tableName);
        writeSelectToFile(*tableCollection->getTable(tableName), tableName);
    }
    else
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
    }
}

void BaseApp::updateTable() const
{
    std::string tableName = words[1];

    if (noOfWords != 10)
    {
        statusManager->print(StatusManager::Error,
                "Invalid number of tokens, expected 9, got " + std::to_string(noOfWords) + "!");
        return;
    }

    if (!tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
        return;
    }
    if (words[2] != "set")
    {
        statusManager->print(StatusManager::Error,
                "Syntax Error: Expected 'SET' at position 2, but found '" + words[2] + "'.");
        return;
    }

    auto table = tableCollection->getTable(tableName);
    std::string setColumnName = words[3];

    if (!table->columnExists(setColumnName))
    {
        statusManager->print(StatusManager::Error,
                "Table '" + tableName + "' does not have column '" + setColumnName + "'!");
        return;
    }
    if (words[4] != "=")
    {
        statusManager->print(StatusManager::Error,
                "Syntax Error: Expected '=' at position 4, but found '" + words[4] + "'.");
        return;
    }
    if (words[8] != "=")
    {
        statusManager->print(StatusManager::Error,
                "Syntax Error: Expected '=' at position 8, but found '" + words[8] + "'.");
        return;
    }

    std::string whereColumnName = words[7];
    if (!table->columnExists(whereColumnName))
    {
        statusManager->print(StatusManager::Error,
                "Table '" + tableName + "' does not have column '" + whereColumnName + "'!");
        return;
    }

    int setIndex = table->returnIndexOfColumnByName(setColumnName);
    std::string setValue = words[5];
    int whereIndex = table->returnIndexOfColumnByName(whereColumnName);
    std::string whereValue = words[9];

    std::string columnType = table->getColumnTypes()[setIndex];

    if (columnType == "integer")
    {
        if (!table->isInteger(setValue))
        {
            statusManager->print(StatusManager::Error,
                    "Type mismatch! Column '" + setColumnName + "' is INTEGER, but value '" +
                    setValue + "' is not.");
            return;
        }
    }
    else if (columnType == "string" || columnType == "text" || columnType == "varchar")
    {
        if (table->isInteger(setValue))
        {
            statusManager->print(StatusManager::Error,
                    "Type mismatch! Column '" + setColumnName + "' is VARCHAR, but value '" +
                    setValue + "' is numeric.");
            return;
        }
    }

    std::string** tableRows = table->getRows();
    int count = 0;
    for (int i = 0; i < table->getNoOfRows(); i++)
    {
        if (tableRows[i][whereIndex] == whereValue)
        {
            tableRows[i][setIndex] = setValue;
            count++;
        }
    }

    if (count > 0)
    {
        table->setRows(tableRows, table->getNoOfRows(), table->getNoOfColumns());
        writeTableToFile(*table);
        std::cout << "Value set to: " << setValue << std::endl;
        statusManager->print(StatusManager::Success,
                "Updated table successfully! (" + std::to_string(count) + " rows affected)");
    }
    else
    {
        statusManager->print(StatusManager::Error, "No rows matched the WHERE condition.");
    }
}

void BaseApp::deleteFrom() const
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
        statusManager->print(StatusManager::Error, "Parse error near '" + words[1] + "'!");
        return;
    } // de adaugat near si aici
    if (words[3] != "where")
    {
        statusManager->print(StatusManager::Error, "Parse error near '" + words[3] + "'!");
        return;
    }

    std::string tableName = words[2];
    std::string columnName = words[4];
    std::string value = words[6];
    if (!tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
        return;
    }

    Table* table = tableCollection->getTable(tableName);
    if (table->deleteFrom(columnName, value) == 0)
    {
        statusManager->print(StatusManager::Success, "Row deleted successfully!");
        writeTableToFile(*table);
    }
}

void BaseApp::writeSelectToFile(const Table& table, const std::string& synonymName) const
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
    table.printTable(file, synonymName);
}

void BaseApp::selectFrom() const
{
    std::string tableName = words[3];

    if (!tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
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
            tableCollection->getTable(tableName)->printTable(std::cout, tableName);
            writeSelectToFile(*tableCollection->getTable(tableName), tableName);
        }
        else
        {
            if (words[4] != "where")
            {
                statusManager->print(StatusManager::Error,
                        "Syntax Error: Expected keyword 'WHERE', but found '" + words[4] +
                        "' instead.");
            }
            else if (words[6] != "=")
            {
                statusManager->print(StatusManager::Error,
                        "Syntax Error: Expected symbol '=', but found '" + words[6] + "' instead.");
            }
            auto originalTable = tableCollection->getTable(tableName);
            std::string columnName = words[noOfWords - 3];
            // value that we search for
            std::string value = words[noOfWords - 1];
            bool found = false;
            int index = originalTable->returnIndexOfColumnByName(columnName);
            const int noOfRows = originalTable->getNoOfRows();
            const int noOfColumns = originalTable->getNoOfColumns();
            std::string** rowsOfOriginalTable = originalTable->getRows();
            auto* tableWithSelectedRows = new Table(noOfColumns, "");
            tableWithSelectedRows->setMaxColumnLengths(originalTable->getMaxColumnLengths(), noOfColumns);
            tableWithSelectedRows->setColumnTypes(originalTable->getColumnTypes(), noOfColumns);

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
                        "No matching values for: '" + value + "' in column: '" + columnName + "'!");
            }
            else
            {
                tableWithSelectedRows->printTable(std::cout, tableName);
                writeSelectToFile(*tableWithSelectedRows, tableName);
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

    const auto originalTable = tableCollection->getTable(tableName);
    for (int i = 0; i < noOfSelectedColumns; i++)
    {
        if (originalTable->columnExists(selectedColumns[i]) == false)
        {
            statusManager->print(StatusManager::Error,
                    "Column '" + selectedColumns[i] + "' does not exist in table '" + tableName +
                    "'!");
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
        tableWithSelectedColumnsOnly->printTable(std::cout, tableName);
        writeSelectToFile(*tableWithSelectedColumnsOnly, tableName);
        delete tableWithSelectedColumnsOnly;
        delete[] selectedColumns;
        return;
    } // for select with no where clause

    if (words[4] != "where")
    {
        statusManager->print(StatusManager::Error,
                "Syntax Error: Expected keyword 'WHERE', but found '" + words[4] + "' instead.");
    }
    else if (words[6] != "=")
    {
        statusManager->print(StatusManager::Error,
                "Syntax Error: Expected symbol '=', but found '" + words[6] + "' instead.");
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
    int index = tableWithSelectedRows->returnIndexOfColumnByName(columnName);

    for (int i = 0; i < noOfRows; i++)
    {
        for (int j = 0; j < noOfSelectedColumns; j++)
        {
            if (rowsOfNewTable[i][j] == value && j == index)
            {
                // it matches what we are searching for
                found = true;
                tableWithSelectedRows->addRow(rowsOfNewTable[i], noOfSelectedColumns);
            }
        }
    }

    if (!found)
    {
        statusManager->print(StatusManager::Error,
                "No matching values for: '" + value + "' in column: '" + columnName + "'!");
    }
    else
    {
        tableWithSelectedRows->printTable(std::cout, tableName);
        writeSelectToFile(*tableWithSelectedRows, tableName);
    }

    // delete all dynamically allocated variables
    delete tableWithSelectedColumnsOnly;
    delete tableWithSelectedRows;
    delete[] selectedColumns;
}

void BaseApp::importTable() const
{
    std::string tableName = words[1];
    std::string fileName = words[2];

    if (!tableCollection->tableExists(tableName))
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
        return;
    }


    Table* table = tableCollection->getTable(tableName);

    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open())
    {
        statusManager->print(StatusManager::Error, "File '" + fileName + "' could not open!");
        return;
    }

    int noOfColumns = table->getNoOfColumns();
    int noOfRows = 0;
    int importedRows = 0;


    while (std::getline(file, line))
    {
        noOfRows++;
        int noOfFields;

        // checking the delimiter (it should be ',')
        if (noOfColumns > 1)
        {
            int noOfCommas = 0;
            bool isText = false;
            for (int i = 0; i < line.length(); i++)
            {
                // ignore ',' if it is in between ' '
                if (line[i] == '\'')
                {
                    isText = !isText;
                }
                if (isText == false && line[i] == ',')
                {
                    noOfCommas++;
                }
            }
            if (noOfCommas != noOfColumns - 1)
            {
                statusManager->print(StatusManager::Error, "Invalid delimiter!");
                continue;
            }
        }
        std::string* fields = this->parser->parseColumn(line, noOfFields);

        bool addRow = true;


        if (noOfColumns != noOfFields)
        {
            statusManager->print(StatusManager::Error,
                    "Invalid column number on row " + std::to_string(noOfRows - 1));
            addRow = false;
        }

        if (addRow && table->addRow(fields, noOfColumns) == 0)
        {
            importedRows++;
        }

        delete[] fields;
    }
    file.close();

    if (importedRows > 0)
    {
        writeTableToFile(*table);
        statusManager->print(StatusManager::Success, "Import completed successfully!");
    }
}


void BaseApp::parseCommandsFromFiles(int argc, char** argv)
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
    AppInitializer initializer;
    initializer.run();

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
                    words = parser->parseWithBracketsSelect(noOfWords);
                }
                else
                {
                    if (tolower(word[0]) == 'i')
                    {
                        words = parser->parseWithBrackets(noOfWords, true);
                    }
                    else
                    {
                        words = parser->parseWithBrackets(noOfWords);
                    }
                }
            }
            else if (numberOfParentheses[0] == 0 && numberOfParentheses[1] == 0)
            {
                words = parser->parseWithoutBrackets(noOfWords);
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
            parseCommand();
            delete[] numberOfParentheses;
        }
    }
}

void BaseApp::parseCommands()
{
    std::cout << "(!) Note: You can exit this program anytime by typing 'q' / 'quit' or" << std::endl
        << "return to the main menu by typing 'menu'." << std::endl;
    while (true)
    {
        std::cout << "> ";
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
                words = parser->parseWithBracketsSelect(noOfWords);
            }
            else
            {
                if (tolower(word[0]) == 'i')
                {
                    words = parser->parseWithBrackets(noOfWords, true);
                }
                else
                {
                    words = parser->parseWithBrackets(noOfWords);
                }
            }
        }
        else if (numberOfParentheses[0] == 0 && numberOfParentheses[1] == 0)
        {
            words = parser->parseWithoutBrackets(noOfWords);
        }
        else
        {
            statusManager->print(StatusManager::Error, "Invalid number of parentheses!");
            delete[] numberOfParentheses;
            continue;
        }

        setQuery(words, noOfWords, parser->getString());
        if (noOfWords == 1)
        {
            if (words[0] == "exit" || words[0] == "quit" || words[0] == "q")
            {
                exit(0);
            }
            if (words[0] == "menu")
            {
                return;
            }
        }
        parseCommand();
        delete[] numberOfParentheses;
    }
}

void BaseApp::printApplication() const{
    std::cout<<"Current running application contains string: '"<< s <<"'.";
}

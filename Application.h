#pragma once
#include <cstring>

#include "StatusManager.h"
#include "Table_Catalog.h"
#include "globals.h"
#include "Index.h"
#include "Index_Catalog.h"
#include "Parser.h"


class Application
{
private:
    Parser* parser = new Parser;
    std::string* words;
    int noOfWords = 0;
    std::string s;

public:
    Application()
    {
        this->words = nullptr;
        this->noOfWords = 0;
    }

    ~Application()
    {
        delete[] this->words;
        delete parser;
    }

    void print_application() const
    {
        std::cout << "Current running application: " << "de schimbat aici" << std::endl;
    }

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

    void parse_command()
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
                        statusManager->print(StatusManager::Error, "ERROR: There are no tables!");
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
        else
        {
            statusManager->print(StatusManager::Error, "Command is wrong. Please enter a new command. ");
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
        std::string tableName = words[2];
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

        Table* table = tableCatalog->getTable(tableName);
        table->add_row(inputFields);
        statusManager->print(StatusManager::Success, "Inserted successfully!");
    }


    void create_table(std::string tableName) const
    {
        int indexOfLastWord = noOfWords - 1;
        if (words[indexOfLastWord][0] != '(' || words[indexOfLastWord][1] != '(' || words[indexOfLastWord][
            words[indexOfLastWord].length() - 2] != ')')
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

        for (int i = 0; i < noOfColumns - 1; i++)
        {
            for (int j = i + 1; j < noOfColumns; j++)
            {
                if (columns[i] == columns[j])
                {
                    statusManager->print(StatusManager::Error, "You can't enter two identical column names!");
                    return;
                }
            }
        }

        int noOfFields;
        auto* table = new Table(noOfColumns, tableName);
        for (int j = 0; j < noOfColumns; j++)
        {
            std::string* fields = parser->parse_column(columns[j], noOfFields);

            if (noOfFields != 4)
            {
                statusManager->print(StatusManager::Error, "Every column should contain exactly 4 fields!");
                delete table;
                return;
            }
            table->setColumn(j, fields[0]);
        }

        if (tableCatalog->add_table(*table) == 0)
        {
            statusManager->print(StatusManager::Success, "Table \"" + tableName + "\" created successfully!");
        }
        delete table;
    }

    void create_table_without_if_not_exists() const
    {
        std::string tableName;
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
        } //de adaugat parse error cu "near"
        if (words[4] != "exists")
        {
            statusManager->print(StatusManager::Error, "Parse error near \"" + words[4] + "\"!");
            return;
        }
        if (words[6] != "on")
        {
            statusManager->print(StatusManager::Error, "Parse error near \"" + words[5] + "\"!");
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
        std::string indexName = words[noOfWords - 3];
        std::string tableName = words[indexOfLastWord - 1];
        if (!tableCatalog->table_exists(tableName))
        {
            statusManager->print(StatusManager::Error,
                                 "Table " + tableName + " does not exist! Cannot create index.");
            return;
        }

        if (words[indexOfLastWord][0] != '(' || words[indexOfLastWord][words[indexOfLastWord].length() - 1] != ')')
        {
            statusManager->print(StatusManager::Error,
                                 "Invalid parentheses!");
            return;
        }

        std::string columnName;
        for (int i = 1; i < words[indexOfLastWord].length() - 1; i++)
        {
            columnName += tolower(words[indexOfLastWord][i]);
        }

        Table* table = tableCatalog->getTable(tableName);
        if (!table->column_exists(columnName))
        {
            statusManager->print(StatusManager::Error,
                                 "Column " + columnName + " does not exist!");
            return;
        }

        if (indexCatalog->setIndex(indexName, columnName) == 0)
        {
            Index* index = new Index(indexName, tableName, columnName);
            indexCatalog->add_index(*index);
            statusManager->print(StatusManager::Success,
                                 "Index \"" + indexName + "\" created successfully!");
            delete index;
        }
    }

    void drop_table() const
    {
        std::string tableName, aux = words[2];
        for (int i = 0; i < words[2].length(); i++)
        {
            tableName += tolower(words[2][i]);
        }

        if (tableCatalog->drop_table(tableName) == 0)
        {
            statusManager->print(StatusManager::Success, "Dropped table \"" + aux + "\" successfully!");
        }
    }

    void drop_index() const
    {
        std::string indexName = "", aux = words[2];
        for (int i = 0; i < words[2].length(); i++)
        {
            indexName += tolower(words[2][i]);
        }

        if (indexCatalog->drop_index(indexName) == 0)
        {
            statusManager->print(StatusManager::Success, "Dropped index \"" + aux + "\" successfully!");
        }
    }

    void display_table() const
    {
        std::string tableName = words[2];
        if (tableCatalog->table_exists(tableName))
        {
            tableCatalog->getTable(tableName)->print_table();
        }
        else
        {
            statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
        }
    }

    void update_table() const
    {
        std::string tableName = words[1];
        if (!tableCatalog->table_exists(tableName))
        {
            statusManager->print(StatusManager::Error, "Table \"" + tableName + "\" does not exist!");
            return;
        }

        auto* table = tableCatalog->getTable(tableName);
        std::string setColumnName = words[3];
        if (!table->column_exists(setColumnName))
        {
            statusManager->print(StatusManager::Error,
                                 "Table \"" + tableName + "\" does not have column \"" + setColumnName + "\"!");
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
        table->setRows(tableRows);
        statusManager->print(StatusManager::Success, "Updated table successfully!");
        delete table;
    }

    void delete_from() const
    {
        if (noOfWords < 6)
        {
            statusManager->print(StatusManager::Error, "Incomplete input!");
        }
        if (words[1] != "from")
        {
            statusManager->print(StatusManager::Error, "Parse error near \"" + words[1] + "\"!");
            return;
        } //de adaugat near si aici
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
        }
    }

    void select_from() const
    {
        std::string tableName;
        if (words[noOfWords - 2][0] == '=')
        {
            tableName = words[noOfWords - 5];
        }
        else
        {
            tableName = words[noOfWords - 1];
        }
        debug(tableName);

        int poz = 0;
        while (poz < s.length() && s[poz] != '(')
        {
            poz++; //dupa asta i se va afla pe prima paranteza
        }

        int noOfColumns = 1;
        for (int i = poz + 1; i < s.length() && s[i] != ')'; i++)
        {
            if (s[i] == ' ' && s[i - 1] != ' ' && i >= 1)
            {
                noOfColumns++;
            }
        }

        auto* selectedColumns = new std::string[noOfColumns];
        int columnIndex = 0;
        for (int i = poz; i < s.length() && s[i] != ')'; i++)
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

        std::string checkForAll;
        for (int i = 0; i < 3; i++)
        {
            checkForAll[i] += tolower(selectedColumns[0][i]);
        }

        if (noOfColumns == 1 && checkForAll == "all")
        {
            tableCatalog->getTable(tableName)->print_table();
            return;
        }

        //we create a new table only with the columns we need
        auto* tableWithSelectedColumns = new Table(noOfColumns, tableName + '.');
        //the dot is so we don't repeat table names
        auto* originalTable = tableCatalog->getTable(tableName);

        //we set the number of rows of new table to be the same as the original table
        tableWithSelectedColumns->setNoOfRows(originalTable->getNoOfRows());
        auto* columnsOfOriginalTable = originalTable->getColumns();
        int noOfRows = originalTable->getNoOfRows();
        std::string** rowsOfOriginalTable = originalTable->getRows();
        std::string** rowsOfNewTable = tableWithSelectedColumns->getRows();

        //we set the column names
        for (int i = 0; i < noOfColumns; i++)
        {
            tableWithSelectedColumns->setColumn(i, selectedColumns[i]);
        }

        //we set the row values, but only on those columns that we need
        int k = 0;
        for (int i = 0; i < noOfColumns && k < noOfColumns; i++)
        {
            if (columnsOfOriginalTable[i] == selectedColumns[k])
            {
                for (int j = 0; j < noOfRows; j++)
                {
                    rowsOfNewTable[j][i] = rowsOfOriginalTable[j][i];
                }
                k++;
            }
        }

        if (noOfWords < 5)
        {
            tableWithSelectedColumns->print_table();
            for (int i = 0; i < noOfRows; i++)
            {
                delete rowsOfOriginalTable[i];
                delete rowsOfNewTable[i];
            }
            delete originalTable;
            delete tableWithSelectedColumns;
            delete[] selectedColumns;
            delete[] columnsOfOriginalTable;
            delete[] rowsOfOriginalTable;
            delete[] rowsOfNewTable;
            return;
        } //for select with no where clause

        std::string columnName = words[noOfWords - 3];
        //value that we search for
        std::string value = words[noOfWords - 1];
        auto* tableWithSelectedRows = new Table(noOfColumns, tableName + ';');
        //; is also so we don't repeat table names same as before
        for (int i = 0; i < noOfRows; i++)
        {
            for (int j = 0; j < noOfColumns; j++)
            {
                if (rowsOfNewTable[i][j] == value)
                {
                    //it matches what we search for
                    tableWithSelectedRows->add_row(rowsOfNewTable[i]);
                }
            }
        }
        //we print the table with the selected rows based on the value
        //we were searching for
        tableWithSelectedRows->print_table();

        //we delete all dynamically allocated variables
        for (int i = 0; i < noOfRows; i++)
        {
            delete rowsOfOriginalTable[i];
            delete rowsOfNewTable[i];
        }
        delete originalTable;
        delete tableWithSelectedColumns;
        delete tableWithSelectedRows;
        delete[] selectedColumns;
        delete[] columnsOfOriginalTable;
        delete[] rowsOfOriginalTable;
        delete[] rowsOfNewTable;
    }

    void startApplication()
    {
        while (true)
        {
            int noOfWords;

            parser->setCommand();
            std::string word = parser->getString();
            int* numberOfParentheses = parser->checkBrackets();
            if (numberOfParentheses[0] == numberOfParentheses[1] && numberOfParentheses[0] != 0)
            {
                if (tolower(word[0]) == 's')
                {
                    words = parser->parse_with_brackets_select(noOfWords);
                }
                else
                {
                    words = parser->parse_with_brackets(noOfWords);
                }
            }
            else if (numberOfParentheses[0] == 0 && numberOfParentheses[1] == 0)
            {
                words = parser->parse_without_brackets(noOfWords);
            }
            else
            {
                statusManager->print(StatusManager::Error, "Invalid number of parentheses!");
            }
            setQuery(words, noOfWords, parser->getString());
            if (noOfWords == 1 && words[0] == "exit")
            {
                break;
            }
            parse_command();
        }
    }
};

//
// Created by ervin on 10/25/25.
//
#ifndef SQL_ENGINE_APPLICATION_H
#define SQL_ENGINE_APPLICATION_H
#include "Table.h"
#include <iostream>
#include <cctype>

#include "globals.h"
#include "Index_Catalog.h"
#include "Table_Catalog.h"
#include "Index.h"


std::string *parse_column(std::string column, int &noOfFields) {
    noOfFields = 1;
    for (int i = 0; i < column.length(); i++) {
        if (column[i] == ',') {
            noOfFields++;
        }
    }
    int k = 0;
    std::string *fields = new std::string[noOfFields];

    for (int i = 0; i < column.length(); i++) {
        if (column[i] == '\'' || column[i] == '(' || column[i] == ')' || column[i] == ';') {
            continue;
        }
        if (column[i] == ',') {
            k++;
        } else {
            fields[k] += column[i];
        }
    }
    return fields;
}


class Application {
private:
    std::string *words;
    int noOfWords;
    std::string s;

public:
    Application() {
        this->words = nullptr;
        this->noOfWords = 0;
    }

    ~Application() { delete[] this->words; }

    void setQuery(std::string *words, int noOfWords, std::string s) {
        this->s = s;
        this->words = words;
        this->noOfWords = noOfWords;
    }

    void parse_command() {
        std::string firstWord = words[0];
        if (firstWord == "create") {
            if (has_invalid_word_count(1)) {
                return;
            }
            std::string secondWord = words[1];
            if (secondWord == "table") {
                if (has_invalid_word_count(2)) {
                    return;
                }
                create_table();
            } else if (secondWord == "index") {
                if (has_invalid_word_count(2)) {
                    return;
                }
                create_index();
            } else {
                std::cout << "Wrong statement! You can create a table or an index." << std::endl;
            }
        } else if (firstWord == "drop") {
            if (has_invalid_word_count(1)) {
                return;
            }
            std::string secondWord = words[1];
            if (secondWord == "table") {
                if (has_invalid_word_count(2)) {
                    return;
                }
                drop_table();
            } else if (secondWord == "index") {
                if (has_invalid_word_count(2)) {
                    return;
                }
                drop_index();
            } else {
                std::cout << "Wrong statement! You can drop a table or an index." << std::endl;
            }
        } else if (firstWord == "display") {
            if (has_invalid_word_count(1)) {
                return;
            }
            display_table();
        } else if (firstWord == "insert") {
            if (has_invalid_word_count(1)) {
                return;
            }
            insert_into_table();
        } else if (firstWord == "select") {
            if (has_invalid_word_count(1)) {
                return;
            }
            create_table();
        } else if (firstWord == "delete") {
            if (has_invalid_word_count(1)) {
                return;
            }
            delete_from();
        } else if (firstWord == "update") {
            if (has_invalid_word_count(1)) {
                return;
            }
            update_table();
        } else if (firstWord == "debug") {
            if (noOfWords == 1) {
                std::cout << "tables or indexes?" << std::endl;
            } else {
                if (words[1] == "tables") {
                    if (tableCatalog->getNoOfTables() == 0) {
                        std::cout << "There are no tables!" << std::endl;
                    } else {
                        std::cout << "The tables are:" << std::endl;
                        tableCatalog->print_tables();
                    }
                } else if (words[1] == "indexes") {
                    indexCatalog->print_indexes();
                }
            }
        } else {
            std::cout << "Command is wrong. Please enter a new command. " << std::endl;
        }
    }

    bool has_invalid_word_count(int noOfWords) {
        if (this->noOfWords == noOfWords) {
            std::cout << "Incomplete input!" << std::endl;
            return true;
        }
        return false;
    }

    void insert_into_table() {
        if (words[1] != "into" || words[3] != "values") {
            std::cout << "Syntax error!" << std::endl;
            return;
        }
        std::string tableName = words[2];
        if (!tableCatalog->table_exists(tableName)) {
            std::cout << "Table " << tableName << " does not exist!" << std::endl;
            return;
        }

        int noOfFields;
        std::string *inputFields = parse_column(words[4], noOfFields);

        if (noOfFields != tableCatalog->getNumberOfColumns(tableName)) {
            debug(noOfFields);
            debug(tableCatalog->getNumberOfColumns(tableName));
            std::cout << std::endl << "Invalid input!" << std::endl;
            return;
        }

        Table *table = tableCatalog->getTable(tableName);
        table->add_row(inputFields);
        std::cout << "Inserted succesfully!" << std::endl;
    }


    void create_table() {
        if (noOfWords >= 3 && words[3] == "if") {
            if (words[4] != "not" || words[5] != "exists") {
                std::cout << "Syntax error!" << std::endl;
                return;
            }
            std::string tableName = "";
            for (int i = 0; i < words[2].length(); i++) {
                tableName += tolower(words[2][i]);
            }
            if (tableCatalog->table_exists(tableName)) {
                std::cout << "Table already exists." << std::endl;
                return;
            }

            if (noOfWords != 7) {
                // daca n are field introdus sau daca mai are ceva
                // introdus dupa field/field-uri
                std::cout << "Too many spaces" << std::endl;
                return;
            }
            if ((words[6][0] != '(' || words[6][1] != '(' || words[6][words[6].length() - 2] != ')'
                ) && (words[6][words[6].length() - 1] != ';' || words[6][words[6].length() - 1] != ')')
            ) {
                std::cout << "Invalid format primele 2" << std::endl;
                return;
            }

            for (int i = 0; i < words[6].length(); i++) {
                if (!isascii(words[6][i])) {
                    std::cout << "Invalid format!!!!!!" << std::endl; // se accepta doar ((x,z3,f,d)), fara
                    // spatii in plus sau alte caractere
                    return;
                }
            }

            int noOfColumns = 1;

            for (int i = 2; i < words[6].length() - 3; i++) {
                if (words[6][i] == ')'
                    and
                    (words[6][i + 1] != ',' or words[6][i + 2] != '(')
                ) {
                    std::cout << "Invalid format separator" << std::endl; // "separatorul" de coloane
                    // este "),("
                    return;
                }

                if (words[6][i] == ')' and words[6][i + 1]
                    ==
                    ','
                    and
                    words[6][i + 2]
                    ==
                    '('
                ) {
                    i += 2; // sarim peste separator
                    noOfColumns++;
                }
                //   if (words[6][i] == '(') {
                //   std::cout << "Invalid format ( in plus" << i;// '('
                //   return;
                // }
            }

            auto *columns = new std::string[noOfColumns + 1];
            int k = 0;

            for (int i = 2; i < words[6].length() - 1; i++) {
                if (words[6][i] == ')') {
                    k++;
                    i += 2;
                } else {
                    columns[k] += words[6][i];
                }
            }

            auto *columnNames = new std::string[noOfColumns];
            for (int i = 0; i < noOfColumns; i++) {
                columnNames[i] += tolower(columns[i][0]);
                for (int j = 1; j < columns[i].length() && columns[i][j] != ','; j++) {
                    columnNames[i] += columns[i][j];
                }
            }

            int noOfFields;
            for (int j = 0; j < noOfColumns; j++) {
                std::string *fields = parse_column(columns[j], noOfFields);

                if (noOfFields != 4) {
                    std::cout << "Every column should contain only 4 fields." << std::endl;
                    return;
                }
            }


            // we create the table
            Table *table = new Table(noOfColumns, tableName);
            for (int i = 0; i < noOfColumns; i++) {
                table->setColumn(i, columnNames[i]);
            }
            if (tableCatalog->add_table(*table) == 0) {
                std::cout << "Table " << tableName << " created succesfully!" << std::endl;
            }
            delete table;
        }
    }

    void create_index() {
        if (noOfWords < 8) {
            std::cout << "Incomplete input!" << std::endl;
            return;
        }
        if (words[2] == "if") {
            if (words[3] != "not" || words[4] != "exists" || words[6] != "on") {
                std::cout << "Syntax error!" << std::endl;
                return;
            }
            std::string indexName = words[5];
            if (indexCatalog->index_exists(indexName)) {
                std::cout << "Index already exists!" << std::endl;
                return;
            }

            std::string tableName = words[7];
            if (!tableCatalog->table_exists(tableName)) {
                std::cout << "Table " << tableName << " does not exist! Cannot create index." << std::endl;
                return;
            }

            if (words[8][0] != '(' || words[8][words[8].length() - 1] != ')') {
                std::cout << "Invalid format paranteze" << std::endl;
                return;
            }

            std::string columnName;
            for (int i = 1; i < words[8].length() - 1; i++) {
                columnName += tolower(words[8][i]);
            }

            Table *table = tableCatalog->getTable(tableName);
            if (!table->column_exists(columnName)) {
                std::cout << "Column " << columnName << " does not exist!" << std::endl;
                return;
            }

            if (table->setIndex(indexName, columnName) == 0) {
                Index *index = new Index(indexName, tableName, columnName);
                indexCatalog->add_index(*index);
                std::cout << "Index " << indexName << " created succesfully!" << std::endl;
                delete index;
            } else {
                std::cout << indexName << std::endl;
                std::cout << table->getHasIndex() << std::endl;
                std::cout << "The table \"" << tableName << "\" already has an index, and the index is on column \"" <<
                        table->getColumnOfTheIndex() << "\"!" << std::endl;
            }
        }
    }

    void drop_table() {
        std::string tableName = "", aux = words[2];
        for (int i = 0; i < words[2].length(); i++) {
            tableName += tolower(words[2][i]);
        }

        if (tableCatalog->drop_table(tableName) == 0) {
            std::cout << "Dropped table " << aux << " succesfully!" << std::endl;
        }
    }

    void drop_index() {
        std::string indexName = "", aux = words[2];
        for (int i = 0; i < words[2].length(); i++) {
            indexName += tolower(words[2][i]);
        }

        if (indexCatalog->drop_index(indexName) == 0) {
            std::cout << "Dropped index " << aux << " succesfully!" << std::endl;
        }
    }

    void display_table() {
        std::string tableName = words[2];
        if (tableCatalog->table_exists(tableName)) {
            tableCatalog->getTable(tableName)->print_table();
        } else {
            std::cout << "Table " << tableName << " does not exist!" << std::endl;
        }
    }

    void update_table() { std::cout << "Updated table successfully!" << std::endl; }

    void delete_from() {
        if (words[1] != "from" || words[3] != "where") {
            std::cout << "Syntax error!" << std::endl;
            return;
        }

        std::string tableName = words[2];
        std::string columnName = words[4];
        std::string value = words[6];
        if (!tableCatalog->table_exists(tableName)) {
            std::cout << "Table " << tableName << " does not exist!" << std::endl;
            return;
        }

        Table *table = tableCatalog->getTable(tableName);
        if (table->delete_from(columnName, value) == 0) {
            std::cout << "Row deleted successfully!" << std::endl;
        }
    }
};

#endif // SQL_ENGINE_APPLICATION_H

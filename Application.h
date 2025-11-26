#pragma once
#include "Table_Catalog.h"
#include "Parser.h"


class Application {
private:
    Parser *parser = new Parser;
    std::string *words;
    int noOfWords;
    std::string s;

public:
    Application() {
        this->words = nullptr;
        this->noOfWords = 0;
    }

    ~Application() {
        delete[] this->words;
        delete parser;
    }

    void print_application() const {
        std::cout << "" << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &out, const Application &application) {
        application.print_application();
        return out;
    }

    bool operator==(const Application &application) const {
        if (this->parser != application.parser || this->noOfWords != application.noOfWords)
            return false;

        for (int i = 0; i < noOfWords; i++) {
            if (this->words[i] != application.words[i]) {
                return false;
            }
        }

        if (this->s != application.s)
            return false;

        return true;
    }

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

                if (words[3] == "if") {
                    create_table_with_if_not_exists();
                } else {
                    create_table_without_if_not_exists();
                }
            } else if (secondWord == "index") {
                if (has_invalid_word_count(2)) {
                    return;
                }
                if (words[3] == "if") {
                    create_index_with_if_not_exists();
                } else {
                    create_index_without_if_not_exists();
                }
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
            select_from();
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
                        std::cout << "ERROR: There are no tables!" << std::endl;
                    } else {
                        std::cout << "The tables are:" << std::endl;
                        tableCatalog->print_tables();
                    }
                } else if (words[1] == "indexes") {
                    indexCatalog->print_indexes();
                }
            }
        } else {
            std::cout << "ERROR: Command is wrong. Please enter a new command. " << std::endl;
        }
    }

    bool has_invalid_word_count(int noOfWords) {
        if (this->noOfWords == noOfWords) {
            std::cout << "ERROR: Incomplete input!" << std::endl;
            return true;
        }
        return false;
    }

    void insert_into_table() {
        if (words[1] != "into" || words[3] != "values") {
            std::cout << "ERROR: Syntax error!" << std::endl;
            return;
        }
        std::string tableName = words[2];
        if (!tableCatalog->table_exists(tableName)) {
            std::cout << "ERROR: Table " << tableName << " does not exist!" << std::endl;
            return;
        }

        int noOfFields;
        std::string *inputFields = parser->parse_column(words[4], noOfFields);

        if (noOfFields != tableCatalog->getNumberOfColumns(tableName)) {
            std::cout << std::endl << "ERROR: Invalid input!" << std::endl;
            return;
        }

        Table *table = tableCatalog->getTable(tableName);
        table->add_row(inputFields);
        std::cout << "SUCCESS: Inserted succesfully!" << std::endl;
    }


    void create_table(std::string tableName) {
        int indexOfLastWord = noOfWords - 1;
        if ((words[indexOfLastWord][0] != '(' || words[indexOfLastWord][1] != '(' || words[indexOfLastWord][
                 words[indexOfLastWord].length() - 2] != ')'
        )) {
            std::cout << "ERROR: Invalid format paranteze!" << std::endl;
            return;
        }

        for (int i = 0; i < words[indexOfLastWord].length(); i++) {
            if (!isascii(words[indexOfLastWord][i])) {
                std::cout << "ERROR: Only ascii characters accepted!" << std::endl;
                return;
            }
        }

        int noOfColumns = 1;

        for (int i = 2; i < words[indexOfLastWord].length() - 2; i++) {
            if (words[indexOfLastWord][i] == ')' && (
                    words[indexOfLastWord][i + 1] != ',' || words[indexOfLastWord][i + 2] != '(')
            ) {
                std::cout << "ERROR: Invalid format separator" << std::endl; // "separatorul" de coloane
                // este "),("
                return;
            }

            if (words[indexOfLastWord][i] == ')' && words[indexOfLastWord][i + 1] == ',' && words[indexOfLastWord][
                    i + 2] == '(') {
                i += 2; // sarim peste separator
                noOfColumns++;
            }
        }

        auto *columns = new std::string[noOfColumns + 1];
        int k = 0;

        for (int i = 2; i < words[indexOfLastWord].length() - 1; i++) {
            if (words[indexOfLastWord][i] == ')') {
                k++;
                i += 2;
            } else {
                columns[k] += words[indexOfLastWord][i];
            }
        }

        int noOfFields;
        Table *table = new Table(noOfColumns, tableName);
        for (int j = 0; j < noOfColumns; j++) {
            std::string *fields = parser->parse_column(columns[j], noOfFields);

            if (noOfFields != 4) {
                std::cout << "ERROR: Every column should contain only 4 fields!" << std::endl;
                delete table;
                return;
            }
            table->setColumn(j, fields[0]);
        }

        if (tableCatalog->add_table(*table) == 0) {
            std::cout << "SUCCESS: Table " << tableName << " created succesfully!" << std::endl;
        }
        delete table;
    }

    void create_table_without_if_not_exists() {
        std::string tableName = "";
        for (int i = 0; i < words[2].length(); i++) {
            tableName += tolower(words[2][i]);
        }

        if (tableCatalog->table_exists(tableName)) {
            std::cout << "Error! Table already exists!" << std::endl;
            return;
        }

        create_table(tableName);
    }

    void create_table_with_if_not_exists() {
        std::string tableName = "";
        for (int i = 0; i < words[2].length(); i++) {
            tableName += tolower(words[2][i]);
        }

        if (words[4] != "not" || words[5] != "exists") {
            std::cout << "Syntax error!" << std::endl;
            return;
        }
        if (tableCatalog->table_exists(tableName)) {
            return;
        }
        create_table(tableName);
    }

    void create_index_without_if_not_exists() {
        std::string indexName = "";
        for (int i = 0; i < words[2].length(); i++) {
            indexName += tolower(words[2][i]);
        }

        if (indexCatalog->index_exists(indexName)) {
            std::cout << "Error! Index already exists!" << std::endl;
            return;
        }

        create_index();
    }

    void create_index_with_if_not_exists() {
        if (words[3] != "not" || words[4] != "exists" || words[6] != "on") {
            std::cout << "Syntax error!" << std::endl;
            return;
        }

        std::string indexName = "";
        for (int i = 0; i < words[5].length(); i++) {
            indexName += tolower(words[5][i]);
        }

        if (indexCatalog->index_exists(indexName)) {
            return;
        }
        create_index();
    }

    void create_index() {
        int indexOfLastWord = noOfWords - 1;
        if (noOfWords < 6) {
            std::cout << "ERROR: Incomplete input!" << std::endl;
            return;
        }
        std::string indexName = words[noOfWords - 3];
        std::string tableName = words[indexOfLastWord - 1];
        if (!tableCatalog->table_exists(tableName)) {
            std::cout << "ERROR: Table " << tableName << " does not exist! Cannot create index." << std::endl;
            return;
        }

        if (words[indexOfLastWord][0] != '(' || words[indexOfLastWord][words[indexOfLastWord].length() - 1] != ')') {
            std::cout << "ERROR: Invalid format paranteze" << std::endl;
            return;
        }

        std::string columnName;
        for (int i = 1; i < words[indexOfLastWord].length() - 1; i++) {
            columnName += tolower(words[indexOfLastWord][i]);
        }

        Table *table = tableCatalog->getTable(tableName);
        if (!table->column_exists(columnName)) {
            std::cout << "ERROR: Column " << columnName << " does not exist!" << std::endl;
            return;
        }

        if (indexCatalog->setIndex(indexName, columnName) == 0) {
            Index *index = new Index(indexName, tableName, columnName);
            indexCatalog->add_index(*index);
            std::cout << "SUCCESS: Index " << indexName << " created succesfully!" << std::endl;
            delete index;
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
            std::cout << "Dropped index \"" << aux << "\" succesfully!" << std::endl;
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

    void select_from() {
    }

    void startApplication() {
        while (true) {
            int noOfWords;

            std::string *words;
            parser->setCommand();
            if (parser->checkBrackets() == true) {
                words = parser->parse_with_brackets(noOfWords);
            } else {
                words = parser->parse_without_brackets(noOfWords);
            }
            setQuery(words, noOfWords, parser->getString());
            if (noOfWords == 1 && words[0] == "exit") {
                break;
            }
            parse_command();
        }
    }
};

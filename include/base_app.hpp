#pragma once
#include <string>
#include "parser.hpp"
#include "table.hpp"

class BaseApp
{
    private:
        Parser* parser = new Parser();
        std::string s;

    protected:
        std::string* words;
        int noOfWords = 0;

    public:
        BaseApp();
        BaseApp(const std::string* words, const int noOfWords);
        virtual ~BaseApp();
        BaseApp(const BaseApp& a);

        bool operator==(const BaseApp& application) const;

        BaseApp& operator=(const BaseApp& a);
        operator int() const;
        bool operator!() const;
        std::string operator[](int index) const;
        friend std::ostream& operator<<(std::ostream& out, const BaseApp& application);

        void createTable(const std::string& tableName) const;

        void writeTableToFile(const Table& table) const;

        void createTableWithoutIfNotExists() const;

        void createTableWithIfNotExists() const;

        void createIndexWithIfNotExists() const;
        void createIndexWithoutIfNotExists() const;
        void createIndex() const;
        void dropTable() const;
        void dropIndex() const;
        void displayTable() const;
        void updateTable() const;
        void writeSelectToFile(const Table& table, const std::string& synonymName) const;
        void deleteFrom() const;
        void selectFrom() const;
        void importTable() const;
        void parseCommandsFromFiles(int argc, char** argv);
        void parseCommands();
        static void listTables();
        void parseCommand();
        bool hasInvalidWordCount(const int wordCount) const;
        virtual void createSynonym() const = 0; 
        void insertIntoTable() const;
        void printApplication() const;
        void setQuery(std::string* words, int noOfWords, const std::string& s);
};

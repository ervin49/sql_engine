#pragma once
#include <iostream>
#include <string>

class Parser
{
private:
    std::string s;

public:
    Parser();
    Parser(const std::string s);
    Parser(const Parser& p);
    ~Parser();

    Parser& operator=(const Parser& p);
    operator std::string() const;
    bool operator!() const;
    char operator[](int index) const;
    bool operator==(const Parser& parser) const;

    void setCommand();
    void setCommandFromFile(const std::string& line);

    std::string getString() const;

    friend std::ostream& operator<<(std::ostream& out, const Parser& parser);

    int* checkBrackets() const;

    std::string* parseWithoutBrackets(int& noOfWords) const;

    std::string* parseWithBrackets(int& noOfWords, const bool isValueFromInsert = false) const;

    std::string* parseWithBracketsSelect(int& noOfWords) const;

    std::string* parseColumn(const std::string& column, int& noOfFields);

    void printParser() const;
};

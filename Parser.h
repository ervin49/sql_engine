#pragma once
#include <iostream>
#include <string>

class Parser {
private:
    std::string s;

public:
    Parser() = default;

    void setCommand() {
        std::getline(std::cin, this->s);
    }

    std::string getString() const {
        return s;
    }

    bool operator==(const Parser &parser) const {
        if (this->s != parser.s)
            return false;

        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, const Parser &parser) {
        parser.print_parser();
        return out;
    }

    bool checkBrackets() const {
        for (int i = 0; i < this->s.length(); i++)
            if (s[i] == ')' || s[i] == '(') {
                return true;
            }
        return false;
    }

    std::string *parse_without_brackets(int &noOfWords) const {
        noOfWords = 0;
        int startIndex = -1;
        int stopIndex = -1;
        for (int i = 0; i < this->s.length(); i++) {
            if (startIndex == -1 && this->s[i] != ' ') {
                startIndex = i;
            }
            if (this->s[i] != ' ') {
                stopIndex = i;
            }
        }

        for (int i = startIndex + 1; i <= stopIndex; i++) {
            if (this->s[i - 1] != ' ' && this->s[i] == ' ') {
                noOfWords++;
            }
        }
        noOfWords++;

        auto *words = new std::string[noOfWords];
        int currentWordIndex = 0;
        for (int i = startIndex; i <= stopIndex; i++) {
            if (this->s[i] == ' ' and this->s[i - 1] != ' ') {
                currentWordIndex++;
            } else if (this->s[i] != ' ') {
                //adaugam caracterele direct lowercase, ca stringul sa fie case-insensitive
                words[currentWordIndex] += tolower(this->s[i]);
            }
        }
        return words;
    }


    std::string *parse_with_brackets(int &noOfWords) const {
        noOfWords = 0;
        int startIndex = -1;
        int stopIndex = -1;
        bool bracketFound = false;
        int k = 0;
        for (int i = 0; i < this->s.length(); i++) {
            if (startIndex == -1 && this->s[i] != ' ') {
                startIndex = i;
            }
            if (s[i] == '(' && bracketFound == false) {
                bracketFound = true;
                k = i;
            }
            if (this->s[i] != ' ' && bracketFound == false) {
                stopIndex = i;
            }
        }
        for (int i = startIndex + 1; i <= stopIndex; i++) {
            if (this->s[i - 1] != ' ' && this->s[i] == ' ') {
                noOfWords++;
            }
        }
        noOfWords += 2;

        auto *words = new std::string[noOfWords];
        int currentWordIndex = 0;
        for (int i = startIndex; i <= stopIndex; i++) {
            if (this->s[i] == ' ' && this->s[i - 1] != ' ' && i > startIndex) {
                currentWordIndex++;
            } else if (this->s[i] != ' ') {
                //adaugam caracterele direct lowercase, ca stringul sa fie case-insensitive
                words[currentWordIndex] += tolower(this->s[i]);
            }
        }
        currentWordIndex++;

        for (int i = k; i < this->s.length(); i++) {
            if (s[i - 1] == ')' && s[i] == ')') {
                words[currentWordIndex] += tolower(this->s[i]);
                break;
            }
            words[currentWordIndex] += tolower(this->s[i]);
        }

        return words;
    }

    static std::string *parse_column(std::string column, int &noOfFields) {
        noOfFields = 1;
        for (int i = 0; i < column.length(); i++) {
            if (column[i] == ',') {
                noOfFields++;
            }
        }
        std::string *fields = new std::string[noOfFields];
        //std::string *parsedFields = new std::string[noOfFields];

        int k = 0;
        for (int i = 0; i < column.length(); i++) {
            if (column[i] == '(' || column[i] == ')') {
                continue;
            }
            if (column[i] == ',') {
                k++;
            } else {
                fields[k] += column[i];
            }
        }

        for (int i = 0; i <= k; i++) {
            int poz1 = -1;
            int poz2 = -1;

            // Cautam inceputul (primul caracter care nu e spatiu)
            for (int j = 0; j < fields[i].length(); j++) {
                if (fields[i][j] != ' ' && poz1 == -1)
                    poz1 = j;
            }

            // Cautam sfarsitul (ultimul caracter care nu e spatiu)
            for (int j = 0; j < fields[i].length(); j++) {
                if (fields[i][j] != ' ') {
                    poz2 = j;
                }
            }

            if (poz1 != -1 && poz2 != -1) {
                // lungimea = index_final - index_start + 1
                fields[i] = fields[i].substr(poz1, poz2 - poz1 + 1);
            } else {
                // Daca nu am gasit caractere valide (ex: stringul era doar "   "), il golim
                fields[i] = " ";
            }
        }

        // Returnam direct fields, nu parsedFields
        return fields;
    }

    void print_parser() const {
        std::cout << "Current string stored in the parser is: \"" << s << "\"." << std::endl;
    }
};

#include <cstring>
#include <iostream>

#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

class Project {
private:
    std::string *words;
    int noOfWords;

public:
    Project(std::string *words, int noOfWords) {
        this->words = words;
        this->noOfWords = noOfWords;
    }

    void parse_command() {
        std::string firstWord = words[0];
        if (firstWord == "create") {
            std::string secondWord = words[1];
            if (secondWord == "table") {
                create_table();
            } else if (secondWord == "index") {
                create_index();
            } else {
                std::cout << "Wrong statement! You can create a table or an index.";
            }
        } else if (firstWord == "drop") {
            std::string secondWord = words[1];
            if (secondWord == "table") {
                drop_table();
            } else if (secondWord == "index") {
                drop_index();
            } else {
                std::cout << "Wrong statement! You can drop a table or an index.";
            }
        } else if (firstWord == "display") {
            display_table();
        } else if (firstWord == "insert") {
            create_table();
        } else if (firstWord == "select") {
            create_table();
        } else if (firstWord == "delete") {
        } else if (firstWord == "update") {
        } else {
            std::cout << "Command is wrong";
        }
    }


    void create_table() {
        if (words[2] == "if") {
            if (words[3] != "not" || words[4] != "exists") {
                std::cout << "Syntax error!";
                return;
            }
            return;
        }
        std::cout << "Table created successfully.";
    }

    void create_index() {
        std::cout << "Index created successfully.";
    }

    void drop_table() {
        std::cout << "Dropped table succesfully.";
    }

    void drop_index() {
        std::cout << "Dropped index successfully.";
    }

    void display_table() {
        std::cout << "Dropped index successfully.";
    }

    void update_table() {
        std::cout << "Updated table successfully.";
    }

    void delete_from() {
    }
};


std::string *read_command_from_console(int &n);

int main() {
    int noOfWords;
    std::string *words = read_command_from_console(noOfWords);
    auto *project = new Project(words, noOfWords);
    project->parse_command();
    delete project;
    delete[] words;
    return 0;
}

std::string *read_command_from_console(int &n) {
    std::string s;
    int noOfWords = 0;
    getline(std::cin, s);
    //numaram cate spatii sunt in tot stringul
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ')
            noOfWords++;
    }
    noOfWords++; //numarul de cuvinte e egal cu numarul de spatii + 1

    auto *words = new std::string[noOfWords];
    int i = 0, currentWordIndex = 0;
    while (i < s.length()) {
        if (s[i] == ' ') {
            currentWordIndex++;
        } else {
            //adaugam caracterele direct lowercase, ca stringul sa fie case-insensitive
            words[currentWordIndex] += tolower(s[i]);
        }
        i++;
    }
    n = noOfWords;
    return words;
}
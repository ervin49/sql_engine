//
// Created by ervin on 10/25/25.
//

#ifndef SQL_ENGINE_APPLICATION_H
#define SQL_ENGINE_APPLICATION_H

class Application {
private:
    std::string *words;
    int noOfWords;

public:
    Application(std::string *words, int noOfWords) {
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
            if (/*tabela exista*/) {
                std::cout << "Table already exists.";
                return;
            }
            //creare tabela
            return;
        }
        if (/*tabela exista*/) {
            throw std::runtime_error("Error! Table already exists!");
            return;
        }
        //creare tabela
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
    }

    void update_table() {
        std::cout << "Updated table successfully.";
    }

    void delete_from() {
        std::cout << "Deleted successfully.";
    }
};

#endif //SQL_ENGINE_APPLICATION_H
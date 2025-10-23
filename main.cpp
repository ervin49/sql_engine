#include <cstring>
#include <iostream>

#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

class Project
{
public:
    void parse_command(std::string command)
    {
        std::string word;

        //construim primul cuvant din comanda, in care fiecare caracter va fi pus lowercase, ca stringul final sa fie case-insensitive
        int i;
        for (i = 0; command[i] != ' ' && i < command.length(); i++)
        {
            word += tolower(command[i]);
        }
        debug(word);

        if (i < command.length())
        {
            i++; //i acum se afla pe primul caracter de dupa primul spatiu
        }
        else
        {
            std::cout << "Command cannot contain only one word!" << std::endl;
        }

        if (word == "create")
        {
            std::string second_word;
            while (command[i] != ' ' && i < command.length())
            {
                second_word += tolower(command[i]);
                i++;
            }
            debug(second_word);
            if (second_word == "table")
            {
                create_table();
            }
            else
            {
                create_index();
            }
        }
        else if (word == "drop")
        {
            std::string second_word;
            while (command[i] != ' ' && i < command.length())
            {
                second_word += command[i];
                i++;
            }
            if (second_word == "table" || second_word == "TABLE")
            {
                drop_table();
            }
            else
            {
                drop_index();
            }
        }
        else if (word == "display")
        {
            display_table();
        }
        else if (word == "insert")
        {
            create_table();
        }
        else if (word == "select")
        {
            create_table();
        }
        else if (word == "delete")
        {
        }
        else if (word == "update")
        {
        }
        else
        {
            std::cout << word << "Command is wrong";
        }
    }

    void create_table()
    {
        std::cout << "Table created";
    }

    void create_index()
    {
    }

    void drop_table()
    {
    }

    void drop_index()
    {
    }

    void display_table()
    {
    }

    void update_table()
    {
    }

    void delete_from()
    {
    }
};

int main()
{
    std::string command;
    std::cin >> command;
    Project* project = new Project();
    project->parse_command(command);
    return 0;
}

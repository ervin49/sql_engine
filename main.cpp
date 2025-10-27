#include <cstring>
#include <iostream>
#include <string>
#include "Application.h"

#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

std::string *read_command_from_console(int &n);

void startApplication();

int main() {
    startApplication();
    return 0;
}

std::string *read_command_from_console(int &n) {
    std::string s;
    int noOfWords = 0;
    
    std::getline(std::cin, s);
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

void startApplication() {
    bool running = true;
    
    auto *application = new Application;

    while(running)
    {
        int noOfWords;
        
        std::string *words = read_command_from_console(noOfWords);
        application->setQuery(words, noOfWords);
        if(noOfWords == 1 && words[0] == "exit") {
            running = false;
        } 
        else {
            application->parse_command();

        }

        delete[] words;
    }
    delete application;
}
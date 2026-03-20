#pragma once
class HelpMenu
{
public:
    HelpMenu();
    static void pressEnterToContinue();

    static void printSyntaxHelp();

    static void printDatatypesHelp();

    static void printGeneralHelp();

    void displayHelp();
};

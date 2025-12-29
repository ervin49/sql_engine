#include "Application.h"
#include "globals.cpp"
#include "Menu.h"

int main(const int argc, char** argv)
{
    if (argc == 1)
    {
        const auto menu = new Menu();
        menu->display_welcome_menu();
        delete menu;
        return 0;
    }

    const auto application = new Application;
    application->parse_commands_from_files(argc, argv);
    delete application;
    return 0;
}

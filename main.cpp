#include "Application.h"

int main(const int argc, char **argv) {
    const auto application = new Application;
    if (argc == 1) {
        application->parseCommands();
        delete application;
        return 0;
    }
    application->parseCommandsFromFiles(argc, argv);
    delete application;
    return 0;
}

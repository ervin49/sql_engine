#include "Application.h"

int main() {
    Application *application = new Application;
    application->startApplication();
    delete application;
    return 0;
}

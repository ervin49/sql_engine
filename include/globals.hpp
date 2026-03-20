#pragma once
#include <cstdio>
#include <cstdlib>
#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

class StatusManager;
class TableCollection;
class IndexCollection;
class HelpMenu;

extern TableCollection* tableCollection;
extern IndexCollection* indexCollection;
extern StatusManager* statusManager;
extern HelpMenu* helpMenu;

static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

inline int _getch()
{
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

#endif

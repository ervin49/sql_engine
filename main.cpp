#include "App.h"
#include "Menu.h"
#include "globals.cpp"

int main(const int argc, char** argv)
{
	if (argc == 1)
	{
		const auto menu = new Menu();
		menu->display_welcome_menu();
		delete menu;
		return 0;
	}

	const auto app = new App;
	app->parse_commands_from_files(argc, argv);
	delete app;
	return 0;
}

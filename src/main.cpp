#include "app.hpp"
#include "main_menu.hpp"

int main(const int argc, char** argv)
{
	if (argc == 1)
	{
		const auto menu = new MainMenu();
		menu->displayWelcomeMenu();
		delete menu;
		return 0;
	}

	const auto app = new App;
	app->parseCommandsFromFiles(argc, argv);
	delete app;
	return 0;
}

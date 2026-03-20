#include "globals.hpp"

#include "status_manager.hpp"
#include "table_collection.hpp"
#include "index_collection.hpp"
#include "help_menu.hpp"

TableCollection* tableCollection = new TableCollection;
IndexCollection* indexCollection = new IndexCollection;
StatusManager* statusManager = new StatusManager;
HelpMenu* helpMenu = new HelpMenu;

#include "globals.h"

#include "StatusManager.h"
#include "Table_Catalog.h"
#include "Index_Catalog.h"

Table_Catalog* tableCatalog = new Table_Catalog;
Index_Catalog* indexCatalog = new Index_Catalog;
StatusManager* statusManager = new StatusManager;

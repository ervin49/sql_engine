#pragma once
#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

class StatusManager;
class Table_Catalog;
class Index_Catalog;
extern Table_Catalog* tableCatalog;
extern Index_Catalog* indexCatalog;
extern StatusManager* statusManager;

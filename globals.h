#pragma once
#define debug(x) std::cout << "variabila " << #x << " este " << x << std::endl;

class Table_Catalog;
class Index_Catalog;
extern Table_Catalog *tableCatalog;
extern Index_Catalog *indexCatalog;

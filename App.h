#pragma once
#include "BaseApp.h"
#include "Table.h"
class App : public BaseApp
{
public:
	void create_synonym() const override
	{
		if (noOfWords != 5)
		{
			statusManager->print(StatusManager::Error, "Invalid syntax!");
			return;
		}
		if (words[3] != "for")
		{
			statusManager->print(StatusManager::Error, "Parse error near '" + words[3] + "'!");
			return;
		}
		const std::string tableName = words[4];
		if (tableCatalog->table_exists(tableName) == false)
		{
			statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
			return;
		}

		const std::string synonymName = words[2];
		Table* table = tableCatalog->getTable(tableName);
		if (tableCatalog->synonym_exists(synonymName) == false)
		{
			table->add_synonym(synonymName);
			write_table_to_file(*table);
			statusManager->print(StatusManager::Success,
			                     "Synonym '" + synonymName + "' created successfully for table '" + tableName +
			                     "'!");
		}
		else
		{
			statusManager->print(StatusManager::Error, "Synonym '" + synonymName + "' already exists!");
		}
	}
};

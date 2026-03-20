#include "app.hpp"
#include "globals.hpp"
#include "status_manager.hpp"
#include "table_collection.hpp"

void App::createSynonym() const
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
    if (tableCollection->tableExists(tableName) == false)
    {
        statusManager->print(StatusManager::Error, "Table '" + tableName + "' does not exist!");
        return;
    }

    const std::string synonymName = words[2];
    Table* table = tableCollection->getTable(tableName);
    if (tableCollection->synonymExists(synonymName) == false)
    {
        table->addSynonym(synonymName);
        writeTableToFile(*table);
        statusManager->print(StatusManager::Success,
                "Synonym '" + synonymName + "' created successfully for table '" + tableName +
                "'!");
    }
    else
    {
        statusManager->print(StatusManager::Error, "Synonym '" + synonymName + "' already exists!");
    }

}

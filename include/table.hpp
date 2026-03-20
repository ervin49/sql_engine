#pragma once
#include "base_table.hpp"

class Table : public BaseTable
{
private:
	int noOfSynonyms;
	std::string* synonyms;

public:
	Table(const BaseTable& other,int noOfSynonyms,std::string* synonyms);
	Table();
	Table(const int noOfColumns, const std::string& tableName);
	~Table() override;

	Table& operator=(const Table& other);

	int getNoOfSynonyms() const;
	std::string* getSynonyms() const;

	void addSynonym(const std::string& newSynonym);
	void setSynonyms(const std::string* newSynonyms, const int newNoOfSynonyms);
	bool hasSynonym(const std::string& synonym) const;
};

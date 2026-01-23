#pragma once
#include "BaseTable.h"

class Table : public BaseTable
{
private:
	int noOfSynonyms;
	std::string* synonyms;

public:
	Table& operator=(const Table& other)
	{
		if (this != &other)
		{
			BaseTable::operator=(other);


            delete[] synonyms;
			this->noOfSynonyms = other.noOfSynonyms;
			this->synonyms = new std::string[noOfSynonyms];
			for (int i = 0; i < noOfSynonyms; i++)
			{
				this->synonyms[i] = other.synonyms[i];
			}
		}
		return *this;
	}

	Table(const BaseTable& other,int noOfSynonyms,std::string* synonyms) : BaseTable(other)
	{
		this->noOfSynonyms = noOfSynonyms;
		this->synonyms = new std::string[noOfSynonyms];
        for(int i = 0; i < noOfSynonyms; i++){
            this->synonyms[i] = synonyms[i];
        }
	}

	Table()
	{
		this->rows = nullptr;
		this->indexNames = nullptr;
		this->columnNames = nullptr;
		this->columnTypes = nullptr;
		this->maxColumnLengths = nullptr;
		this->synonyms = nullptr;
		this->noOfRows = 0;
		this->noOfColumns = 0;
		this->noOfIndexes = 0;
		this->noOfSynonyms = 0;
		this->tableName = "";
	}

	Table(const int noOfColumns, const std::string& tableName) : BaseTable(noOfColumns, tableName)
	{
		noOfSynonyms = 0;
		synonyms = nullptr;
	}

	~Table() override
	{
		delete[] synonyms;
	}

	void add_synonym(const std::string& newSynonym)
	{
		auto newSynonyms = new std::string[noOfSynonyms + 1];
		for (int i = 0; i < noOfSynonyms; i++)
		{
			newSynonyms[i] = synonyms[i];
		}
		newSynonyms[noOfSynonyms] = newSynonym;

		delete[] synonyms;
		synonyms = newSynonyms;
		noOfSynonyms++;
	}

	bool has_synonym(const std::string& synonym) const
	{
		if (tableName == synonym)
		{
			return true;
		}
		for (int i = 0; i < noOfSynonyms; i++)
		{
			if (synonyms[i] == synonym)
			{
				return true;
			}
		}

		return false;
	}

	int getNoOfSynonyms() const
	{
		return noOfSynonyms;
	}

	std::string* getSynonyms() const
	{
		auto aux = new std::string[noOfSynonyms];
		for (int i = 0; i < noOfSynonyms; i++)
		{
			aux[i] = synonyms[i];
		}

		return aux;
	}

	void setSynonyms(const std::string* newSynonyms, const int newNoOfSynonyms)
	{
		noOfSynonyms = newNoOfSynonyms;
		std::string* aux = new std::string[noOfSynonyms];
		for (int i = 0; i < noOfSynonyms; i++)
		{
			aux[i] = newSynonyms[i];
		}

		delete[] synonyms;
		synonyms = aux;
	}
};

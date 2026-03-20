#pragma once
#include <string>
#include <vector>

#define t 3 //gradul maxim al unui nod
// un nod are minim t - 1 elemente (in cazul nostru minim 2 elemente)
// si maxim 2*t - 1 elemente (in cazul nostru maxim 5 elemente)

class BTreeNode
{
	std::vector<std::string> keys; //valorile stocate in nod, "banane" , "mere", "pere"
	std::vector<std::vector<int>> values;
	// pozitiile unde se afla fiecare key (row index), values[i] corespunde cheii keys[i]
	std::vector<BTreeNode*> children; // orice nod cu k elemente are k + 1 copii
	bool isLeaf; // pentru a stii cand sa ne oprim, daca (isLeaf == true) si nu am gasit valoarea, inseamna ca nu exista

public:
	BTreeNode(bool isLeaf);

    ~BTreeNode();

	std::vector<int> search(const std::string& key);

	void splitChild(int i, BTreeNode* y);

	void insertNonFull(const std::string& key, int rowIndex);
};

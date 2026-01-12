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
	BTreeNode(bool isLeaf)
	{
		this->isLeaf = isLeaf;
	}

	std::vector<int> search(const std::string& key)
	{
		int i = 0;
		//cautam prima cheie care este >= cu ce cautam noi
		while (i < keys.size() && keys[i] < key)
		{
			i++;
		}
		if (i < keys.size() && keys[i] == key)
		{
			return values[i]; //daca am gasit cheia, returnam pe ce randuri apare
		}

		if (isLeaf)
		{
			return {}; // return la un vector gol, s-a terminat cautarea
		}

		//children[i] contine toate valorile mai mici decat keys[i]
		//deci unde s-ar afla cheia cautata

		return children[i]->search(key);
	}

	void splitChild(int i, BTreeNode* y)
	{
		// cream un nod nou z care va lua jumatatea din dreapta a lui y
		BTreeNode* z = new BTreeNode(y->isLeaf);


		// copiem in z cheile si "locatiile" cheilor din y, dar doar cele
		//din dreapta adica cele de pe pozitiile t+0, t+1 ... t+t-1
		for (int j = 0; j < t - 1; j++)
		{
			z->keys.push_back(y->keys[j + t]);
			z->values.push_back(y->values[j + t]);
		}

		//daca y nu e frunza, mutam in z si copii corespunzatori fiecarui element mutat
		if (!y->isLeaf)
		{
			for (int j = 0; j < t; j++)
			{
				z->children.push_back(y->children[j + t]);
			}
		}

		// elementul din mijloc (de la indexul t - 1) trebuie sa urce in parinte (this)
		std::string medianKey = y->keys[t - 1];
		std::vector<int> medianValues = y->values[t - 1];

		//redimensionam y(stergem ce am adaugat in z)
		y->keys.resize(t - 1);
		y->values.resize(t - 1);
		if (!y->isLeaf)
		{
			//daca nu e frunza, redimensionam si lista de copii
			y->children.resize(t);
		}

		//modificam parintele adaugand z in lista de copii si adaugand elementul din mijloc in nodul superior

		this->children.insert(children.begin() + i + 1, z);

		this->keys.insert(keys.begin() + i, medianKey);
		this->values.insert(values.begin() + i, medianValues);
	}

	void insertNonFull(const std::string& key, int rowIndex)
	{
		int i = keys.size() - 1; // pornim de la dreapta(cea mai amre cheie)

		for (int k = 0; k < keys.size(); k++)
		{
			if (keys[k] == key)
			{
				values[k].push_back(rowIndex);
				return;
			}
		}

		if (isLeaf)
		{
			//redimensionam keys si valuesa ca sa putem adauga cheia noua
			this->keys.push_back("");
			this->values.push_back({});


			//mutam elementele mai mari la dreapta pt a putea insera
			while (i >= 0 && keys[i] > key)
			{
				keys[i + 1] = keys[i];
				values[i + 1] = values[i];
				i--;
			}

			keys[i + 1] = key;
			values[i + 1] = {rowIndex};
		}
		else
		{
			//daca nu suntem intr o frunza, trebuie sa coboram la copilul potrivit
			while (i >= 0 && keys[i] > key)
			{
				i--;
			}
			//acum i este indexul cheii mai mici. copilul este la i+1
			i++;


			//inainte sa coboram, verificam daca nodul copil este plin, daca este ii dam split
			if (this->children[i]->keys.size() == 2 * t - 1)
			{
				splitChild(i, this->children[i]);

				//dupa split, nodul din mijloc a urcat, deci iar trebuie sa verificam daca suntem pe pozitia corecta
				if (keys[i] < key)
				{
					i++;
				}
			}

			//inseram, stiind ca acum copilul sigur are loc
			children[i]->insertNonFull(key, rowIndex);
		}
	}


	~BTreeNode()
	{
		for (auto child : children)
		{
			delete child;
		}
	}
};

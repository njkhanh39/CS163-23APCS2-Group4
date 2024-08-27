#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <cctype>
#include <sstream>
#include "Unicode/utf8.h"

#include "Definition.h"

using namespace std;

class Definition;

class Word {
private:
	string text;
	vector<Definition> defList;
	bool isFavourite;
	string dataset;
public:
	Word() {
		text = "";
		isFavourite = false;
	}

	Word(string word) {
		text = word;
		isFavourite = false;
	}

	Word(string word, string def) {
		text = word;
		addDefinition(def);
		isFavourite = false;
	}


	// Equality operator
	bool operator==(const Word& other) const {
		return text == other.text;
	}

	// Inequality operator
	bool operator!=(const Word& other) const {
		return !(*this == other);
	}

	// Less than operator
	bool operator<(const Word& other) const {
		if (text < other.text) return true;
		return false;
	}

	// Greater than operator
	bool operator>(const Word& other) const {
		if (text > other.text) return true;
		return false;
	}

	//check for emptiness
	bool empty();

	int findDefinition(string& def) {
		for (int i = 0; i < (defList).size(); ++i) {
			if (defList[i].getStringDefinition() == def) {
				return i;
			}
		}
		return -1;
	}

	//getters

	string getText();

	int getNumberOfDefinitions();

	Definition getDefinitionAt(int i);
	
	vector<Definition> getDefinitions();

	vector<string> getStringDefinitions();

	void markFavourite();

	string getDataset();

	//adders and setters

	void addDefinition(string& def);

	void addDefinition(Definition& def);

	void setWord(string word);

	void setDefinition(string def, int index);

	void setFavourite(bool state);

	void setDataset(string dataset);

	void clear();

	//adjust-er

	void sortDefinitions();

	void modifyDefinition(string def, int index);

	void removeDefinition(int index);

	void removeDefinition(string& def);

	//helpers

	void merge(vector<Definition>& a, int l, int r, int mid);

	void mergeSort(vector<Definition>& a, int l, int r, int n);
	

};
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

	//getters

	string getWord();

	int getNumberOfDefinitions();

	Definition getDefinitionAt(int i);
	
	vector<Definition> getDefinitions();

	vector<string> getStringDefinitions();

	bool getFavourite();

	string getDataset();

	//adders and setters

	void addDefinition(string& def);

	void addDefinition(Definition& def);

	void setWord(string word);

	void setFavourite(bool state);

	void setDataset(string dataset);
	void clear();

	//adjust-er

	void sortDefinitions();

	void modifyDefinition(string def, int index);

	void removeDefinition(int index);

	void removeDefinition(string& def);

	void merge(vector<Definition>& a, int l, int r, int mid) {
		vector<Definition> temp(r - l + 1);

		int ptr1 = l, ptr2 = mid + 1, cur = 0;

		while (ptr1 <= mid && ptr2 <= r) {
			if (a[ptr1] < a[ptr2])
				temp[cur++] = a[ptr1++];
			else
				temp[cur++] = a[ptr2++];
		}

		while (ptr1 <= mid) temp[cur++] = a[ptr1++];
		while (ptr2 <= r) temp[cur++] = a[ptr2++];

		for (int i = l, cnt = 0; i <= r; ++i) a[i] = temp[cnt++];
	}

	void mergeSort(vector<Definition>& a, int l, int r, int n) {
		if (l > r || l==r) return;

		int mid = l + (r - l) / 2;

		mergeSort(a, l, mid, n);
		mergeSort(a, mid + 1, r, n);

		merge(a, l, r, mid);
	}

};
#include "Word.h"


//check for emptiness
bool Word::empty() {
	if (text == "") return true;
	return false;
}

//getters

string Word::getText() {
	return text;
}

int Word::getNumberOfDefinitions() {
	return (int)defList.size();
}

Definition Word::getDefinitionAt(int i) {
	if (i < (int)defList.size() && i >= 0) return defList[i];
	Definition trash;
	return trash;
}

vector<Definition> Word::getDefinitions() {
	return defList;
}

vector<string> Word::getStringDefinitions() {
	vector<string> ans;
	for (auto& p : defList) {
		ans.push_back(p.getStringDefinition());
	}

	return ans;
}

void Word::markFavourite() {
	isFavourite = true;
}

string Word::getDataset() {
	return dataset;
}

//adders and setters

void Word::addDefinition(string& def) {
	Definition newDef = Definition(def, this);
	defList.push_back(newDef);
}

void Word::addDefinition(Definition& def) {
	defList.push_back(def);
}

void Word::setWord(string word) {
	text = word;
}

void Word::clear() {
	text = "";
	defList.clear();
}

void Word::setFavourite(bool state) {
	isFavourite = state;
}

void Word::setDataset(string dataset) {
	this->dataset = dataset;
}

//adjust-er

void Word::sortDefinitions() {
	int n = getNumberOfDefinitions();
	mergeSort(defList, 0, n - 1, n);
}

void Word::modifyDefinition(string def, int index) {
	defList[index] = Definition(def, this);
}

void Word::removeDefinition(int index) {
	defList.erase(defList.begin() + index);
}

void Word::removeDefinition(string& def) {
	for (auto it = defList.begin(); it != defList.end(); ++it) {
		if (it->getStringDefinition() == def) {
			defList.erase(it);
			return;
		}
	}
}

void Word::merge(vector<Definition>& a, int l, int r, int mid) {
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

void Word::mergeSort(vector<Definition>& a, int l, int r, int n) {
	if (l > r || l == r) return;

	int mid = l + (r - l) / 2;

	mergeSort(a, l, mid, n);
	mergeSort(a, mid + 1, r, n);

	merge(a, l, r, mid);
}
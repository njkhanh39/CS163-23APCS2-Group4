#include "Word.h"


//check for emptiness
bool Word::empty() {
	if (text == "") return true;
	return false;
}

//getters

string Word::getWord() {
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

bool Word::getFavourite() {
	return isFavourite;
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

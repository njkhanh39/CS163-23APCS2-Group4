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
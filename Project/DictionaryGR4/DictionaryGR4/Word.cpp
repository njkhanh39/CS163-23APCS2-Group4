#include "Word.h"


//getters

string Word::getWord() {
	return text;
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

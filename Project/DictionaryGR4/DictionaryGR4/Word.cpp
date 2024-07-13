#include "Word.h"

string Word::getWord() {
	return text;
}


list<Definition*> Word::getDefinitions() {
	return defList;
}

list<string> Word::getStringDefinitions() {
	list<string> ans;
	for (auto& p : defList) {
		ans.push_back(p->getDefinition());
	}

	return ans;
}

void Word::addDefinition(string& def) {
	Definition* newDef = new Definition(def, this);
	defList.push_back(newDef);
}

void Word::addDefinition(Definition& def) {
	Definition* ptr = &def;
	defList.push_back(ptr);
}

#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include "Definition.h"
using namespace std;

class Definition;

class Word {
private:
	string text;
	list<Definition*> defList;
public:
	Word() {
		text = "";
	}

	Word(const string word) {
		text = word;
	}

	~Word() {
		for (auto& d : defList) delete d;
	}

	void addDefinition(string& def) {
		Definition* newDef = new Definition(def, this);
		defList.push_back(newDef);
	}
};
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

	//getters

	string getWord();

	list<Definition*> getDefinitions();

	list<string> getStringDefinitions();

	void addDefinition(string& def);

	void addDefinition(Definition& def);

};
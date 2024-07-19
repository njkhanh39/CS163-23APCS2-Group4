#pragma once
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
	list<Definition> defList;
public:
	Word() {
		text = "";
	}

	Word(const string word) {
		text = word;
	}


	//getters

	string getWord();

	
	list<Definition> getDefinitions();

	list<string> getStringDefinitions();

	//adders and setters

	void addDefinition(string& def);

	void addDefinition(Definition& def);

	void setWord(string word) {
		text = word;
	}
};
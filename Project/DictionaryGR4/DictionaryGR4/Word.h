#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include "Definition.h"
using namespace std;

class Definition;

class Word {
private:
	string text;
	vector<Definition> defList;
public:
	Word() {
		text = "";
	}

	Word(const string word) {
		text = word;
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

	bool empty() {
		if (text == "") return true;
		return false;
	}
	//getters

	string getWord();

	
	vector<Definition> getDefinitions();

	vector<string> getStringDefinitions();

	//adders and setters

	void addDefinition(string& def);

	void addDefinition(Definition& def);

	void setWord(string word) {
		text = word;
	}
};
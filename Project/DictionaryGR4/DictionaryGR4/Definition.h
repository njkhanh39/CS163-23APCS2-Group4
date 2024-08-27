#pragma once 
#include <iostream>
#include <string>
#include <algorithm>
#include <list>

using namespace std;

class Word;

class Definition {
private:
	string text;
	Word* owner;

public:
	Definition(string& defText, Word* owningWord);

	Definition() {
		text = "";
		owner = nullptr;
	}

	// Equality operator
	bool operator==(const Definition& other) const {
		return text == other.text;
	}

	// Inequality operator
	bool operator!=(const Definition& other) const {
		return !(*this == other);
	}

	// Less than operator
	bool operator<(const Definition& other) const {
		if (text < other.text) return true;
		return false;
	}

	// Greater than operator
	bool operator>(const Definition& other) const {
		if (text > other.text) return true;
		return false;
	}

	string getStringDefinition();

	void setDefinition(string newText);

};
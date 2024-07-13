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
	Definition(string& defText, Word* owningWord) {
		text = defText;
		owner = owningWord;
	}
};
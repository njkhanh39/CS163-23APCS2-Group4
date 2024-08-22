#pragma once

#include "SearchedWord.h"
#include <list>
#include <fstream>
#include <sstream>



class History {
private:
	list<SearchedWord> searchList;  // for loading
	string mode;
public:
	History() {}


	list<SearchedWord> getsearchList() {
		return searchList;
	};

	void setMode(string dataset);
	void addToHistory(SearchedWord w);
	void loadFromFile();
	void saveToFile(SearchedWord w);
	void clearHistory();
	void remove(string word);  // for removing a word

};




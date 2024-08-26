#pragma once

#include "SearchedWord.h"
#include <list>
#include <fstream>
#include <sstream>



class History {
private:
	list<SearchedWord> searchList;  // for loading
	
public:
	History() {}


	list<SearchedWord> getsearchList() {
		return searchList;
	};


	void addToHistory(SearchedWord w);
	bool loadFromFile(string activeDataset);
	void saveToFile(SearchedWord w, string activeDataset); // for saving word -> def
	void saveDevToWord(SearchedWord w, string def, string activeDataset);
	void clearHistory();
	void remove(string word);  // for removing a word

};




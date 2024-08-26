#pragma once

#include "SearchedWord.h"
#include <list>
#include <fstream>
#include <sstream>



class History {

	
	
public:
	History() {}

	list<SearchedWord> searchList;  // for loading



	void addToHistory(SearchedWord w);
	bool loadFromFile(string activeDataset);
	void saveToFile(SearchedWord w, string activeDataset); // for saving word -> def
	void saveDevToWord(SearchedWord w, string def, string activeDataset);
	void clearHistory();
	void remove(string word);  // for removing a word

};




#pragma once

#include "SearchedWord.h"
#include <list>
#include <fstream>




class History {
private:
	list<SearchedWord> searchList;
public:
	History() {}


	list<SearchedWord> getsearchList() {
		return searchList;
	};


	void addToHistory(SearchedWord w);
	void loadFromFile();
	void saveToFile();
	void clearHistory();

};




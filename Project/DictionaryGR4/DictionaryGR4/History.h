#pragma once

#include "SearchedWord.h"
#include <list>




class History {
private:
	list<SearchedWord> searchList;
public:
	History() {}


	list<SearchedWord> getsearchList() {
		return searchList;
	};


	void addToHistory(SearchedWord w);

};




#pragma once
#include"Word.h"
#include "SearchTree.h"
#include <windows.h>
#include <sstream>
#include <iomanip>


class SearchedWord : public Word {
private:
	SYSTEMTIME st;
	string date;
	string time;
	ostringstream dateStream;
	ostringstream timeStream;
public:
	SearchedWord(const string word) {
		setWord(word);
	}
	SearchedWord() {
		setWord("");
	}
	void setDate();
	void setTime();

	string getDate();
	string getTime();
};


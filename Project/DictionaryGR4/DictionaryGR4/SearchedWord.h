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

	SearchedWord(string word) {
		setWord(word);
	}

	SearchedWord() {
		setWord("");
	}

    // Copy constructor

	SearchedWord(const SearchedWord& other);

    // Copy assignment operator
    SearchedWord& operator=(const SearchedWord& other) {
        if (this != &other) {
            Word::operator=(other);
            st = other.st;
            date = other.date;
            time = other.time;
        }
        return *this;
    }

	void setDate();
	void setTime();
	void setDate(string date);
	void setTime(string time);
	string getDate();
	string getTime();
};


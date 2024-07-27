#include "SearchedWord.h"


SearchedWord::SearchedWord(const SearchedWord& other) : Word(other) {
	st = other.st;
	date = other.date;
	time = other.time;
}

string SearchedWord::getDate() {
	return date;
}

string SearchedWord::getTime() {
	return time;
}

void SearchedWord::setDate() {
	GetLocalTime(&st);
	dateStream << setw(2) << setfill('0') << st.wDay << "/" << setw(2) << std::setfill('0') << st.wMonth << "/" << st.wYear;
    date = dateStream.str();
}

void SearchedWord::setTime() {
	GetLocalTime(&st);
	timeStream << setw(2) << setfill('0') << st.wHour << ":" << setw(2) << setfill('0') << st.wMinute;
	time = timeStream.str();
}

void SearchedWord::setDate(string date) {
	this->date = date;
}

void SearchedWord::setTime(string time) {
	this->time = time;
}
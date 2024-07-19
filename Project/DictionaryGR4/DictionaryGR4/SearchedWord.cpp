#include "SearchedWord.h"

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
#include "History.h"





void History::addToHistory(SearchedWord w) {
	searchList.push_back(w);
}

void History::saveToFile(SearchedWord w) {
	ofstream out;
	out.open("History/history.csv", ios::app);
	if (!out.is_open()) return;
	out << w.getWord() << "," << w.getTime() << "," << w.getDate() << endl;
	out.close();
}

void History::loadFromFile() {

}

void History::clearHistory() {

}
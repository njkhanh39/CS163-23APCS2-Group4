#include "History.h"


void History::setMode(string dataset) {
	mode = dataset;
}

void History::addToHistory(SearchedWord w) {
	searchList.push_back(w);
}

void History::saveToFile(SearchedWord w) {
	ofstream out;
	out.open("History\\" + mode + "\\history.csv", ios::app);
	if (!out.is_open()) return;
	out << w.getWord() << "," << w.getTime() << "," << w.getDate() << endl;
	out.close();
}

void History::loadFromFile() {
	ifstream in;
	in.open("History\\" + mode + "\\history.csv");
	string line;
	getline(in, line);   // ignore the first line

	while (getline(in, line)) {
		SearchedWord word;
		stringstream ss(line); 
		string temp;

		getline(ss, temp, ',');
		word.setWord(temp);


		getline(ss, temp, ',');
		word.setTime(temp);


		getline(ss, temp, ',');
		word.setDate(temp);

		searchList.push_back(word);

	}

	searchList.reverse();

}

void History::clearHistory() {

}

void History::remove(string word) {

}
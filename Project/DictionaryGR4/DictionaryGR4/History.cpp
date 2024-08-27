#include "History.h"




void History::addToHistory(SearchedWord w) {
	searchList.push_back(w);
}

void History::saveToFile(SearchedWord w, string activeDataset) {
	ofstream out;
	out.open("History\\" + activeDataset + "\\history.csv", ios::app);
	if (!out.is_open()) return;
	out << w.getText() << "," << w.getTime() << "," << w.getDate() << endl;
	out.close();
}

bool History::loadFromFile(string activeDataset) {
	searchList.clear();
	ifstream in;
	in.open("History\\" + activeDataset + "\\history.csv");
	if (!in.is_open()) {
		return false;
	}

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
	return !searchList.empty();
}

bool History::clearHistory(string activeDataset) {
	ofstream out;
	out.open("History\\" + activeDataset + "\\history.csv");
	if (!out.is_open()) {
		return false;
	}
	out << "word,time,date" << endl;
	out.close();
	return true;
}

void History::remove(string word) {

}


void History::saveDevToWord(SearchedWord w, string def, string activeDataset) {
	ofstream out;
	out.open("History\\" + activeDataset + "\\histDevWord.csv", ios::app);
	if (!out.is_open()) return;
	out << w.getText() << "," << def << "," << w.getTime() << "," << w.getDate() << endl;
	out.close();


}
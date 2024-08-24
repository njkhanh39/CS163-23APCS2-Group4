#pragma once

#include <wx/wx.h>
#include <chrono>
#include <random>

#include "SearchTree.h"
#include "History.h"
#include <list>

class Dictionary {
private:
	WordFinder* activeSearcher = nullptr; //for def -> word in all datasets
	WordFinder toolEngEng, toolEngVie, toolVieEng;

	Trie myTrie; //for word -> def in Eng-Eng & Eng-Vie

	const string EngEng = "Eng-Eng", EngVie = "Eng-Vie", VieEng = "Vie-Eng"; //datasets
	string activeDataSet = EngEng; //changeable
	History hist;
	list<string> favList;
	
public:
	bool isSearchingDefinition = false;
	Dictionary() {
		activeSearcher = &toolEngEng;
	}
	~Dictionary() {
		//saveToFile();
	}

	//"Eng-Eng", "Eng-Vie", "Vie-Eng"
	bool chooseLanguage(string t);

	//Pass in search bar's current word, true/false for LogMessaging on Status Bar.
	void runSearchEngine(string word, bool yesLogMessage);

	//after calling, searchDefinitions function is ready.Turning off will disable that function.
	void runSearchDefinitionEngine();

	void turnOffSearchDefinitionEngine();

	//save changed features. Automatically call when ending program
	void saveToFile();

	//user may delete words, those which are saved in a file, we will ignore these
	bool getAvailableWords(Word& w);

	string getActiveDataset();

	//get a "Word" object that matches a string
	Word searchWordMatching(string word);

	Word* getWordPtr(string word);

	Word getRandomWord(string& wordText);

	//return definitions of keyword
	vector<Definition> searchDefinitions(string word);

	//return definitions of keyword as strings
	vector<string> searchStringDefinitions(string word);

	//search for words with same prefix + limit number of words
	vector<Word> searchRelatedWords(string word, int limit);

	//this one needs runSearchDefinitionsEngine
	vector<Word> searchDefToWord(string& keyword, int limit);



	void addToFavourite(Word& word);

	History getHistory();

	//setters & adders

	bool editDefInFile(string text, string olddef, string newdef, string dir);

	void editDefOnWordFinder(string text, string olddef, string newdef);

	void editDefinition(string text, string olddef, string newdef);

	void deleteWord(Word& word);

	int	deleteWordOneDef(string& text, string& def);

	void addNewWord(Word& newWord);

	//return true if add word successfully
	bool addNewWordOneDef(string& text, string& def);

	//helpers

	vector<string> transformSentence(string& input);
	
	void EngineHelper(string keyword, bool yesLogMessage);

	string mapStringToFile(string word);

	void sortVectorString(vector<string>& vec) {
		int n = vec.size();
		mergeSort(vec, 0, n - 1, n);
	}

	void merge(vector<string>& a, int l, int r, int mid);

	void mergeSort(vector<string>& a, int l, int r, int n);

	//vector<Word> helperDefToWordENGENG(string keyword, int limit){
	//	//format word
	//	string word;

	//	vector<Word> ans;

	//	if (keyword.empty()) return ans;

	//	int i = 0, n = (int)keyword.length();
	//	while (i<n && keyword[i] == ' ') ++i;

	//	if (i == n) return ans;

	//	for (int j = i; j < n; ++j) {
	//		if (j < n - 1 && (keyword[j] == ' ' && keyword[j + 1] == ' ')) continue;
	//		if (j == n - 1 && keyword[j] == ' ') continue;
	//		word.push_back(tolower(keyword[j]));
	//	}


	//	//loop through all files

	//	for (int file = 1; file <= 28; ++file) {
	//		fstream fin;
	//		fin.open("DataSet\\" + to_string(file) + ".txt");

	//		if (!fin.is_open()) {
	//			fin.close();
	//			continue;
	//		}

	//		string line;
	//		while (getline(fin, line)) {

	//			string s, t, u;
	//			int i = 0;
	//			while (line[i] != '\t') {
	//				s.push_back(line[i]);
	//				++i;
	//			}

	//			++i;

	//			//format to lowercase, remove commas and dots
	//			for (int j = i; j < (int)line.length(); ++j) {
	//				if(line[j]!=',' && line[j]!=';' && line[j]!='.') t.push_back(tolower(line[j]));
	//				u.push_back(line[j]);
	//			}

	//			
	//			if (better_wordcheck(t, word)) {
	//				Word w; w.setWord(s); w.addDefinition(u);
	//				
	//				ans.push_back(w);
	//				if (ans.size() == limit) {
	//					fin.close();
	//					return ans;
	//				}
	//			}
	//		}

	//		fin.close();
	//	}

	//	return ans;
	//}

	//bool wordcheck(string& a, string& b, int position) {
	//	for (int i = 0; i < (int)a.length(); ++i) {
	//		if (i == 0 || (a[i] != ' ' && a[i - 1] == ' ')) {
	//			bool S = true;
	//			int j = i, k = position;
	//			for (j, k; k != (int)b.length() && b[k] != ' '; ++j, ++k) {
	//				if (j == (int)a.length() || a[j] == ' ') {
	//					S = false;
	//					break;
	//				}
	//				if (a[j] != b[k]) {
	//					S = false;
	//					break;
	//				}
	//			}
	//			if (a[j] != ' ' && j < (int)a.length()) S = false;
	//			if (S) return true;
	//		}
	//	}
	//	return false;
	//}
	//bool better_wordcheck(string& a, string& b) {
	//	vector<int> pos;
	//	int spacecount = 0;
	//	for (int i = 0; i < (int)b.length(); ++i) {
	//		if (b[i] == ' ') ++spacecount;
	//		if (i == 0 || (b[i - 1] == ' ')) pos.push_back(i);
	//	}
	//	int i = 0;
	//	for (int i = 0; i <= spacecount; ++i) {
	//		if (!wordcheck(a, b, pos[i])) return false;
	//	}
	//	return true;
	//}
};
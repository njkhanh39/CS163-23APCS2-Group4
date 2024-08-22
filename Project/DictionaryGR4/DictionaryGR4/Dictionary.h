#pragma once

#include <wx/wx.h>

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

	//get a "Word" object that matches a string
	Word searchWordMatching(string word);

	Word* getWordPtr(string word);

	Word* getRandomWord(string& wordText, string activeDataset);

	//return definitions of keyword
	vector<Definition> searchDefinitions(string word);

	//return definitions of keyword as strings
	vector<string> searchStringDefinitions(string word);

	//search for words with same prefix + limit number of words
	vector<Word> searchRelatedWords(string word, int limit);

	//this one needs runSearchDefinitionsEngine
	vector<Word> searchDefToWord(string& keyword, int limit);







	void resetDictionary();
	void addToFavourite();

	History getHistory();

	//setters & adders

	void editDefinition(string text, string def) {
		
	}

	void addNewWord(Word& newWord) {
		string text = newWord.getWord();
		for (auto& def : newWord.getDefinitions()) {
			string defStr = def.getStringDefinition();
			addNewWordOneDef(text, defStr);
		}
	}

	bool addNewWordOneDef(string& text, string& def) {
		//find and delete in deleted.txt
		
		ifstream fin;
		vector<string> deletedFile; string line; bool found = false;
		fin.open("DataSet\\" + activeDataSet + "\\deletedWords.txt");

		while (getline(fin, line)) {
			string lineText, lineDef;
			int i = 0;
			while (line[i] != '\t') {
				lineText.push_back(line[i]);
				++i;
			}
			++i;
			
			lineDef = line.substr(i);

			//found the word in deleted.txt
			if (lineText == text && lineDef == def) {
				found = true;
				continue;
			}

			deletedFile.push_back(line);
		}

		fin.close();

		if (found == true) { //remove the found line in deletedWords.txt
			ofstream fout;
			fout.open("DataSet\\" + activeDataSet + "\\deletedWords.txt");

			for (auto& str : deletedFile) {
				fout << str << '\n';
			}

			fout.close();

			return true; //success adding
		}

		//check if word already existed

		Word match = searchWordMatching(text);

		if (match.findDefinition(def) != -1) {
			return false; //word already exists
		}

		//couldnt find word, now we add it to wordfinder

		activeSearcher->addNewWord(text, def);


		//append the word to addedwords.txt and addsorted.txt

		ofstream fout;

		fout.open("DataSet\\" + activeDataSet + "\\addedWords.txt", ios::app);

		fout << text << '\t' << def << '\n';

		fout.close();

		vector<string> sortedDef = transformSentence(def);

		//remember to sort
		sortVectorString(sortedDef);
		

		fout.open("DataSet\\" + activeDataSet + "\\sortedAddedWords.txt", ios::app);

		fout << text << '\t';
		for (int i = 0; i < (int)sortedDef.size(); ++i) {
			fout << sortedDef[i];
			if (i + 1 == (int)sortedDef.size()) {
				fout << '\n';
			}
			else fout << ' ';
		}

		fout.close();

		return true;
	}

	//helpers

	vector<string> transformSentence(string& input);
	
	void EngineHelper(string keyword, bool yesLogMessage);

	string mapStringToFile(string word);

	void sortVectorString(vector<string>& vec) {
		int n = vec.size();
		mergeSort(vec, 0, n - 1, n);
	}

	void merge(vector<string>& a, int l, int r, int mid) {
		vector<string> temp(r - l + 1);

		int ptr1 = l, ptr2 = mid + 1, cur = 0;

		while (ptr1 <= mid && ptr2 <= r) {
			if (a[ptr1] < a[ptr2])
				temp[cur++] = a[ptr1++];
			else
				temp[cur++] = a[ptr2++];
		}

		while (ptr1 <= mid) temp[cur++] = a[ptr1++];
		while (ptr2 <= r) temp[cur++] = a[ptr2++];

		for (int i = l, cnt = 0; i <= r; ++i) a[i] = temp[cnt++];
	}

	void mergeSort(vector<string>& a, int l, int r, int n) {
		if (l > r || l == r) return;

		int mid = l + (r - l) / 2;

		mergeSort(a, l, mid, n);
		mergeSort(a, mid + 1, r, n);

		merge(a, l, r, mid);
	}

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
#pragma once
#include "SearchTree.h"
#include <wx/wx.h>

class Dictionary {
private:
	Trie myTrie; //for word -> def in Eng-Eng
	const string EngEng = "DataSetEngEng", EngVie = "DataSetEngVie", VieEng = "DataSetVieEng"; //datasets

	string activeDataSet = EngEng; //can be changeable
public:
	// string = DataSetEngEng, DataSetEngVie, DataSetVieEng
	bool chooseLanguage(string t) {
		if (t != EngEng && t != EngVie && t != VieEng) return false;
		activeDataSet = t;
		return true;
	}

	//Pass in search bar's current word, true/false for LogMessaging on Status Bar.
	void runSearchEngine(string word, bool yesLogMessage) {
		if (activeDataSet == EngEng) EngineHelperENG_ENG(word, yesLogMessage);
	}

	void EngineHelperENG_ENG(string word, bool yesLogMessage) {
		//format the word

		for (auto& c : word) c = tolower(c);

		if ((int)word.length() == 1 && myTrie.empty()) {
			char key = word[0];
			myTrie.loadDataEngEng(key);
			wxLogStatus("Loading data...");
		}
		else if ((int)word.length() == 0) {
			if (!myTrie.empty()) myTrie.clear();
			wxLogStatus("Clearing Search Tree...");
		}
		else {
			wxLogStatus("Doing nothing...");
		}
	}

	list<Definition> searchDefinitions(string word) {
		list<Definition> ans;
		if (activeDataSet == EngEng) return myTrie.getDefinitions(word);
		return ans;
	}

	list<string> searchStringDefinitions(string word) {
		list<string> ans;
		if (activeDataSet == EngEng) return myTrie.getStringDefinitions(word);
		return ans;
	}

	//only list 10 related words
	list<Word> relatedWord(string word, int desired) {
		list<Word> ans;
		//format word
		for (auto& x : word) x = tolower(x);

		if (activeDataSet == EngEng) return myTrie.getWordsWithPrefix(word, desired);
		return ans;
	}

	list<Word> searchDefToWord(string word){
		//format word
		for (auto& x : word) x = tolower(x);

		list<Word> ans;

		//loop through all files

		for (int file = 1; file <= 28; ++file) {
			fstream fin;
			fin.open("DataSet\\" + to_string(file) + ".txt");

			if (!fin.is_open()) {
				fin.close();
				continue;
			}

			string line;
			while (getline(fin, line)) {

				string s, t;
				int i = 0;
				while (line[i] != '\t') {
					s.push_back(line[i]);
					++i;
				}

				++i;

				//format to lowercase
				for (int j = i; j < (int)line.length(); ++j) {
					t.push_back(tolower(line[j]));
				}

				
				if (better_wordcheck(t, word)) {
					Word w; w.setWord(s); w.addDefinition(t);
					
					ans.push_back(w);
					if (ans.size() == 20) {
						fin.close();
						return ans;
					}
				}
			}

			fin.close();
		}

		return ans;
	}

private:
	bool wordcheck(string& a, string& b, int position) {
		for (int i = 0; i < (int)a.length(); ++i) {
			if (i == 0 || (a[i] != ' ' && a[i - 1] == ' ')) {
				bool S = true;
				int j = i, k = position;
				for (j, k; k != (int)b.length() && b[k] != ' '; ++j, ++k) {
					if (j == (int)a.length() || a[j] == ' ') {
						S = false;
						break;
					}
					if (a[j] != b[k]) {
						S = false;
						break;
					}
				}
				if (a[j] != ' ' && j < (int)a.length()) S = false;
				if (S) return true;
			}
		}
		return false;
	}
	bool better_wordcheck(string& a, string& b) {
		vector<int> pos;
		int spacecount = 0;
		for (int i = 0; i < (int)b.length(); ++i) {
			if (b[i] == ' ') ++spacecount;
			if (i == 0 || (b[i - 1] == ' ')) pos.push_back(i);
		}
		int i = 0;
		for (int i = 0; i <= spacecount; ++i) {
			if (!wordcheck(a, b, pos[i])) return false;
		}
		return true;
	}
};
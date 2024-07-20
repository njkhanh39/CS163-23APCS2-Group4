#pragma once
#include "SearchTree.h"
#include <wx/wx.h>

class Dictionary {
private:
	WordFinder tool;
	Trie myTrie; //for word -> def in Eng-Eng
	const string EngEng = "DataSetEngEng", EngVie = "DataSetEngVie", VieEng = "DataSetVieEng"; //datasets

	string activeDataSet = EngEng; //can be changeable
	
public:
	bool isToolLoaded = false;
	Dictionary() {
		//tool.load(EngEng);
	}

	void load() {
		tool.load(EngEng);
		isToolLoaded = true;
	}



	//string = DataSetEngEng, DataSetEngVie, DataSetVieEng
	bool chooseLanguage(string t) {
		if (t != EngEng && t != EngVie && t != VieEng) return false;
		activeDataSet = t;
		return true;
	}

	//Pass in search bar's current word, true/false for LogMessaging on Status Bar.
	void runSearchEngine(string word, bool yesLogMessage) {
		if (activeDataSet == EngEng) EngineHelperENG_ENG(word, yesLogMessage);
	}

	//return definitions of keyword
	vector<Definition> searchDefinitions(string word) {
		vector<Definition> ans;
		if (activeDataSet == EngEng) return myTrie.getDefinitions(word);
		return ans;
	}

	//return definitions of keyword as strings
	vector<string> searchStringDefinitions(string word) {
		vector<string> ans;
		if (activeDataSet == EngEng) return myTrie.getStringDefinitions(word);
		return ans;
	}

	//search for words with same prefix + limit number of words
	vector<Word> searchRelatedWords(string word, int limit) {
		vector<Word> ans;
		//format word
		for (auto& x : word) x = tolower(x);

		if (activeDataSet == EngEng) return myTrie.getWordsWithPrefix(word, limit);
		return ans;
	}

	//search def -> word + limit number of words
	vector<Word> searchDefToWord(string keyword, int limit) {
		//formatting is done in helper

		vector<Word> ans;

		if (activeDataSet == EngEng) return helperDefToWordENGENG(keyword, limit);

		return ans;
	}

	vector<Word> searchDefToWord2(string keyword, int limit) {
		return tool.find(keyword, limit);
	}
	

private:
	//helpers
	
	void EngineHelperENG_ENG(string keyword, bool yesLogMessage) {
		//format the word
		string word;
		int i = 0, n = keyword.length();
		while (i < n && keyword[i] == ' ') ++i;


		for (int j = 0; j < n; ++j) {
			word.push_back(tolower(keyword[j]));
		}

		int m = word.length(); --m;

		while (m >= 0 && word[m] == ' ') word.pop_back();

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
	vector<Word> helperDefToWordENGENG(string keyword, int limit){
		//format word
		string word;

		vector<Word> ans;

		if (keyword.empty()) return ans;

		int i = 0, n = (int)keyword.length();
		while (i<n && keyword[i] == ' ') ++i;

		if (i == n) return ans;

		for (int j = i; j < n; ++j) {
			if (j < n - 1 && (keyword[j] == ' ' && keyword[j + 1] == ' ')) continue;
			if (j == n - 1 && keyword[j] == ' ') continue;
			word.push_back(tolower(keyword[j]));
		}


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

				string s, t, u;
				int i = 0;
				while (line[i] != '\t') {
					s.push_back(line[i]);
					++i;
				}

				++i;

				//format to lowercase, remove commas and dots
				for (int j = i; j < (int)line.length(); ++j) {
					if(line[j]!=',' && line[j]!=';' && line[j]!='.') t.push_back(tolower(line[j]));
					u.push_back(line[j]);
				}

				
				if (better_wordcheck(t, word)) {
					Word w; w.setWord(s); w.addDefinition(u);
					
					ans.push_back(w);
					if (ans.size() == limit) {
						fin.close();
						return ans;
					}
				}
			}

			fin.close();
		}

		return ans;
	}

	

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
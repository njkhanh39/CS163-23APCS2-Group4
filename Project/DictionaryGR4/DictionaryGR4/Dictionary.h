#pragma once
#include "SearchTree.h"
#include "History.h"
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

	//Pass in search bar's current word, true/false for LogMessaging on Status Bar
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
		if (activeDataSet == EngEng) return myTrie.getWordsWithPrefix(word, desired);
		return ans;
	}

};
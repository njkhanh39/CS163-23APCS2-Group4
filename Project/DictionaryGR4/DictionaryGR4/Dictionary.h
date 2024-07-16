#pragma once
#include "SearchTree.h"
#include <wx/wx.h>

class Dictionary {
private:
	Trie myTrie; //for word -> def in Eng-Eng
	const string EngEng = "DataSetEngEng", EngVie = "DataSetEngVie", VieEng = "DataSetVieEng"; //datasets

	string activeDataSet = EngEng; //changeable
public:
	// string = DataSetEngEng, DataSetEngVie, DataSetVieEng
	bool chooseLanguage(string t);

	//Pass in search bar's current word, true/false for LogMessaging on Status Bar
	void runSearchEngine(string word, bool yesLogMessage);

	void EngineHelperENG_ENG(string word, bool yesLogMessage);

	list<Definition> searchDefinitions(string word);

	list<string> searchStringDefinitions(string word);

	//only list 10 related words
	list<Word> relatedWord(string keyword, int desired);

};
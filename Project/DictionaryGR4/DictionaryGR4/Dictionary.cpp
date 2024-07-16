#include "Dictionary.h"
bool Dictionary::chooseLanguage(string t) {
	if (t != EngEng && t != EngVie && t != VieEng) return false;
	activeDataSet = t;
	return true;
}

//Pass in search bar's current word, true/false for LogMessaging on Status Bar
void Dictionary::runSearchEngine(string word, bool yesLogMessage) {
	if (activeDataSet == EngEng) EngineHelperENG_ENG(word, yesLogMessage);
}

void Dictionary::EngineHelperENG_ENG(string word, bool yesLogMessage) {
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

list<Definition> Dictionary::searchDefinitions(string word) {
	list<Definition> ans;
	if (activeDataSet == EngEng) return myTrie.getDefinitions(word);
	return ans;
}

list<string> Dictionary::searchStringDefinitions(string word) {
	list<string> ans;
	if (activeDataSet == EngEng) return myTrie.getStringDefinitions(word);
	return ans;
}

//list a desired number of words that has same prefix with a keyword
list<Word> Dictionary::relatedWord(string keyword, int desired) {
	list<Word> ans;
	if (activeDataSet == EngEng) return myTrie.getWordsWithPrefix(keyword, desired);
	return ans;
}
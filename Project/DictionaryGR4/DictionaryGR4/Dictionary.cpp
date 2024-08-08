#include "Dictionary.h"

using namespace std;

//"Eng-Eng", "Eng-Vie", "Vie-Eng"
bool Dictionary::chooseLanguage(string t) {
	if (t != EngEng && t != EngVie && t != VieEng) return false;
	activeDataSet = t;
	return true;
}

//Pass in search bar's current word, true/false for LogMessaging on Status Bar.
void Dictionary::runSearchEngine(string word, bool yesLogMessage) {
	if (activeDataSet == EngEng) EngineHelperENG_ENG(word, yesLogMessage);
	else if (activeDataSet == EngVie) EngineHelperENG_VIE(word, yesLogMessage);
}

//after calling, searchDefinitions function is ready.Turning off will disable that function.
void Dictionary::runSearchDefinitionEngine() {
	if (activeDataSet == EngEng) tool.load("Eng-Eng", activeData);
	if (activeDataSet == EngVie) tool.load("Eng-Vie", activeData);
	if (activeDataSet == VieEng) tool.load("Vie-Eng", activeData);
	isSearchingDefinition = true;
}

void Dictionary::turnOffSearchDefinitionEngine() {
	tool.unload();
	isSearchingDefinition = false;
}

//get a "Word" object that matches a string
Word Dictionary::searchWordMatching(string word) {
	Word w;
	//format word
	for (auto& x : word) x = tolower(x);

	if (activeDataSet == EngEng) {
		if (w.empty() && isSearchingDefinition) return tool.searchWord(word);
		else w = myTrie.getWordMatching(word);
	}
	if (activeDataSet == EngVie) {
		if (w.empty() && isSearchingDefinition) return tool.searchWord(word);
		w = myTrie.getWordMatching(word);
	}
	return w;
}

//return definitions of keyword
vector<Definition> Dictionary::searchDefinitions(string word) {
	//format word
	for (auto& x : word) x = tolower(x);
	if (activeDataSet == EngEng || activeDataSet == EngVie) return myTrie.getDefinitions(word);
	vector<Definition> ans;
	return ans;
}

//return definitions of keyword as strings
vector<string> Dictionary::searchStringDefinitions(string word) {
	//format word
	for (auto& x : word) x = tolower(x);
	if (activeDataSet == EngEng || activeDataSet == EngVie) return myTrie.getStringDefinitions(word);
	vector<string> ans;
	return ans;
}

//search for words with same prefix + limit number of words
vector<Word> Dictionary::searchRelatedWords(string word, int limit) {
	vector<Word> ans;
	//format word
	for (auto& x : word) x = tolower(x);

	if (activeDataSet == EngEng || activeDataSet == EngVie) return myTrie.getWordsWithPrefix(word, limit);
	return ans;
}

//this one needs runSearchDefinitionsEngine
vector<Word> Dictionary::searchDefToWord(string& keyword, int limit) {
	vector<string> subkeys = transformSentence(keyword);
	return tool.searchDefinitionsToWord(subkeys, limit);
}

void Dictionary::duplicateDataset() {
	if (!filesystem::exists("Customized DataSet"))
		filesystem::copy("DataSet", "Customized DataSet");
	activeData = "Customized DataSet";
}

void Dictionary::deleteDataset() {
	if (filesystem::exists("Customized DataSet"))
		filesystem::remove_all("Customized DataSet");
	activeData = "DataSet";
}

//helpers

vector<string> Dictionary::transformSentence(string& input) {
	string intermediate;
	intermediate.reserve(input.size());

	// Transform to lowercase and remove dots, commas, and semicolons
	for (char ch : input) {
		if (ch != '.' && ch != ',' && ch != ';') {
			intermediate += tolower(ch);
		}
	}

	// Remove redundant spaces and split into words
	istringstream iss(intermediate);
	vector<string> words;
	string word;

	while (iss >> word) {
		words.push_back(word);
	}

	return words;
}

void Dictionary::EngineHelperENG_ENG(string keyword, bool yesLogMessage) {
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
		myTrie.loadData(key, EngEng, activeData);
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

void Dictionary::EngineHelperENG_VIE(string keyword, bool yesLogMessage) {
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
		myTrie.loadData(key, EngVie, activeData);
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
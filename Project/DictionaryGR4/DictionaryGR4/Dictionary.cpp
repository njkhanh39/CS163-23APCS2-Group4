#include "Dictionary.h"

using namespace std;

//"Eng-Eng", "Eng-Vie", "Vie-Eng"
bool Dictionary::chooseLanguage(string t) {
	if (t != EngEng && t != EngVie && t != VieEng) return false;
	activeDataSet = t;
	
	if (t == EngEng) activeSearcher = &toolEngEng;
	if (t == EngVie) activeSearcher = &toolEngVie;
	if (t == VieEng) activeSearcher = &toolVieEng;

	hist.setMode(activeDataSet);
	return true;
}

//Pass in search bar's current word, true/false for LogMessaging on Status Bar.
void Dictionary::runSearchEngine(string word, bool yesLogMessage) {
	EngineHelper(word, yesLogMessage);
}

//after calling, searchDefinitions function is ready.Turning off will disable that function.
void Dictionary::runSearchDefinitionEngine() {
	if (!isSearchingDefinition) {
		toolEngEng.load("Eng-Eng");
		toolEngVie.load("Eng-Vie");
		toolVieEng.load("Vie-Eng");
		isSearchingDefinition = true;
	}
}

void Dictionary::turnOffSearchDefinitionEngine() {
	toolEngEng.unload();
	toolEngVie.unload();
	toolVieEng.unload();
	isSearchingDefinition = false;
}

void Dictionary::saveToFile() {
	toolEngEng.saveToFile("Eng-Eng");
	toolEngVie.saveToFile("Eng-Vie");
	toolVieEng.saveToFile("Vie-Eng");
}

//get a "Word" object that matches a string
Word Dictionary::searchWordMatching(string word) {
	Word w;
	//format word
	for (auto& x : word) x = tolower(x);

	
	if (!myTrie.empty()) {
		w = myTrie.getWordMatching(word);
		if(w.empty()) w = activeSearcher->searchWord(word);
	}
	else w = activeSearcher->searchWord(word);
	 
	
	//since some of w's definitions may be deleted by user, we have to check.
	getAvailableWords(w);

	return w;
}

Word* Dictionary::getWordPtr(string word) {
	return myTrie.getWordPointer(word);
}

Word* Dictionary::getRandomWord(string& wordText, string dataset) {
	string dir, temp;
	if (dataset == "Eng-Eng")
		temp = to_string(rand() % 28 + 1);

	else if (dataset == "Eng-Vie") {
		int t = rand() % 29 + 1;
		if (t == 29)
			temp = "number";
		else
			temp = to_string(t);
	}

	else if (dataset == "Vie-Eng")
		temp = to_string(rand() % 25 + 1);

	dir = "DataSet\\" + dataset + "\\" + temp + ".txt";

	myTrie.loadData(dir, dataset);

	return myTrie.getRandomWord(wordText, dataset);
}

//return definitions of keyword
vector<Definition> Dictionary::searchDefinitions(string word) {
	//format word
	for (auto& x : word) x = tolower(x);
	vector<Definition> ans = myTrie.getDefinitions(word);

	Word newWord;
	newWord.setWord(word);
	for (auto& s : ans) newWord.addDefinition(s);

	//trim out all deleted definitions in deleted.txt
	getAvailableWords(newWord);

	return newWord.getDefinitions();
}

//return definitions of keyword as strings
vector<string> Dictionary::searchStringDefinitions(string word) {
	//format word
	for (auto& x : word) x = tolower(x);
	vector<string> ans = myTrie.getStringDefinitions(word);

	Word newWord;
	newWord.setWord(word);
	for (auto& s : ans) newWord.addDefinition(s);

	//trim out all deleted definitions in deleted.txt
	getAvailableWords(newWord);

	return newWord.getStringDefinitions();
}

//search for words with same prefix + limit number of words
vector<Word> Dictionary::searchRelatedWords(string word, int limit) {
	vector<Word> ans;
	//format word
	for (auto& x : word) x = tolower(x);

	ans = myTrie.getWordsWithPrefix(word, limit);

	//trim out deleted words in deleted.txt
	for (int i = 0; i < (int)ans.size(); ++i) {
		getAvailableWords(ans[i]);
		if (ans[i].empty()) {
			swap(ans[i], ans.back());
			ans.pop_back();
		}
	}

	return ans;
}

//this one needs runSearchDefinitionsEngine
vector<Word> Dictionary::searchDefToWord(string& keyword, int limit) {
	vector<string> subkeys = transformSentence(keyword);
	vector<Word> ans;
	ans = activeSearcher->searchDefinitionsToWord(subkeys, limit);

	//trim out deleted words in deleted.txt
	for (int i = 0; i < (int)ans.size(); ++i) {
		getAvailableWords(ans[i]);
		if (ans[i].empty()) {
			swap(ans[i], ans.back());
			ans.pop_back();
		}
	}

	return ans;
}

//helpers

bool Dictionary::getAvailableWords(Word& w) {
	string file = "DataSet\\" + activeDataSet + "\\deletedWords.txt";

	ifstream fin;
	fin.open(file);

	string line;
	while (getline(fin, line)) {
		int i = 0;
		string comp;
		while (line[i] != '\t') {
			comp.push_back(line[i]);
			++i;
		}
		++i;

		//match
		if (comp == w.getWord()) {
			char check = line[i];
			if (check == '*') {
				w.clear();
				break;
			}

			string def = line.substr(i);

			w.removeDefinition(def);
		}
	}

	fin.close();

	if (w.getNumberOfDefinitions() == 0) w.clear();

	return true;
}

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

string Dictionary::mapStringToFile(string word) {
	string file; string temp;

	auto it = word.begin();
	uint32_t codepoint = utf8::next(it, word.end());


	int num = -1;
	char16_t c = (char16_t)codepoint;

	//handling the char

	if (codepoint == 39) {
		num = 1; //1.txt = '
	}
	if (codepoint == 45) {
		num = 2; //2.txt = "-"
	}
	if (97 <= codepoint && codepoint <= 122) {
		//eng chars
		num = myTrie.codePointToIndex(codepoint) - 10;
		if (activeDataSet == "Vie-Eng") {
			num -= 2;
			if (c == 'z') num = 25;
			if (c == 'y') num = 24;
			if (c == 'x') num = 23;
		}
	}
	int tmp = myTrie.codePointToIndex(codepoint);
	if (39 <= tmp && tmp <= 105) {
		//vie chars

		int idx = activeSearcher->getIndex(c);
		if (idx < activeSearcher->getIndex('b')) {
			//variations of 'a'
			num = 1;
		}
		else if (idx < activeSearcher->getIndex('e')) {
			// variations of 'd'
			num = 4;
		}
		else if (idx < activeSearcher->getIndex('f')) {
			//variations of 'e'
			num = 5;
		}
		else if (idx < activeSearcher->getIndex('j')) {
			//variations of 'i'
			num = 9;
		}
		else if (idx < activeSearcher->getIndex('p')) {
			//variations of 'o'
			num = 15;
		}
		else if (idx < activeSearcher->getIndex('v')) {
			//variations of 'u'
			num = 21;
		}
		else {
			//variations of 'y'
			num = 24;
		}

	}
    temp = to_string(num);

	file = "DataSet\\" + activeDataSet + "\\" + temp + ".txt";

	return file;
}

void Dictionary::EngineHelper(string keyword, bool yesLogMessage) {
	//format the word
	string word;
	int i = 0, n = keyword.length();
	while (i < n && keyword[i] == ' ') ++i;


	for (int j = 0; j < n; ++j) {
		word.push_back(tolower(keyword[j]));
	}

	int m = word.length(); --m;

	while (m >= 0 && word[m] == ' ') word.pop_back();

	int realLength = 0;

	auto it = word.begin();

	while (it != word.end()) {
		uint32_t codepoint = utf8::next(it, word.end());
		++realLength;
	}

	//get word's first character for loading

	string file; //load using this one

	string file2 = "DataSet\\" + activeDataSet + "\\addedWords.txt";

	//crucial
	if (!word.empty()) {
		file = mapStringToFile(word);
	}
	

	if (realLength == 1 && myTrie.empty()) {
		myTrie.loadData(file,activeDataSet);
		wxLogStatus("Loading data...");
	}
	else if (realLength == 0) {
		if (!myTrie.empty()) {
			myTrie.clear();
		}
		wxLogStatus("Clearing Search Tree...");
	}
	else {
		wxLogStatus("Doing nothing...");
	}
}

History Dictionary::getHistory() {
	return hist;
}

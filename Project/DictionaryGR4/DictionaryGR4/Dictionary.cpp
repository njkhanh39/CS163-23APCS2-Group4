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

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
int RandInt(int l, int r)
{
	return l + rng() % (r - l + 1);
}

Word Dictionary::getRandomWord(string& wordText) {
	srand(time(NULL));
	string text;
	while (text.empty()) {
		int r = RandInt(0, 200000);
		text = activeSearcher->getWord(r).getWord();
	}

	return activeSearcher->searchWord(text);
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
void Dictionary::addNewWord(Word& newWord) {
	string text = newWord.getWord();
	auto v = newWord.getDefinitions();
	for (auto& def : v) {
		string defStr = def.getStringDefinition();
		addNewWordOneDef(text, defStr);
	}
}

bool Dictionary::addNewWordOneDef(string& text, string& def) {
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

		if (lineText != text) {
			deletedFile.push_back(line);
			continue;
		}

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

void Dictionary::deleteWord(Word& word) {
	string text = word.getWord();
	auto v = word.getDefinitions();
	for (auto& defs : v) {
		string def = defs.getStringDefinition();
		deleteWordOneDef(text, def);
	}
}

int	Dictionary::deleteWordOneDef(string& text, string& def) {
	//check if word exists
	Word match = searchWordMatching(text);
	if (match.findDefinition(def) == -1) {
		return -1; //word does not exist 
	}

	ifstream fin; bool appear = false;
	string line;

	//check if it has been deleted already
	fin.open("DataSet\\" + activeDataSet + "\\deletedWords.txt");

	while (getline(fin, line)) {
		string lineText, lineDef;
		int i = 0;
		while (line[i] != '\t') {
			lineText.push_back(line[i]);
			++i;
		}
		++i;

		if (lineText != text) continue;

		if (lineDef == def) {
			fin.close();
			return 0; //word was deleted before
		}
	}

	fin.close();

	//adding it to deleted file

	ofstream fout;

	fout.open("DataSet\\" + activeDataSet + "\\deletedWords.txt", ios::app);

	fout << text << '\t' << def << '\n';

	fout.close();

	return 1; //word deleted successfully
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

string Dictionary::getActiveDataset() {
	return activeDataSet;
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

void Dictionary::merge(vector<string>& a, int l, int r, int mid) {
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

void Dictionary::mergeSort(vector<string>& a, int l, int r, int n) {
	if (l > r || l == r) return;

	int mid = l + (r - l) / 2;

	mergeSort(a, l, mid, n);
	mergeSort(a, mid + 1, r, n);

	merge(a, l, r, mid);
}

History Dictionary::getHistory() {
	return hist;
}

void Dictionary::editDefinition(string text, string def, int index) {
	string dir = mapStringToFile(text);
	ifstream fin;
	fin.open(dir);

	string prev, after;
	string word, line;

	if (fin.is_open()) {
		getline(fin, line);
		word = line.substr(0, line.find("\t"));
		while (word != text) {
			prev += line + "\n";
			getline(fin, line);
			word = line.substr(0, line.find("\t"));
		}
		for (int i = 0; i < index; ++i) {
			prev += line + "\n";
			getline(fin, line);
			word = line.substr(0, line.find("\t"));
		}
		prev += line.substr(0, line.find(")") + 2);

		string temp;
		while (getline(fin, temp))
			after += temp + "\n";
		fin.close();
	}

	ofstream fout;
	fout.open(dir);
	if (fout.is_open()) {
		fout << prev << def << "\n" << after;
		fout.close();
	}
}

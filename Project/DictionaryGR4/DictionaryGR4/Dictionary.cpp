#include "Dictionary.h"

using namespace std;

//"Eng-Eng", "Eng-Vie", "Vie-Eng"
bool Dictionary::chooseLanguage(string t) {
	if (t != EngEng && t != EngVie && t != VieEng) return false;
	activeDataSet = t;
	
	if (t == EngEng) activeSearcher = &toolEngEng;
	if (t == EngVie) activeSearcher = &toolEngVie;
	if (t == VieEng) activeSearcher = &toolVieEng;


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

void Dictionary::reloadWordFinder(string dataset) {
	if (dataset == "Eng-Eng") toolEngEng.load(dataset);
	else if (dataset == "Eng-Vie") toolEngVie.load(dataset);
	else if (dataset == "Vie-Eng") toolVieEng.load(dataset);
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

	//cautious with trie, it should be clear after searching
	if (!myTrie.empty()) w = myTrie.getWordMatching(word);
	if (w.empty()) w = activeSearcher->searchWord(word);
	
	//since some of w's definitions may be deleted by user, we have to check.
	getAvailableWords(w);

	return w;
}

Word* Dictionary::getWordPtr(string word) {
	return myTrie.getWordPointer(word);
}


Word Dictionary::getRandomWord(string& wordText) {
	string text;
	while (text.empty()) {
		int r = RandInt(0, 200000);
		text = activeSearcher->getWord(r).getText();
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
	string text = newWord.getText();
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
	string text = word.getText();
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
		if (comp == w.getText()) {
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
		if (ch != '.' && ch != ',' && ch != ';' && ch != '(' && ch != ')') {
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

vector<string> Dictionary::transformSentenceWithBracs(string& input) {
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

// edit 1-28.txt and addedWords.txt
bool Dictionary::editDefInFile(string text, string olddef, string newdef, string dir) {
	ifstream fin;
	fin.open(dir);

	string prev, after;
	string word, line;

	if (fin.is_open()) {
		if (!getline(fin, line))
			return 0;
		word = line.substr(0, line.find("\t"));
		while (word != text) {
			prev += line + "\n";
			if (!getline(fin, line))
				return 0;
			word = line.substr(0, line.find("\t"));
		}

		size_t i = line.find(")");
		if (i == string::npos)
			i = line.find("\t") + 1;
		else
			i += 2;

		string curdef = line.substr(i, line.length() - i);
		while (curdef.compare(olddef)) {
			prev += line + "\n";
			getline(fin, line);
			word = line.substr(0, line.find("\t"));
			if (word != text)
				return 0;
			i = line.find(")") + 2;
			curdef = line.substr(i, line.length() - i);
		}

		prev += line.substr(0, i);

		string temp;
		while (getline(fin, temp))
			after += temp + "\n";
		fin.close();
	}

	ofstream fout;
	fout.open(dir);
	if (fout.is_open()) {
		fout << prev << newdef << "\n" << after;
		fout.close();
		return 1;
	}
}

// edit on wordFinder and in sortedData.txt, sortedAddedWords.txt
void Dictionary::editDefOnWordFinder(string text, string olddef, string newdef, string pop) {
	int size = activeSearcher->getSize();
	int added = activeSearcher->getNumAdded();

	bool done = 0;

	// find in the added-part of wordFinder and edit in sortedAddedWords.txt
	olddef = pop + " " + olddef;
	vector<string> sortedOlddef = transformSentence(olddef);
	sortVectorString(sortedOlddef);

	newdef = pop + " " + newdef;
	vector<string> sortedNewdef = transformSentence(newdef);
	sortVectorString(sortedNewdef);

	for (int i = size; i < size + added; ++i) {
		vector<string> test = *(activeSearcher->getSubDef(i));
		if (activeSearcher->getWord(i).getText() == text and *(activeSearcher->getSubDef(i)) == sortedOlddef) {
			*(activeSearcher->getSubDef(i)) = sortedNewdef;

			string prev, after, line;

			ifstream fin;
			fin.open("DataSet\\" + activeDataSet + "\\sortedAddedWords.txt");
			if (fin.is_open()) {
				for (int j = size; j < i; ++j) {
					getline(fin, line);
					prev += line + "\n";
				}
				getline(fin, line);
				prev += line.substr(0, line.find("\t") + 1);
				while (getline(fin, line))
					after += line + "\n";
				fin.close();
			}

			ofstream fout;
			fout.open("DataSet\\" + activeDataSet + "\\sortedAddedWords.txt");
			fout << prev;
			for (int j = 0; j < sortedNewdef.size() - 1; ++j)
				fout << sortedNewdef[j] << " ";
			fout << sortedNewdef[sortedNewdef.size() - 1] << "\n" << after;
			fout.close();

			done = 1;
			break;
		}
	}

	// find and edit in normal part of wordFinder
	sortedOlddef = transformSentenceWithBracs(olddef);
	sortVectorString(sortedOlddef);

	sortedNewdef = transformSentenceWithBracs(newdef);
	sortVectorString(sortedNewdef);

	if (!done) {
		int i = activeSearcher->searchWordIndex(text);
		int t = i;

		bool doneUpper = 0;
		vector<string> test = *(activeSearcher->getSubDef(t));
		while (*(activeSearcher->getSubDef(t)) != sortedOlddef) {
			if (!doneUpper)
				--t;
			else
				++t;

			if (activeSearcher->getWord(t).getText() != text) {
				t = i;
				doneUpper = 1;
			}
		}

		*(activeSearcher->getSubDef(t)) = sortedNewdef;

		//ifstream fin;
		//string prev, after;
		//fin.open("DataSet\\" + activeDataSet + "\\sortedData.txt");
		//if (fin.is_open()) {
		//	string line;
		//	for (int j = 0; j < t; ++j) {
		//		getline(fin, line);
		//		prev += line + "\n";
		//	}
		//	getline(fin, line);
		//	prev += line.substr(0, line.find("\t") + 1);
		//	while (getline(fin, line))
		//		after += line + "\n";
		//	fin.close();
		//}
		//ofstream fout;
		//fout.open("DataSet\\" + activeDataSet + "\\sortedData.txt");
		//fout << prev;
		//for (int j = 0; j < sortedNewdef.size() - 1; ++j)
		//	fout << sortedNewdef[j] << " ";
		//fout << sortedNewdef[sortedNewdef.size() - 1] << "\n" << after;
		//fout.close();
	}

}

void Dictionary::editDefinition(string text, string olddef, string newdef, string pop) {
	string dir = mapStringToFile(text);

	if (!editDefInFile(text, olddef, newdef, "DataSet\\" + activeDataSet + "\\addedWords.txt"))
		editDefInFile(text, olddef, newdef, dir);

	editDefOnWordFinder(text, olddef, newdef, pop);
}

void Dictionary::addToFavourite(Word& word) {
	ofstream out;
	out.open("Favourite\\" + activeDataSet + "\\favList.txt", ios::app);
	if (!out.is_open()) return;
	out << word.getText() << endl;
	out.close();
	word.markFavourite();
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

bool Dictionary::isUnwantedPunctuation(char c) {
	return c == '.' || c == ',' || c == ';' || c == '(' || c == ')';
}

// Helper function to transform a single character
char Dictionary::transformChar(char c) {
	if (isUnwantedPunctuation(c)) {
		return ' '; // Replace unwanted punctuation with space
	}
	else {
		return std::tolower(c);
	}
}

std::string Dictionary::normalizeString(std::string& sentence) {
	std::string result;
	result.reserve(sentence.size());

	// Convert to lowercase and remove unwanted punctuation
	for (char c : sentence) {
		result.push_back(transformChar(c));
	}

	// Remove redundant spaces
	std::istringstream iss(result);
	std::string word;
	result.clear();
	bool firstWord = true;
	while (iss >> word) {
		if (!firstWord) {
			result += " ";
		}
		result += word;
		firstWord = false;
	}

	return result;
}
#pragma once 
#include "SearchTree.h"

//mapping a character to a child[i]

//char Trie::indexToChar(int i) {
//    if (i == 0) return ' ';
//    if (i == 1) return char(39); // '
//    if (i == 2) return '-';
//    if (3 <= i && i <= 12) return char(i + 45); // 0->9
//    if (13 <= i && i <= 38) return char(i + 84); // a -> z
//    if (i == 39) return '.';
//
//    return '*';
//}
//
//int Trie::charToIndex(char s) {
//    if (s == char(32)) return 0;
//    if (s == char(39)) return 1;
//    if (s == char(45)) return 2;
//    if (s == char(46)) return 39;
//
//    if (48 <= int(s) && int(s) <= 57) return int(s) - 45;
//    if (97 <= int(s) && int(s) <= 122) return int(s) - 84;
//
//    return -1;
//}

int Trie::indexToCodePoint(int i) {
    if (i == 0) return 32; // ' '
    if (i == 1) return 39; // '
    if (i == 2) return 45; // -
    if (3 <= i && i <= 12) return (i + 45); //0->9
    if (13 <= i && i <= 38) return (i + 84); //a->z

    int idx = i - 39;
    //vietkeys (67 chars), i = 39->105
    if (0 <= idx && idx <= 66) return codepoints[idx];

    if (i == 106) return 46; //.


    //unidentified
    return -1;
}

int Trie::codePointToIndex(int code) {
    if (code == 32) return 0; // idx 0 = ' '
    if (code == 39) return 1; // idx 1 = '
    if (code == 45) return 2; // idx 2 = -

    //idx 3->12 = 0,...,9
    if (48 <= code && code <= 57) return code - 45;
    //idx 13->38 = a,...z
    if (97 <= code && code <= 122) return code - 84;

    //idx 106 = .
    if (code == 46) return 106; //. 

    //idx 39->105: vietkeys
    int l = 0, r = 66;
    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (codepoints[mid] == code) {
            return (39 + mid);
        }
        else if (codepoints[mid] > code) {
            r = mid - 1;
        }
        else l = mid + 1;
    }
    //unidentified
    return -1;
}

void Trie::addWord(string word, string& def) {
    Node* p = root;
    auto it = word.begin();
    while(it!=word.end()){
        uint32_t f = utf8::next(it, word.end());

        int c = codePointToIndex(f);
        if (c == -1) return; //invalid

        if (p->child[c] == NULL) p->child[c] = new Node();

        p = p->child[c];
        p->cnt++;
    }
    p->exist++;
    p->emptyWord.addDefinition(def);

    ++size;
}

bool Trie::empty() {
    for (int i = 0; i < 107; ++i) {
        if (root->child[i]) return false;
    }
    return true;
}

bool Trie::findWord(string s) {
    Node* p = root;
    auto it = s.begin();
    while(it!=s.end()) {
        uint32_t f = utf8::next(it, s.end());

        int c = codePointToIndex(f);
        if (c == -1) return false; //invalid

        if (p->child[c] == NULL) return false;
        p = p->child[c];
    }
    return (p->exist != 0);
}


void Trie::clear() {
    deleteHelperAll(root);
    root = new Node();
}

void Trie::deleteHelperAll(Node*& pRoot) {
    if (!pRoot) return;
    for (int i = 0; i < 107; ++i) {
        deleteHelperAll(pRoot->child[i]);
    }
    size = 0;
    delete pRoot;
}





bool Trie::deleteWordRecursive(Node* p, string& s, int i) {
    if (i != (int)s.size()) {
        int c = codePointToIndex(int(s[i])); 
        if (c == -1) return false; //invalid
        bool isChildDeleted = deleteWordRecursive(p->child[c], s, i + 1);
        if (isChildDeleted) p->child[c] = NULL;
    }
    else p->exist--;

    if (p != root) {
        p->cnt--;
        if (p->cnt == 0) {
            delete(p);
            --size;
            return true;
        }
    }
    return false;
}

void Trie::removeWord(string s) {
    if (findWord(s) == false) return;

    deleteWordRecursive(root, s, 0);
}

void Trie::saveToFile(string& file) {
    ofstream fout;

    fout.open(file);

    vector<int> cps;
    traverseWrite(root, fout, cps);
    fout.close();
}

//get definitions of a string s
vector<Definition> Trie::getDefinitions(string s) {
    vector<Definition> empty;
    Node* p = root;
    auto it = s.begin();
    while (it!=s.end()) {
        uint32_t f = utf8::next(it, s.end());
        int c = codePointToIndex(f);
        if (c == -1) return empty; //invalid
        if (p->child[c] == NULL) return empty;
        p = p->child[c];
    }
    if (p->exist != 0) {
        return p->emptyWord.getDefinitions();
    }

    return empty;
}

//get word that matches string s
Word Trie::getWordMatching(string s) {
    Word ans; //empty word
    Node* p = root;
    auto it = s.begin();
    while (it!=s.end()) {
        uint32_t f = utf8::next(it, s.end());
        int c = codePointToIndex(f);
        if (c == -1) return ans; //invalid
        if (p->child[c] == NULL) return ans;
        p = p->child[c];
    }
    if (p->exist != 0) {
        ans = p->emptyWord;
        ans.setWord(s);

        return ans;
    }

    return ans;
}

Word* Trie::getWordPointer(string s)
{
    Word* ans = nullptr; //empty word
    Node* p = root;
    auto it = s.begin();
    while (it != s.end()) {
        uint32_t f = utf8::next(it, s.end());
        int c = codePointToIndex(f);
        if (c == -1) return ans; //invalid
        if (p->child[c] == NULL) return ans;
        p = p->child[c];
    }
    if (p->exist != 0) {
        ans = p->ptrToEmptyWord;
        //ans.setWord(s);

        return ans;
    }

    return ans;
}

Word* Trie::getRandomWord(string& wordText, string activeDataset) {
    int n = -1;

    if (activeDataset == "Eng-Eng" or activeDataset == "Eng-Vie")
        n = 39;
    else if (activeDataset == "Vie-Eng")
        n = 106;

    wordText = "";
    Node* cur = root;

    while (true) {
        if (!cur->hasChild()) {
            cur = root;
            wordText = "";
        }

        // iChild = 0 -> n or iChild = 106
        int t = rand() % (n + 1);
        int iChild = (t < n) ? t : 106;

        if (cur->child[iChild]) {
            cur = cur->child[iChild];
            wordText.push_back(indexToCodePoint(iChild));
            if (cur->exist) {
                bool take = rand() % 2;
                if (take) {
                    return cur->ptrToEmptyWord;
                }
            }
        }
    }
}

vector<string> Trie::getStringDefinitions(string s) {
    vector<string> empty;
    Node* p = root;
    auto it = s.begin();
    while (it!=s.end()) {
        uint32_t f = utf8::next(it, s.end());
        int c = codePointToIndex(f);
        if (c == -1) return empty; //invalid
        if (p->child[c] == NULL) return empty;
        p = p->child[c];
    }
    if (p->exist != 0) {
        return p->emptyWord.getStringDefinitions();
    }
    return empty;
}

void Trie::helperGetWordsPrefix(vector<int>& cp, Node* cur, vector<Word>& ans, bool& done, int& limit) {
    if (!cur || done) return;
    
    for (int i = 0; i < 107; ++i) {
        if ((int)ans.size() == limit) {
            done = true;
            return;
        }
        bool yes = false;
        if (cur->child[i]) {
           cp.push_back(indexToCodePoint(i));
           // temp.push_back(char(indexToCodePoint(i)));
            if (cur->child[i]->exist != 0) {

                u16string tmp16; for (int& x : cp) tmp16 += (char16_t)x;
                
                string str;

                utf8::utf16to8(tmp16.begin(), tmp16.end(), back_inserter(str));

                cur->child[i]->emptyWord.setWord(str);

                ans.push_back(cur->child[i]->emptyWord); //add to ans

                cur->child[i]->emptyWord.setWord("");
            }
            yes = true;
        }

        
        helperGetWordsPrefix(cp, cur->child[i], ans, done, limit);
        if (yes && !cp.empty()) cp.pop_back();
    }
}

void Trie::traverseWrite(Node*& cur, ofstream& fout, vector<int>& cps) {
    if (!cur) return;
    
    for (int i = 0; i < 107; ++i) {
        cps.push_back(indexToCodePoint(i));
        traverseWrite(cur->child[i], fout, cps);

        if (cur->child[i] && cur->child[i]->exist != 0) {
            u16string str;
            for (auto& c : cps) {
                str += (char16_t)c;
            }

            string s;
            utf8::utf16to8(str.begin(), str.end(), back_inserter(s));

            for (auto& def : cur->child[i]->emptyWord.getDefinitions()) {
                string d = def.getStringDefinition();
                fout << s << '\t' << d << '\n';
            }
        }
        cps.pop_back();
        

        //if (cur->child[i]) cps.pop_back();
    }
}

vector<Word> Trie::getWordsWithPrefix(string s, int& limit) {
    vector<Word> empty;
    Node* p = root;
    auto it = s.begin();
    while (it!=s.end()) {
        uint32_t f = utf8::next(it, s.end());
        int c = codePointToIndex(f);
        if (c == -1) return empty; //invalid
        if (p->child[c] == NULL) return empty;
        p = p->child[c];
    }
    if (p->exist != 0) {
        p->emptyWord.setWord(s);
        empty.push_back(p->emptyWord);
        p->emptyWord.setWord("");
    }

    bool done = false;

    vector<int> cp;
    it = s.begin();

    while (it != s.end()) {
        uint32_t q = utf8::next(it, s.end());
        cp.push_back(q);
    }

    helperGetWordsPrefix(cp, p, empty, done, limit);

    return empty;
}

//vector<Word> Trie::getClosestMatchWords(string s, int& desired) {
//
//    //suggested words will differ our word by at most one position
//
//    vector<Word> ans;
//
//    vector<string> possible;
//
//    for (int i = 0; i < (int)s.length(); ++i) {
//        char old = s[i];
//        for (int idx = 0; idx < 39; ++idx) {
//            char l = indexToChar(idx);
//            s[i] = l;
//            possible.push_back(s);
//        }
//        s[i] = old;
//    }
//
//    for (auto& str : possible) {
//        Word temp = getWordMatching(str);
//        if (!temp.empty()) ans.push_back(temp);
//        if ((int)ans.size() == desired) return ans;
//    }
//
//    return ans;
//}

int Trie::getSize() {
    return size;
}

bool Trie::loadData(string file, string dataset) {
    string file2 = "DataSet\\" + dataset + "\\addedWords.txt";

    ifstream fin;
    fin.open(file);
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            string s, t;
            int i = 0;
            while (line[i] != '\t') {
                s.push_back(line[i]);
                ++i;
            }

            ++i;

            t = line.substr(i, (int)line.length());

            addWord(s, t);
            //cout << s << '\n';
        }
    }
    else {
        fin.close();
        return false;
    }
    
    fin.close();

    //added words by user
    fin.open(file2);
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            string s, t;
            int i = 0;
            while (line[i] != '\t') {
                s.push_back(line[i]);
                ++i;
            }

            ++i;

            t = line.substr(i, (int)line.length());

            addWord(s, t);
            //cout << s << '\n';
        }
    }
    else {
        fin.close();
        return false;
    }

    fin.close();

    return true;
}


void WordFinder::addNewWord(string s, string def) {
    slots[size + numWordsAdded].setWord(s, def);
    //remember to sort the subdef

    string sortedDef = transformSentence(def);

    stringstream iss(sortedDef);
    string subdef;

    //add the subdefs
    while (iss >> subdef) addSubDef(subdef, size + numWordsAdded);

    //sort for the sake of finding
    slots[size + numWordsAdded].arrange();

    ++numWordsAdded;
}

void WordFinder::addSubDef(string subdef, int order) {
    slots[order].addSubDef(subdef);
}

void WordFinder::load(string dataset) {
    //load from processed data
    int curbucket = 0;

    ifstream fin;
    fin.open("DataSet\\" + dataset + "\\sortedData.txt");

    if (!fin.is_open()) {
        fin.close();
        return;
    }

    string line; //1 line = 1 bucket
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string s;
        int i = 0;
        while (line[i] != '\t') {
            s.push_back(line[i]);
            ++i;
        }

        ++i;

        string cur = "";
        for (int j = i; j < (int)line.length(); ++j) {
            if (line[j] == ' ') {
                if (cur != "") addSubDef(cur, curbucket);
                cur = "";
            }
            else cur.push_back(line[j]);
        }

        if (cur != "") addSubDef(cur, curbucket);


        //no need sorting
        //slots[curbucket].arrange();

        ++curbucket;
    }

    fin.close();
    size = curbucket;

    //read from user-added file


    fin.open("DataSet\\" + dataset + "\\sortedAddedWords.txt");
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string s;
        int i = 0;
        while (line[i] != '\t') {
            s.push_back(line[i]);
            ++i;
        }

        ++i;

        string cur = "";
        for (int j = i; j < (int)line.length(); ++j) {
            if (line[j] == ' ') {
                if (cur != "") addSubDef(cur, size+numWordsAdded);
                cur = "";
            }
            else cur.push_back(line[j]);
        }

        if (cur != "") addSubDef(cur, size+numWordsAdded);


        //no need sorting
        //slots[curbucket].arrange();

        ++numWordsAdded;
    }


    fin.close();


    //get actual words

    int cur = 0;
    for (int file = 1; file <= 28; ++file) {
        ifstream fin;
        fin.open("DataSet\\" + dataset + "\\" + to_string(file) + ".txt");

        if (!fin.is_open()) {
            fin.close();
            continue;
        }

        string line;
        while (getline(fin, line)) {
            string word = "";
            int j = 0;
            while (line[j] != '\t') {
                word.push_back(line[j]);
                ++j;
            }

            ++j;
            string def = line.substr(j, (int)line.length());

            slots[cur].setWord(word, def);
            ++cur;
        }

        fin.close();
    }

    //get actual words from user-added file

    fin.open("DataSet\\" + dataset + "\\addedWords.txt");

    cur = 0;

    while (getline(fin, line)) {
        string word = "";
        int j = 0;
        while (line[j] != '\t') {
            word.push_back(line[j]);
            ++j;
        }

        ++j;
        string def = line.substr(j, (int)line.length());

        slots[size+cur].setWord(word, def);
        cur++;
    }

    fin.close();


    fin.close();
}

void WordFinder::unload() {
    for (int i = 0; i < size; ++i) {
        slots[i].clear();
    }
}

//save to sorteddata.txt & addedSorted
void WordFinder::saveToFile(string dataset) {

    string file = "DataSet\\" + dataset + "\\sortedData.txt";

    ofstream fout;
    fout.open(file);

    if (!fout.is_open()) {
        fout.close();
        return;
    }
    //remember only write from 0->size, from size->size+numAdded is for added dataset
    for (int i = 0; i < size; ++i) {
        fout << slots[i].word.getWord() << '\t';
        for (int j = 0; j < (int)slots[i].subdef.size(); ++j) {
            fout << slots[i].subdef[j];
            if (j + 1 != (int)slots[i].subdef.size()) fout << ' ';
        }
        fout << '\n';
    }

    fout.close();

    string fileAdd = "DataSet\\" + dataset + "\\sortedAddedWords.txt";

    fout.open(fileAdd);

    for (int i = size; i < size + numWordsAdded; ++i) {
        fout << slots[i].word.getWord() << '\t';
        for (int j = 0; j < (int)slots[i].subdef.size(); ++j) {
            fout << slots[i].subdef[j];
            if (j + 1 != (int)slots[i].subdef.size()) fout << ' ';
        }
        fout << '\n';
    }

    fout.close();
}

Word WordFinder::searchWord(string text) {
    int left = 0, right = size - 1;

    bool yes = false;
    Word ans;

    int j = -1;

    u16string wxtest = tou16(text);

    //bin search for left most word matches text
    while (left <= right) {
        int mid = left + (right - left) / 2;
        string cur = slots[mid].word.getWord();

        u16string midstr = tou16(cur);

        if (midstr == wxtest) {
            j = mid;
            right = mid - 1;
        }
        else if (compare(midstr,wxtest) == 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (j != -1) { //move up to find same word, different def
        for (int i = j; i < size; ++i) {
            if (slots[i].word.getWord() != text) break;

            if (!yes) {
                ans.setWord(text);
                yes = true;
            }

            ans.addDefinition(slots[i].word.getDefinitions().back());
        }
    }


    //find possibly user-added word
    for (int i = size + numWordsAdded - 1; i >= size; --i) {
        string cur = slots[i].word.getWord();
        u16string u16cur = tou16(cur);
        if (u16cur == wxtest) {
            if (!yes) {
                ans.setWord(text);
                yes = true;
            }
            ans.addDefinition(slots[i].word.getDefinitions().back());
        }
    }

    return ans;
}

vector<Word> WordFinder::searchDefinitionsToWord(vector<string>& subkey, int limit) {
    vector<Word> ans;


    //loop through "size+numWordsAdded" slots
    for (int i = 0; i < size+numWordsAdded; ++i) {

        bool checkAns = true;

        for (auto& sub : subkey) {
            //BS on slots[i].subdef


            bool yes = binary_search(slots[i].subdef.begin(), slots[i].subdef.end(), sub);

            if (!yes) {
                checkAns = false;
                break;
            }
        }

        if (checkAns) ans.push_back(slots[i].word);
        if (ans.size() == limit) return ans;
    }

    return ans;
}

//helpers

bool WordFinder::isUnwantedPunctuation(char c) {
    return c == '.' || c == ',' || c == ';' || c == '(' || c == ')';
}

// Helper function to transform a single character
char WordFinder::transformChar(char c) {
    if (isUnwantedPunctuation(c)) {
        return ' '; // Replace unwanted punctuation with space
    }
    else {
        return std::tolower(c);
    }
}

std::string WordFinder::transformSentence(std::string& sentence) {
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

u16string WordFinder::tou16(string& s) {
    auto it = s.begin();
    u16string u16;
    while (it != s.end()) {
        uint32_t cp = utf8::next(it, s.end());
        u16 += (char16_t)cp;
    }

    return u16;
}

int WordFinder::getIndex(char16_t c) {
    auto iterator = find(alphabet.begin(), alphabet.end(), c);
    if (iterator != alphabet.end())
        return iterator - alphabet.begin();
    else
        return -1;
}

//true if s1 > s2, false otherwise
bool WordFinder::compare(u16string& s1, u16string& s2) {
    for (int i = 0; i <= min(s1.length(), s2.length()); ++i) {
        int idx1 = getIndex(s1[i]);

        //if (idx1 == 0)
        //    return 0;

        int idx2 = getIndex(s2[i]);
        if (idx1 < idx2)
            return 0;
        if (idx1 > idx2)
            return 1;
    }
    return 0;
}

//true if s1 > s2, false otherwise
bool WordFinder::compare(string& s1, string& s2) {
    for (int i = 0; i <= min(s1.length(), s2.length()); ++i) {
        int idx1 = getIndex((char16_t)s1[i]);

        //if (idx1 == 0)
        //    return 0;

        int idx2 = getIndex(s2[i]);
        if (idx1 < idx2)
            return 0;
        if (idx1 > idx2)
            return 1;
    }
    return 0;
}
#pragma once 
#include "SearchTree.h"

namespace fs = std::experimental::filesystem;

//mapping a character to a child[i]

char Trie::indexToChar(int i) {
    if (i == 0) return ' ';
    if (i == 1) return char(39); // '
    if (i == 2) return '-';
    if (3 <= i && i <= 12) return char(i + 45); // 0->9
    if (13 <= i && i <= 38) return char(i + 84); // a -> z
    if (i == 39) return '.';

    return '*';
}

int Trie::charToIndex(char s) {
    if (s == char(32)) return 0;
    if (s == char(39)) return 1;
    if (s == char(45)) return 2;
    if (s == char(46)) return 39;

    if (48 <= int(s) && int(s) <= 57) return int(s) - 45;
    if (97 <= int(s) && int(s) <= 122) return int(s) - 84;

    return -1;
}

void Trie::addWord(string word, string& def) {
    Node* p = root;
    for (auto f : word) {
        int c = charToIndex(f);
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
    for (int i = 0; i < 40; ++i) {
        if (root->child[i]) return false;
    }
    return true;
}

bool Trie::findWord(string s) {
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
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
    for (int i = 0; i < 40; ++i) {
        deleteHelperAll(pRoot->child[i]);
    }
    size = 0;
    delete pRoot;
}





bool Trie::deleteWordRecursive(Node* p, string& s, int i) {
    if (i != (int)s.size()) {
        int c = charToIndex(s[i]); 
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

//get definitions of a string s
vector<Definition> Trie::getDefinitions(string s) {
    vector<Definition> empty;
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
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
    for (auto f : s) {
        int c = charToIndex(f);
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


vector<string> Trie::getStringDefinitions(string s) {
    vector<string> empty;
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
        if (c == -1) return empty; //invalid
        if (p->child[c] == NULL) return empty;
        p = p->child[c];
    }
    if (p->exist != 0) {
        return p->emptyWord.getStringDefinitions();
    }
    return empty;
}

void Trie::helperGetWordsPrefix(string prefix, Node* cur, vector<Word>& ans, bool& done, int& limit) {
    if (!cur || done) return;
    
    string temp = prefix;
    for (int i = 0; i < 40; ++i) {
        if ((int)ans.size() == limit) {
            done = true;
            return;
        }
        bool yes = false;
        if (cur->child[i]) {
            temp.push_back(indexToChar(i));
            if (cur->child[i]->exist != 0) {
               
                cur->child[i]->emptyWord.setWord(temp);

                ans.push_back(cur->child[i]->emptyWord); //add to ans

                cur->child[i]->emptyWord.setWord("");
            }
            yes = true;
        }


        helperGetWordsPrefix(temp, cur->child[i], ans, done, limit);
        if (yes && !temp.empty()) temp.pop_back();
    }


}

vector<Word> Trie::getWordsWithPrefix(string s, int& limit) {
    vector<Word> empty;
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
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

    helperGetWordsPrefix(s, p, empty, done, limit);

    return empty;
}

vector<Word> Trie::getClosestMatchWords(string s, int& desired) {

    //suggested words will differ our word by at most one position

    vector<Word> ans;

    vector<string> possible;

    for (int i = 0; i < (int)s.length(); ++i) {
        char old = s[i];
        for (int idx = 0; idx < 39; ++idx) {
            char l = indexToChar(idx);
            s[i] = l;
            possible.push_back(s);
        }
        s[i] = old;
    }

    for (auto& str : possible) {
        Word temp = getWordMatching(str);
        if (!temp.empty()) ans.push_back(temp);
        if ((int)ans.size() == desired) return ans;
    }

    return ans;
}

int Trie::getSize() {
    return size;
}

bool Trie::loadData(char key, string dataset) {
    string s;

    key = tolower(key);
    int num = charToIndex(key);


    //no character begins with numbers, spaces or special chars
    if (!(num == 1 || num == 2 || (13 <= num && num <= 38))) return false;


    if (num == 1) s = "DataSet\\" + dataset + "\\1.txt";
    else if (num == 2) s = "DataSet\\" + dataset + "\\2.txt";
    else {
        if (dataset == "Vie-Eng") num -= 2;
        num -= 10;
        string idx = to_string(num);

        s = "DataSet\\" + dataset + "\\" + idx + ".txt";
    }
    //cout << "Loading file: " << s << '\n';
    ifstream fin;
    fin.open(s);
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {

            string s, t;
            int i = 0;
            while (line[i] != '\t') {
                s.push_back(line[i]);
                ++i;
            }

            ++i;

            for (int j = i; j < (int)line.length(); ++j) t.push_back(line[j]);

            addWord(s, t);
            //cout << s << '\n';
        }
    }
    else return false;

    return true;
}

void Trie::resetDataset(string dataset) {
    fs::copy("DataSet - Backup\\" + dataset, "DataSet\\" + dataset);
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
}

void WordFinder::unload() {
    for (int i = 0; i < size; ++i) {
        slots[i].clear();
    }
}

Word WordFinder::searchWord(string text) {
    int left = 0, right = size - 1;

    bool yes = false;
    Word ans;

    int j = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (slots[mid].word.getWord() < text) {
            left = mid + 1;
        }
        else {
            if (slots[mid].word.getWord() == text) j = mid;
            right = mid - 1;
        }
    }

    if (j == -1) return ans;

    for (int i = j; i < size; ++i) {
        if (slots[i].word.getWord() != text) break;

        if (!yes) {
            ans.setWord(text);
            yes = true;
        }

        ans.addDefinition(slots[i].word.getDefinitions().back());
    }

    return ans;
}

vector<Word> WordFinder::searchDefinitionsToWord(vector<string>& subkey, int limit) {
    vector<Word> ans;


    //loop through "size" slots
    for (int i = 0; i < size; ++i) {

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
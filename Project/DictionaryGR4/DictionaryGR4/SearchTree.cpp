#pragma once 
#include "SearchTree.h"

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

    ++cur;
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
    cur = 0;
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
            --cur;
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
list<Definition> Trie::getDefinitions(string s) {
    list<Definition> empty;
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


list<string> Trie::getStringDefinitions(string s) {
    list<string> empty;
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

void Trie::helperGetWordsPrefix(string prefix, Node* cur, list<Word>& ans, bool& done, int& desired) {
    if (!cur || done) return;
    
    string temp = prefix;
    for (int i = 0; i < 40; ++i) {
        if ((int)ans.size() == desired) {
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


        helperGetWordsPrefix(temp, cur->child[i], ans, done, desired);
        if (yes && !temp.empty()) temp.pop_back();
    }


}

list<Word> Trie::getWordsWithPrefix(string s, int& desired) {
    list<Word> empty;
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

    helperGetWordsPrefix(s, p, empty, done, desired);

    return empty;
}

int Trie::getSize() {
    return cur;
}
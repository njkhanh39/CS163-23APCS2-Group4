#include "SearchTree.h"
void Trie::traverse() {
    std::string temp;
    traverseHelp(root, temp);
}

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

void Trie::traverseHelp(Node*& pRoot, std::string& s) {
    if (!pRoot) return;
    if (pRoot->exist == 1) std::cout << s << '\n';
    for (int i = 0; i < 40; ++i) {
        if (pRoot->child[i]) {

            s.push_back(indexToChar(i));

            traverseHelp(pRoot->child[i], s);
            s.pop_back();
        }
    }
}

bool Trie::empty() {
    for (int i = 0; i < 40; ++i) {
        if (root->child[i]) return false;
    }
    return true;
}

void Trie::deleteHelper(Node*& pRoot) {
    if (!pRoot) return;
    for (int i = 0; i < 40; ++i) {
        deleteHelper(pRoot->child[i]);
    }
    cur = 0;
    delete pRoot;
}


void Trie::addWord(string word, string& def) {
    Node* p = root;
    for (auto f : word) {
        int c = charToIndex(f);

        if (p->child[c] == NULL) p->child[c] = new Node();

        p = p->child[c];
        p->cnt++;
    }
    p->exist++;
    p->emptyWord.addDefinition(def);

    ++cur;
}

void Trie::clear() {
    deleteHelper(root);
    root = new Node();
}

bool Trie::delete_string_recursive(Node* p, string& s, int i) {
    if (i != (int)s.size()) {
        int c = charToIndex(s[i]);
        bool isChildDeleted = delete_string_recursive(p->child[c], s, i + 1);
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

void Trie::delete_string(string s) {
    if (find_string(s) == false) return;

    delete_string_recursive(root, s, 0);
}

bool Trie::find_string(string s) {
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
        if (p->child[c] == NULL) return false;
        p = p->child[c];
    }
    return (p->exist != 0);
}

list<Definition*> Trie::getDefinitions(string s) {
    list<Definition*> empty;
    Node* p = root;
    for (auto f : s) {
        int c = charToIndex(f);
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
        if (p->child[c] == NULL) return empty;
        p = p->child[c];
    }
    if (p->exist != 0) {
        return p->emptyWord.getStringDefinitions();
    }

    return empty;
}

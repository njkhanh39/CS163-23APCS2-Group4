//#pragma once
//#include "Definition.h"
//#include "Word.h"
//#include <wx/wx.h>
//class Trie {
//public:
//    struct Node {
//        Node* child[39];
//        int exist, cnt;
//        Word emptyWord; //only stores definitions
//
//        Node() {
//            for (int i = 0; i < 39; i++) child[i] = NULL;
//            exist = cnt = 0;
//        }
//    };
//
//    int cur;
//    Node* root;
//    Trie() : cur(0) { root = new Node(); };
//
//    ~Trie() { deleteHelper(root); }
//
//    void traverse() {
//        std::string temp;
//        traverseHelp(root, temp);
//    }
//
//    char indexToChar(int i) {
//
//        if (i == 1) return char(39);
//        if (i == 2) return '-';
//        if (3 <= i && i <= 12) return char(i + 45);
//        if (13 <= i && i <= 38) return char(i + 84);
//
//        return ' ';
//    }
//
//    int charToIndex(char s) {
//        if (s == char(39)) return 1;
//        if (s == char(45)) return 2;
//
//        if (48 <= int(s) && int(s) <= 57) return int(s) - 45;
//        if (97 <= int(s) && int(s) <= 122) return int(s) - 84;
//
//        return 0;
//    }
//
//    void traverseHelp(Node*& pRoot, std::string& s) {
//        if (!pRoot) return;
//        if (pRoot->exist == 1) std::cout << s << '\n';
//        for (int i = 0; i < 39; ++i) {
//            if (pRoot->child[i]) {
//
//                s.push_back(indexToChar(i));
//
//                traverseHelp(pRoot->child[i], s);
//                s.pop_back();
//            }
//        }
//    }
//
//    void deleteHelper(Node*& pRoot) {
//        if (!pRoot) return;
//        for (int i = 0; i < 39; ++i) {
//            deleteHelper(pRoot->child[i]);
//        }
//        delete pRoot;
//    }
//
//    void add_string(std::string s) {
//
//        Node* p = root;
//        for (auto f : s) {
//            int c = charToIndex(f);
//
//            if (p->child[c] == NULL) p->child[c] = new Node();
//
//            p = p->child[c];
//            p->cnt++;
//        }
//        p->exist++;
//        ++cur;
//    }
//
//    void add_string(std::string s, std::string d) {
//
//        Node* p = root;
//        for (auto f : s) {
//            int c = charToIndex(f);
//
//            if (p->child[c] == NULL) p->child[c] = new Node();
//
//            p = p->child[c];
//            p->cnt++;
//        }
//        p->exist++;
//        p->emptyWord.addDefinition(d);
//        ++cur;
//    }
//
//
//    bool delete_string_recursive(Node* p, std::string& s, int i) {
//        if (i != (int)s.size()) {
//            int c = charToIndex(s[i]);
//            bool isChildDeleted = delete_string_recursive(p->child[c], s, i + 1);
//            if (isChildDeleted) p->child[c] = NULL;
//        }
//        else p->exist--;
//
//        if (p != root) {
//            p->cnt--;
//            if (p->cnt == 0) {
//                delete(p);
//
//                return true;
//            }
//        }
//        return false;
//    }
//
//    void delete_string(std::string s) {
//        if (find_string(s) == false) return;
//
//        delete_string_recursive(root, s, 0);
//    }
//
//    bool find_string(std::string s) {
//        Node* p = root;
//        for (auto f : s) {
//            int c = charToIndex(f);
//            if (p->child[c] == NULL) return false;
//            p = p->child[c];
//        }
//        return (p->exist != 0);
//    }
//
//    std::list<Definition*> getDefinitions(std::string s) {
//        std::list<Definition*> empty;
//        Node* p = root;
//        for (auto f : s) {
//            int c = charToIndex(f);
//            if (p->child[c] == NULL) return empty;
//            p = p->child[c];
//        }
//        if (p->exist != 0) {
//            return p->emptyWord.getDefinitions();
//        }
//
//        return empty;
//    }
//};
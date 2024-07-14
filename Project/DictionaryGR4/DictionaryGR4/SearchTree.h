#pragma once
#include "Word.h"
#include "fstream"

class Trie {
private:
    struct Node {
        Node* child[40];
        int exist, cnt;
        Word emptyWord; //only stores definitions

        Node() {
            for (int i = 0; i < 40; i++) child[i] = NULL;
            exist = cnt = 0;
        }
    };

    int cur;
    Node* root;

public:
    Trie() : cur(0) { root = new Node(); };

    ~Trie() { deleteHelperAll(root); }

    //mapping a character to a child[i]

    char indexToChar(int i);

    int charToIndex(char s);


    //add a word with definition
    void addWord(string word, string& def);

    //check if a word is empty
    bool empty();

    //check if a word exists in search tree
    bool findWord(string s);

    //clearing search tree
    void clear();

    void deleteHelperAll(Node*& pRoot);

    bool deleteWordRecursive(Node* p, string& s, int i);

    //remove a word from Trie
    void removeWord(string s);

    //get definitions of a string s
    list<Definition> getDefinitions(string s);

    //get string definitions as strings 
    list<string> getStringDefinitions(string s);

    void helperGetWordsPrefix(string prefix, Node* cur, list<Word>& ans, bool& done, int& desired);

    //Get words with prefix s, with desired limit (max ... words)
    list<Word> getWordsWithPrefix(string s, int& desired);

    //return number of added words (even duplicates)
    int getSize();

    //load

    bool loadDataEngEng(char key) {
        string s;

        key = tolower(key);
        int num = charToIndex(key);


        //no character begins with numbers, spaces or special chars
        if (!(num == 1 || num == 2 || (13 <= num && num <= 38))) return false;

        
        if (num == 1) s = "DataSet\\1_Apostrophe.txt";
        else if (num == 2) s = "DataSet\\2_Hyperphen.txt";
        else {
            num -= 10;
            string idx = to_string(num);
            key = toupper(key);
            string temp;
            temp.push_back('_');
            temp.push_back(key);

            s = "DataSet\\" + idx + temp + ".txt";
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
};


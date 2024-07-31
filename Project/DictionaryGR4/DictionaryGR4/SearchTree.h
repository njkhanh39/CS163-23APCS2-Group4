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

    //check if Trie is empty
    bool empty();

    //check if a word exists in search tree
    bool findWord(string s);

    //clearing search tree
    void clear();

    //remove a word from Trie
    void removeWord(string s);

    //get definitions of a string s
    vector<Definition> getDefinitions(string s);

    //get word that matches string s
    Word getWordMatching(string s);

    //get string definitions as strings 
    vector<string> getStringDefinitions(string s);

    
    //Get words with prefix s, with desired limit (max ... words)
    vector<Word> getWordsWithPrefix(string s, int& desired);

    //in case word typed does not match, we suggest words (customable limit)
    vector<Word> getClosestMatchWords(string s, int& desired) {
        
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

    //return number of added words (even duplicates)
    int getSize();

    //load

    bool loadDataEngEng(char key);

private:
    //helpers

    void deleteHelperAll(Node*& pRoot);
    bool deleteWordRecursive(Node* p, string& s, int i);
    void helperGetWordsPrefix(string prefix, Node* cur, vector<Word>& ans, bool& done, int& limit);
};



struct Bucket {
	Word word; 
	vector<string> subdef; //partition definition paragraph into sorted words

	Bucket() {
		word.setWord("");
	}

	Bucket(string keyword) {
		word.setWord(keyword);
	}

	void setWord(string keyword, string def) {
		word.setWord(keyword);
		word.addDefinition(def);
	}

	void addSubDef(string s) {
		subdef.push_back(s);
	}

	void arrange() {
		sort(subdef.begin(), subdef.end());
	}

    void clear() {
        word.clear();
        subdef.clear();
    }
};

class WordFinder {
private:
	Bucket* slots;
	int size = 0;
public:
	WordFinder() {
		slots = new Bucket[185000];
	}
	~WordFinder() {
		delete[] slots;
	}


	void addSubDef(string subdef, int order);

    void load(string dataset);

    void unload() {
        for (int i = 0; i < size; ++i) {
            slots[i].clear();
        }
    }

    Word searchWord(string text) {
        int left = 0, right = size - 1;

        bool yes = false;
        Word ans;

        int j = -1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (slots[mid].word.getWord() < text) {
                left= mid + 1;
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

    vector<Word> searchDefinitionsToWord(string key, int limit);

    /*void writeToFile(string filename) {
        ofstream fout;
        fout.open(filename);
        if (fout.is_open()) {
            for (int i = 0; i < size; ++i) {
                fout << slots[i].word.getWord() << '\t';
                for (int j = 0; j < (int)slots[i].subdef.size(); ++j) {
                    fout << slots[i].subdef[j];
                    if (j + 1 != (int)slots[i].subdef.size()) fout << ' ';
                    else fout << '\n';
                }
            }
        }
        fout.close();
    }*/
};







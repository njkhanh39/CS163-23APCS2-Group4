#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <fstream>
#include <experimental/filesystem>

#include "Word.h"

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

    int size;
    Node* root;

public:
    Trie() : size(0) { root = new Node(); };

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
    vector<Word> getClosestMatchWords(string s, int& desired);

    //return number of added words (even duplicates)
    int getSize();

    //load

    bool loadData(char key, string dataset);

    void resetDataset(string dataset);

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

    void unload();

    Word searchWord(string text);

    vector<Word> searchDefinitionsToWord(vector<string>& subkey, int limit);
       
};


// bool isUnwantedPunctuation(char c) {
//     return c == '.' || c == ',' || c == ';' || c == '(' || c == ')';
// }

// // Helper function to transform a single character
// char transformChar(char c) {
//     if (isUnwantedPunctuation(c)) {
//         return ' '; // Replace unwanted punctuation with space
//     }
//     else {
//         return std::tolower(c);
//     }
// }

// std::string transformSentence(std::string& sentence) {
//    std::string result;
//    result.reserve(sentence.size());

//    // Convert to lowercase and remove unwanted punctuation
//    for (char c : sentence) {
//        result.push_back(transformChar(c));
//    }

//    // Remove redundant spaces
//    std::istringstream iss(result);
//    std::string word;
//    result.clear();
//    bool firstWord = true;
//    while (iss >> word) {
//        if (!firstWord) {
//            result += " ";
//        }
//        result += word;
//        firstWord = false;
//    }

//    return result;
//}

// void loadThenWriteEngVie() {
//     int curbucket = 0;
//     for (int file = 1; file <= 28; ++file) {
//         ifstream fin;
//         fin.open("DataSet\\Eng-Vie\\" + to_string(file) + ".txt");

//         if (!fin.is_open()) {
//             fin.close();
//             continue;
//         }

//         string line;
//         while (getline(fin, line)) {
//             string s;
//             int i = 0;
//             while (line[i] != '\t') {
//                 s.push_back(line[i]);
//                 ++i;
//             }

//             ++i;

//             string tmp = line.substr(i, (int)line.length());
//             string so = transformSentence(tmp);

//             string cur = "";
//             for (int j = 0; j < (int)so.length(); ++j) {
//                 if (so[j] == ' ') {
//                     slots[curbucket].addSubDef(cur);
//                     cur = "";
//                 }
//                 else cur.push_back(so[j]);
//             }
//             slots[curbucket].addSubDef(cur);

//             slots[curbucket].arrange();

//             ++curbucket;
//         }

//         fin.close();
//     }

//     size = curbucket;

//     int cur = 0;
//     for (int file = 1; file <= 28; ++file) {
//         ifstream fin;
//         fin.open("DataSet\\Eng-Vie\\" + to_string(file) + ".txt");

//         if (!fin.is_open()) {
//             fin.close();
//             continue;
//         }

//         string line;
//         while (getline(fin, line)) {
//             string word = "";
//             int j = 0;
//             while (line[j] != '\t') {
//                 word.push_back(line[j]);
//                 ++j;
//             }

//             ++j;
//             string def = line.substr(j, (int)line.length());

//             slots[cur].setWord(word, def);
//             ++cur;
//         }

//         fin.close();
//     }

//     ofstream fout;
//     fout.open("dataSortedEngVie.txt");

//     if (!fout.is_open()) {
//         fout.close();
//         return;
//     }

//     string line;
//     for (int i = 0; i < size; ++i) {
//         fout << slots[i].word.getWord() << '\t';
//         for (int j = 0; j < (int)slots[i].subdef.size(); ++j) {
//             fout << slots[i].subdef[j];
//             if (j + 1 == (int)slots[i].subdef.size()) fout << '\n';
//             else fout << ' ';
//         }
//     }

//     fout.close();
// }

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
   

    
    




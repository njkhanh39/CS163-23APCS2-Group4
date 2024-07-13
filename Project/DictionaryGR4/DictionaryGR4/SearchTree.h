#pragma once
#include "Word.h"
#include "fstream"

class Trie {
public:
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
    Trie() : cur(0) { root = new Node(); };

    ~Trie() { deleteHelper(root); }

    char indexToChar(int i);

    int charToIndex(char s);

    //debugging

    void traverse();

    //for console, does not work here
    void traverseHelp(Node*& pRoot, std::string& s);



    //adding, finding

    //add a word with definition
    void addWord(string word, string& def);

    bool find_string(string s);

    bool empty();

    //deleting
    void clear();

    void deleteHelper(Node*& pRoot);

    bool delete_string_recursive(Node* p, string& s, int i);

    void delete_string(string s);


    //get definitions of a string s
    list<Definition*> getDefinitions(string s);

    list<string> getStringDefinitions(string s);

    //load

    bool loadData(char key) {
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


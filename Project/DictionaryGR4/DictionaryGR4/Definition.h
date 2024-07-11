#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <wx/wx.h>
class Word;

class Definition {
private:
    std::string text;
    Word* owner;
public:
    //constructors,destructor

    Definition(const std::string& defText, Word* owningWord)
        : text(defText), owner(owningWord) {}

    //setters

    //getters
    std::string getDefinitionString();


};


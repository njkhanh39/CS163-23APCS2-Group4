#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <wx/wx.h>
class Definition;

class Word {
private:
    std::string text;
    std::list<Definition*> defList;
public:
    //constructors,destructor

    Word(const std::string& wordText)
        : text(wordText) {}
    Word();
    ~Word();

    //setters - adders

    void addDefinition(const std::string& defText);

    void setText(std::string s);

    //getters

    std::list<Definition*> getDefinitions();

    //voids

    //void wxPrintWord();
   
};

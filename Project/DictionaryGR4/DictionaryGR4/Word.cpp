#include "Word.h"
#include "Definition.h"

//constructors,destructor

Word::Word() {
    text = "";
}

Word::~Word() {
    for (auto d : defList) delete d;
}

//setters - adders

void Word::addDefinition(const std::string& defText) {
    Definition* newDef = new Definition(defText, this);

    defList.push_back(newDef);
}

void Word::setText(std::string s) {
    text = s;
}

//getters

std::list<Definition*> Word::getDefinitions() {
    return defList;
}


//voids

//void Word::wxPrintWord() {
//    wxString word = wxString(text);
//    std::list<wxString> lstDef;
//
//    for (auto& d : defList) lstDef.push_back(d->getDefinitionString());
//
//    wxString large = word + "\n";
//
//    for (auto& s : lstDef) large += (s + "\n");
//
//    wxLogMessage(large);
//}
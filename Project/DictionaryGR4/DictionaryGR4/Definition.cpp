#include "Definition.h"

//constructor
Definition::Definition(string& defText, Word* owningWord) {
	text = defText;
	owner = owningWord;
}

//getters2


string Definition::getStringDefinition() {
	return text;
}

void Definition::setDefinition(string newText) {
	text = newText;
}
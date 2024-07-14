#include "Definition.h"

//constructor
Definition::Definition(string& defText, Word* owningWord) {
	text = defText;
	owner = owningWord;
}

//getters


string Definition::getStringDefinition() {
	return text;
}
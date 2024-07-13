#include "Definition.h"
Definition::Definition(string& defText, Word* owningWord) {
	text = defText;
	owner = owningWord;
}

string Definition::getDefinition() {
	return text;
}